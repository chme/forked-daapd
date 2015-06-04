/*
 * Copyright (C) 2015 Christian Meffert <christian.meffert@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <event2/event.h>

#include "logger.h"
#include "cmdev_util.h"


struct cmdev_command
{
  pthread_mutex_t lck;
  pthread_cond_t cond;

  cmd_func func;

  void *arg;

  int nonblock;

  int ret;
};


static void
command_cb(int fd, short what, void *arg)
{
  struct cmdev_command *cmd;
  struct cmdev *cmdev;
  int ret;

  cmdev = (struct cmdev *) arg;

  ret = read(cmdev->cmd_pipe[0], &cmd, sizeof(cmd));
  if (ret != sizeof(cmd))
    {
      DPRINTF(E_LOG, L_MAIN, "Could not read command! (read %d): %s\n", ret, (ret < 0) ? strerror(errno) : "-no error-");
      goto readd;
    }

  if (cmd->nonblock)
    {
      cmd->func(cmd->arg);

      free(cmd->arg);
      free(cmd);
      goto readd;
    }

  pthread_mutex_lock(&cmd->lck);

  ret = cmd->func(cmd);
  cmd->ret = ret;

  pthread_cond_signal(&cmd->cond);
  pthread_mutex_unlock(&cmd->lck);

 readd:
  event_add(cmdev->cmd_ev, NULL);
}

struct cmdev *
cmdev_util_new(struct event_base *evbase)
{
  struct cmdev *cmdev;
  int ret;

  cmdev = (struct cmdev*)calloc(sizeof(struct cmdev), 1);
  if (!cmdev)
    {
      DPRINTF(E_LOG, L_MAIN, "Out of memory for cmdev\n");
      return NULL;
    }

# if defined(__linux__)
  ret = pipe2(cmdev->cmd_pipe, O_CLOEXEC);
# else
  ret = pipe(cmdev->cmd_pipe);
# endif
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_MAIN, "Could not create command pipe: %s\n", strerror(errno));
      free(cmdev);
      return NULL;
    }

  cmdev->cmd_ev = event_new(evbase, cmdev->cmd_pipe[0], EV_READ, command_cb, cmdev);
  if (!cmdev->cmd_ev)
    {
      DPRINTF(E_LOG, L_MAIN, "Could not create cmd event\n");
      close(cmdev->cmd_pipe[0]);
      close(cmdev->cmd_pipe[1]);
      free(cmdev);
      return NULL;
    }

  ret = event_add(cmdev->cmd_ev, NULL);
  if (ret != 0)
    {
      DPRINTF(E_LOG, L_MAIN, "Could not add cmd event\n");
      close(cmdev->cmd_pipe[0]);
      close(cmdev->cmd_pipe[1]);
      free(cmdev);
      return NULL;
    }

  return cmdev;
}

int
cmdev_util_free(struct cmdev *cmdev)
{
  close(cmdev->cmd_pipe[0]);
  close(cmdev->cmd_pipe[1]);
  free(cmdev);

  return 0;
}

int
send_command(struct cmdev *cmdev, struct cmdev_command *cmd)
{
  int ret;

  if (!cmd->func)
    {
      DPRINTF(E_LOG, L_MAIN, "BUG: func is NULL!\n");
      return -1;
    }

  ret = write(cmdev->cmd_pipe[1], &cmd, sizeof(cmd));
  if (ret != sizeof(cmd))
    {
      DPRINTF(E_LOG, L_MAIN, "Could not send command: %s\n", strerror(errno));
      return -1;
    }

  return 0;
}

int
cmdev_util_sync_command(struct cmdev *cmdev, cmd_func func, void *arg)
{
  struct cmdev_command *cmd;
  int ret;

  cmd = (struct cmdev_command*)calloc(sizeof(struct cmdev_command), 1);
  cmd->func = func;
  cmd->arg = arg;
  cmd->nonblock = 0;

  pthread_mutex_lock(&cmd->lck);

  ret = send_command(cmdev, cmd);
  if (ret < 0)
    {
      pthread_mutex_unlock(&cmd->lck);
      return -1;
    }

  pthread_cond_wait(&cmd->cond, &cmd->lck);
  pthread_mutex_unlock(&cmd->lck);

  ret = cmd->ret;

  free(cmd);

  return ret;
}

int
cmdev_util_nonblock_command(struct cmdev *cmdev, cmd_func func, void *arg)
{
  struct cmdev_command *cmd;
  int ret;

  cmd = (struct cmdev_command*)calloc(sizeof(struct cmdev_command), 1);
  cmd->func = func;
  cmd->arg = arg;
  cmd->nonblock = 1;

  ret = send_command(cmdev, cmd);
  if (ret < 0)
    return -1;

  return 0;
}
