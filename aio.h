// Copyright (c) 2017, eaugeas <eaugeas at gmail dot com>

#ifndef AIO_H_
#define AIO_H_

#ifdef __APPLE__
#error "macOS not implemented"
#elif __linux__
#include <sys/epoll.h>
typedef struct epoll_event aqevent_t;
#else
#error "platform not implemented"
#endif

#include <arpa/inet.h>

extern int aqcreate();

extern int aqclose(int queue);

extern int aqwait(int queue,
                  aqevent_t *events,
                  size_t len,
                  int64_t timeout_ms);

extern int aqadd(int queue,
                 int sockfd,
                 void *data);

extern int aqremove(int queue,
                    int sockfd);

extern void* aqget(const aqevent_t *event);

extern int aqis_closed(const aqevent_t *event);

extern int aqis_peer_closed(const aqevent_t *event);

extern int aqis_error(const aqevent_t *event);

extern int aqis_read(const aqevent_t *event);

extern int aqis_write(const aqevent_t *event);

extern int aqpipe(int fd[2]);

extern int aqsocket(int domain, int type, int protocol);

extern int aqaccept(int sockfd);

extern int aqlisten(int sockfd,
                    int backlog);

extern int aqconnect(int sockfd,
                     struct sockaddr* addr,
                     size_t addrlen);

#endif  //AIO_H_
