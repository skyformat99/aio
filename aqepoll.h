// Copyright (c) 2017, eaugeas <eaugeas at gmail dot com>

#ifndef AQEPOLL_H_
#define AQEPOLL_H_

#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>

extern inline int
aqcreate() {
  return epoll_create(1);
}

extern inline int
aqclose(int queue) {
  return close(queue);
}

extern inline int
aqwait(int queue,
      struct epoll_event *events,
      size_t len,
      int64_t timeout_ms) {
  return epoll_wait(queue, events, len, timeout_ms);
}

extern inline int
aqadd(int queue,
      int fd,
      void *data) {
  struct epoll_event event;
  memset(&event, 0, sizeof(struct epoll_event));

  event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
  event.data.ptr = data;

  return epoll_ctl(queue, EPOLL_CTL_ADD, fd, &event);
}

extern inline int
aqremove(int queue,
        int fd) {
  struct epoll_event event;
  memset(&event, 0, sizeof(struct epoll_event));

  return epoll_ctl(queue, EPOLL_CTL_DEL, fd, &event);
}

extern inline void *
aqget(const struct epoll_event *event) {
  return event->data.ptr;
}

extern inline int
aqis_closed(const struct epoll_event *event) {
  return (event->events & EPOLLHUP);
}

extern inline int
aqis_peer_closed(const struct epoll_event *event) {
  return (event->events & EPOLLRDHUP);
}

extern inline int
aqis_error(const struct epoll_event *event) {
  return (event->events & EPOLLERR);
}

extern inline int
aqis_read(const struct epoll_event *event) {
  return (event->events & EPOLLIN);
}

extern inline int
aqis_write(const struct epoll_event *event) {
  return (event->events & EPOLLOUT);
}

#endif  //AQEPOLL_H_
