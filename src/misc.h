#ifndef __MISC_H__
#define __MISC_H__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/* ------------------------ Network utility functions ----------------------- */

#include <sys/socket.h>
#include <netinet/in.h>

union net_sockaddr
{
  struct sockaddr_in sin;
  struct sockaddr_in6 sin6;
  struct sockaddr sa;
  struct sockaddr_storage ss;
};

// Checks if the address is in a network that is configured as trusted
bool
net_peer_address_is_trusted(union net_sockaddr *naddr);

int
net_address_get(char *addr, size_t addr_len, union net_sockaddr *naddr);

int
net_port_get(short unsigned *port, union net_sockaddr *naddr);

int
net_if_get(char *ifname, size_t ifname_len, const char *addr);

// Returns the socket fd from socket(), -1 on error
int
net_connect(const char *addr, unsigned short port, int type, const char *log_service_name);

// Returns the socket fd from socket(), -1 on error
int
net_bind(short unsigned *port, int type, const char *log_service_name);

int
net_bind_with_reuseport(short unsigned *port, int type, const char *log_service_name);

// To avoid polluting namespace too much we don't include event2/http.h here
struct evhttp;

int
net_evhttp_bind(struct evhttp *evhttp, unsigned short port, const char *log_service_name);

// Just checks if the protocol is http or https
bool
net_is_http_or_https(const char *url);

/* ----------------------- Conversion/hashing/sanitizers -------------------- */

#ifdef HAVE_ENDIAN_H
# include <endian.h>
#elif defined(HAVE_SYS_ENDIAN_H)
# include <sys/endian.h>
#elif defined(HAVE_LIBKERN_OSBYTEORDER_H)
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#endif

// Samples to bytes, bytes to samples
#define STOB(s, bits, c) ((s) * (c) * (bits) / 8)
#define BTOS(b, bits, c) ((b) / ((c) * (bits) / 8))

#define ARRAY_SIZE(x) ((unsigned int)(sizeof(x) / sizeof((x)[0])))

#ifndef MIN
# define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
# define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

// If you have something like "#define MYNUMBER 3" then NTOSTR(MYNUMBER) will be
// the string value, so "3".
#define NTOSTR_HELPER(x) #x
#define NTOSTR(x) NTOSTR_HELPER(x)

int
safe_atoi32(const char *str, int32_t *val);

int
safe_atou32(const char *str, uint32_t *val);

int
safe_hextou32(const char *str, uint32_t *val);

int
safe_atoi64(const char *str, int64_t *val);

int
safe_atou64(const char *str, uint64_t *val);

int
safe_hextou64(const char *str, uint64_t *val);

char *
safe_strdup(const char *str);

char *
safe_asprintf(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));

int
safe_snprintf_cat(char *dst, size_t n, const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));

int
safe_snreplace(char *s, size_t sz, const char *pattern, const char *replacement);

char *
unicode_fixup_string(char *str, const char *fromcode);

// Modifies str so it is trimmed. Returns pointer to str.
char *
trim(char *str);

// Copies the trimmed part of str to a newly allocated string (caller must free)
char *
atrim(const char *str);

void
swap_pointers(char **a, char **b);

uint32_t
djb_hash(const void *data, size_t len);

int64_t
two_str_hash(const char *a, const char *b);

uint8_t *
b64_decode(int *dstlen, const char *src);

char *
b64_encode(const uint8_t *src, int srclen);

uint64_t
murmur_hash64(const void *key, int len, uint32_t seed);


/* --------------------------- Key/value functions -------------------------- */

struct onekeyval {
  char *name;
  char *value;

  struct onekeyval *next;
  struct onekeyval *sort;
};

struct keyval {
  struct onekeyval *head;
  struct onekeyval *tail;
};

struct keyval *
keyval_alloc(void);

int
keyval_add(struct keyval *kv, const char *name, const char *value);

int
keyval_add_size(struct keyval *kv, const char *name, const char *value, size_t size);

void
keyval_remove(struct keyval *kv, const char *name);

const char *
keyval_get(struct keyval *kv, const char *name);

void
keyval_clear(struct keyval *kv);

void
keyval_sort(struct keyval *kv);


/* ------------------------------- Ringbuffer ------------------------------- */

struct ringbuffer {
  uint8_t *buffer;
  size_t size;
  size_t write_avail;
  size_t read_avail;
  size_t write_pos;
  size_t read_pos;
};

int
ringbuffer_init(struct ringbuffer *buf, size_t size);

void
ringbuffer_free(struct ringbuffer *buf, bool content_only);

size_t
ringbuffer_write(struct ringbuffer *buf, const void* src, size_t srclen);

size_t
ringbuffer_read(uint8_t **dst, size_t dstlen, struct ringbuffer *buf);


/* ------------------------- Clock utility functions ------------------------ */

#include <time.h>

#ifndef HAVE_CLOCK_GETTIME

#ifndef CLOCK_REALTIME
#  define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#  define CLOCK_MONOTONIC 1
#endif

typedef int clockid_t;

int
clock_gettime(clockid_t clock_id, struct timespec *tp);

int
clock_getres(clockid_t clock_id, struct timespec *res);
#endif

#ifndef HAVE_TIMER_SETTIME

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};
typedef uint64_t timer_t;

int
timer_create(clockid_t clock_id, void *sevp, timer_t *timer_id);

int
timer_delete(timer_t timer_id);

int
timer_settime(timer_t timer_id, int flags, const struct itimerspec *tp,
              struct itimerspec *old);

int
timer_getoverrun(timer_t timer_id);

#endif

// Timer function for platforms without hi-res timers
int
clock_gettime_with_res(clockid_t clock_id, struct timespec *tp, struct timespec *res);

struct timespec
timespec_add(struct timespec time1, struct timespec time2);

int
timespec_cmp(struct timespec time1, struct timespec time2);

struct timespec
timespec_reltoabs(struct timespec relative);


/* ------------------------------- Media quality ---------------------------- */

// Bit flags for the sake of outputs announcing what they support
enum media_format {
  MEDIA_FORMAT_UNKNOWN = 0,
  MEDIA_FORMAT_PCM     = (1 << 0),
  MEDIA_FORMAT_WAV     = (1 << 1),
  MEDIA_FORMAT_MP3     = (1 << 2),
  MEDIA_FORMAT_ALAC    = (1 << 3),
  MEDIA_FORMAT_OPUS    = (1 << 4),
};

// For iteration
#define MEDIA_FORMAT_FIRST MEDIA_FORMAT_PCM
#define MEDIA_FORMAT_LAST MEDIA_FORMAT_OPUS
#define MEDIA_FORMAT_NEXT(f) (f << 1)

// Remember to adjust quality_is_equal() if adding elements
struct media_quality {
  int sample_rate;
  int bits_per_sample;
  int channels;
  int bit_rate;
};

bool
quality_is_equal(struct media_quality *a, struct media_quality *b);

enum media_format
media_format_from_string(const char *s);

const char *
media_format_to_string(enum media_format format);


/* -------------------------- Misc utility functions ------------------------ */

#include <pthread.h>

char **
buildopts_get(void);

// initialize mutex with error checking (not default on all platforms)
int
mutex_init(pthread_mutex_t *mutex);

// wrapper for gettid/pthread_getthreadid_np
int
thread_gettid();

// wrapper for pthread_getname_np/pthread_get_name_np
void
thread_getname(pthread_t thread, char *name, size_t len);

void
thread_getnametid(char *buf, size_t len);

// wrapper for pthread_setname_np/pthread_set_name_np
void
thread_setname(pthread_t thread, const char *name);

void
uuid_make(char *str);

int
linear_regression(double *m, double *b, double *r, const double *x, const double *y, int n);

char **
m_readfile(const char *path, int num_lines);


/* -------------------------------- Assertion ------------------------------- */

/* Check that the function returns 0, logging a fatal error referencing
   returned error (type errno) if it fails, and aborts the process.
   Example: CHECK_ERR(L_MAIN, my_function()); */
#define CHECK_ERR(d, f) \
  do { int chk_err; \
    if ( (chk_err = (f)) != 0) \
      log_fatal_err(d, #f, __LINE__, chk_err); \
  } while(0)

/* Check that the function returns 0 or okval, logging a fatal
   error referencing returned erro (type errno) if not, and aborts the process.
   Example: int err; CHECK_ERR_EXCEPT(L_MAIN, my_wait(), err, ETIMEDOUT); */
#define CHECK_ERR_EXCEPT(d, f, var, okval) \
  do { (var) = (f);                             \
    if (! (((var) == (okval)) || ((var) == 0))) \
      log_fatal_err(d, #f, __LINE__, (var)); \
  } while(0)

/* Check that the function returns value >= 0, logging a fatal error
   referencing errno if it not, and aborts the process.
   Example: int ret; CHECK_ERRNO(L_MAIN, ret = my_function()); */
#define CHECK_ERRNO(d, f) \
  do { \
    if ( (f) < 0 ) \
      log_fatal_errno(d, #f, __LINE__); \
  } while(0)

/* Check that the function returns non-NULL, logging a fatal error if not,
   and aborts the process.
   Example: void *ptr; CHECK_NULL(L_MAIN, ptr = my_create()); */
#define CHECK_NULL(d, f) \
  do { \
    if ( (f) == NULL ) \
      log_fatal_null(d, #f, __LINE__); \
  } while(0)

/* Used by CHECK_*() macros */
void
log_fatal_err(int domain, const char *func, int line, int err) __attribute__((__noreturn__));

void
log_fatal_errno(int domain, const char *func, int line)        __attribute__((__noreturn__));

void
log_fatal_null(int domain, const char *func, int line)         __attribute__((__noreturn__));

#endif /* !__MISC_H__ */
