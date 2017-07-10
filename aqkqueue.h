// Copyright (c) 2017, eaugeas <eaugeas at gmail dot com>

#ifndef AQEPOLL_H_
#define AQEPOLL_H_

#include <sys/event.h>
#include <string.h>
#include <unistd.h>

extern inline int
aqcreate() {
  return kqueue();
}

extern inline int
aqclose(int queue) {
  return close(queue);
}

extern inline int
aqwait(int queue,
      struct kevent *events,
      size_t len,
      int64_t timeout_ms) {
  struct timespec t_spec = {
    .tv_sec = timeout_ms / 1000,
    .tv_nsec = (timeout_ms % 1000) * 1000000
  };

  return kevent(queue, NULL, 0, events, len, &t_spec);
}

extern inline int
aqadd(int queue,
     int fd,
     void *data) {
  struct kevent event[2];
  memset(event, 0, 2*sizeof(struct kevent));

  EV_SET(&event[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, data);
  EV_SET(&event[0], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, data);

  return kevent(queue, event, 2, NULL, 0, 0);
}

extern inline int
aqremove(int queue,
        int fd) {
  struct kevent event[2];
  memset(event, 0, 2*sizeof(struct kevent));

  EV_SET(&event[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  EV_SET(&event[0], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

  return kevent(queue, event, 2, NULL, 0, 0);
}

extern inline void *
aqget(const struct kevent *event) {
  return event->udata;
}

extern inline int
aqis_closed(const struct kevent *event) {
  return (event->flags & EV_EOF);
}

extern inline int
aqis_peer_closed(const struct kevent *event) {
  return (event->flags & EV_EOF);
}

extern inline int
aqis_error(const struct kevent *event) {
  return (event->flags & EV_ERROR);
}

extern inline int
aqis_read(const struct kevent *event) {
  return (event->filter & EVFILT_READ);
}

extern inline int
aqis_write(const struct kevent *event) {
  return (event->filter & EVFILT_WRITE);
}

#endif  //AQEPOLL_H_
