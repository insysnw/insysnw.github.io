#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

 printf("Yay\n");

}
