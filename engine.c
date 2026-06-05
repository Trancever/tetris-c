#include <time.h>
#include <errno.h>

#include "engine.h"

void sleep_ms(long milliseconds) {
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000L;

  while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
  }
}
