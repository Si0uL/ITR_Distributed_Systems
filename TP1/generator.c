#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 30000
#define IPADDRESS "127.0.0.1"
#define SIZETOSEND 100000000

int main (int argc, char *argv[]) {

  int packetSize;
  packetSize = 100;

  char packet[packetSize]; // C99 only
  memset(packet, 'A', packetSize * sizeof(char));

  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in my_addr;

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = inet_addr(IPADDRESS);
  bzero(&(my_addr.sin_zero), 8);

  if (connect (sock, (struct sockaddr *) &my_addr, sizeof my_addr) != 0) {
    printf("%s\n", "Error while trying to connect");
    return (1);
  }

  int max_i = SIZETOSEND / packetSize;

  for (int i = 0; i < max_i; i ++)
    write(sock, packet, packetSize);
  close(sock);

  return 0;
}
