#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

  fcntl(socketfd, F_SETFL, O_NONBLOCK);

  struct sockaddr_in sin;
  memset(&sin,0, sizeof(sin)); // Hi!
  printf("Port before bind %d\n", ntohs(sin.sin_port));
  socklen_t len = sizeof(sin);

  int bindr = bind(socketfd, (struct sockaddr *) &sin, len);

  printf("Port after bind %d\n", ntohs(sin.sin_port));
  if (bindr == -1) {
    printf("Unable to bind: %d\n", errno);
    exit(2);
  }

  int listenr = listen(socketfd, 3);

  if (listenr == -1) {
    printf("Unable to listen: %d\n", errno);
    exit(3);
  }

  int nameres = getsockname(socketfd, (struct sockaddr *) &sin, &len);

  printf("Binded port: %d\n", ntohs(sin.sin_port));
  if (nameres == -1) {
    printf("Unable to get socket description: %d\n", errno);
    exit(5);
  }

  int pfds_len = 10;
  int active = 1;
  struct pollfd pfds[pfds_len];

  pfds[0].fd = socketfd;
  pfds[0].events = POLLIN;

  for(;;) {
    int readyfds = poll (pfds, 1, -1);
    if (readyfds == -1) {
      printf("Unable to poll: %d\n", errno);
      exit(7);
    }
    printf("Active users: %d\n", active);

    for(int i = 0; i < active ; i ++) {
      if (i == 0 ) {
        int happened = pfds[i].revents & POLLIN;
        if (happened) {
          if (active < pfds_len) {
            int acceptfd = accept(socketfd, (struct sockaddr *) &sin, &len);

            if (acceptfd == -1) {
              printf("Unable to accept: %d\n", errno);
              exit(4);
            }
            printf("Accepted port: %d\n", ntohs(sin.sin_port));
            pfds[active].fd = acceptfd;
            pfds[active].events = POLLIN;
            active++;
          } else {
            printf("Too many active users");
          }
        }
      } else {
        int happened = pfds[i].revents & POLLIN;
        if(happened) {
          uint8_t buf[16];
          int bytes_read = read(pfds[i].fd, (void *)buf, 16);

          printf("Bytes read: %d\n", bytes_read);
          if (bytes_read == -1) {
            printf("Unable to read: %d\n", errno);
            exit(5);
          }
        }
      }
    }
  }
}
