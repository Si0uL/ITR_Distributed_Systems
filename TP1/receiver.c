#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 30000
#define IPADDRESS "127.0.0.1"
#define BUFFSIZE 16

int main (int argc, char *argv[]) {

  // Init
  int received = 0, cnt = 0;
  char buf[BUFFSIZE];

  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in my_addr;

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = inet_addr(IPADDRESS);
  bzero(&(my_addr.sin_zero), 8);

  if (bind (sock, (struct sockaddr *) &my_addr,
            sizeof (struct sockaddr)) != 0) {
    printf("%s\n", "Problem whilst binding");
    return 1;
  }

  listen (sock, 4);

  printf("Listening to port %d\n", PORT);

  struct sockaddr_in from;
  unsigned int size = 0;
  int sock2;
  sock2 = accept(sock, (struct sockaddr *) &from, &size);

  printf("%s\n", "Emitter connected");

  while ((cnt = read(sock2, buf, BUFFSIZE)) > 0)
    received += cnt;

  return 0;
}
