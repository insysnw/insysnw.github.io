#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdint.h> // new
#include <unistd.h> // new
#include <errno.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

// < new

  uint8_t buf[16];
  int bytes_read = read(socketfd, (void *)buf, 16);

  printf("Bytes read: %d\n", bytes_read);
  if (bytes_read == -1) {
    printf("Unable to read: %d\n", errno);
    exit(1);
  }

// new >
}
