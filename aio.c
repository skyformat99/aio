// Copyright (c) 2017, eaugeas <eaugeas at gmail dot com>

#include <errno.h>
#include <fcntl.h>

#define RECOVER_ON_ERROR(action, recover)  \
  if ((action) == -1) {                    \
    int tmp_errno = errno;                 \
    do {                                   \
      recover                              \
    } while(0);                            \
    errno = tmp_errno;                     \
    return -1;                             \
  }

#define RETURN_ON_ERROR(action) \
  if ((action) == -1) {         \
    return -1;                  \
  }

#ifdef __APPLE__
#include "aqkqueue.h"
#elif __linux__
#include "aqepoll.h"
#else
#error "platform not implemented"
#endif

#include "aio.h"

static inline int
__make_fd_async(int fd)
{
  int current_flags = fcntl(fd, F_GETFL, 0);
  RETURN_ON_ERROR(current_flags);

  int new_flags = current_flags | O_NONBLOCK;
  RETURN_ON_ERROR(fcntl(fd, F_SETFL, new_flags));

  return 0;
}

inline int
aqpipe(int fd[2]) {
  RETURN_ON_ERROR(pipe(fd));

  RECOVER_ON_ERROR(__make_fd_async(fd[0]), {
    close(fd[0]);
    close(fd[1]);
    });

  RECOVER_ON_ERROR(__make_fd_async(fd[1]), {
    close(fd[0]);
    close(fd[1]);
    });

  return 0;
}


inline int
aqsocket(int domain, int type, int protocol) {
  int sockfd = socket(domain, type, protocol);
  RETURN_ON_ERROR(sockfd);

  RECOVER_ON_ERROR(sockfd, {
      close(sockfd);
    });

  return sockfd;
}


inline int
aqaccept(int serverfd) {
  int sockfd = accept(serverfd, NULL, NULL);
  RETURN_ON_ERROR(sockfd);

  RECOVER_ON_ERROR(__make_fd_async(sockfd), {
      close(sockfd);
    });

  return sockfd;
}

inline int
aqbind(int sockfd,
       struct sockaddr* addr,
       size_t addrlen) {
  RETURN_ON_ERROR(setsockopt(sockfd, SOL_SOCKET,
                             SO_REUSEADDR, (int[]){1}, sizeof(int)));
  RETURN_ON_ERROR(bind(sockfd, addr, addrlen));
  return 0;
}

inline int
aqlisten(int sockfd,
         int backlog) {
  RETURN_ON_ERROR(listen(sockfd, backlog));
  return 0;
}


inline int
aqconnect(int sockfd,
          struct sockaddr* addr,
          size_t addrlen) {
  int res = connect(sockfd, addr, addrlen);
  if (res == -1 && errno != EINPROGRESS) {
    return -1;
  }

  return 0;
}
