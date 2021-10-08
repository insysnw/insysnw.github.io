#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

  int acceptfd = -1;
  while (acceptfd == -1){
    acceptfd = accept(socketfd, (struct sockaddr *) &sin, &len);
  }

  printf("Accepted port: %d\n", ntohs(sin.sin_port));

  uint8_t buf[16];
  int bytes_read = read(acceptfd, (void *)buf, 16);

  printf("Bytes read: %d\n", bytes_read);
  if (bytes_read == -1) {
    printf("Unable to read: %d\n", errno);
    exit(5);
  }
}
