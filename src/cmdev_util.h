
#ifndef __CMDEV_UTIL_H__
#define __CMDEV_UTIL_H__


struct cmdev
{
  int cmd_pipe[2];
  struct event *cmd_ev;
};

typedef int (*cmd_func)(void *arg);


struct cmdev *
cmdev_util_new(struct event_base *evbase);

int
cmdev_util_free(struct cmdev *cmdev);

int
cmdev_util_sync_command(struct cmdev *cmdev, cmd_func func, void *arg);

int
cmdev_util_nonblock_command(struct cmdev *cmdev, cmd_func func, void *arg);

#endif /* __CMDEV_UTIL_H__ */
