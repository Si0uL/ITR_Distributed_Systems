#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <error.h>

#include "socketlib.h"
#define BUFFSIZE 50

void send_buffer(char *buffer, int bufferSize, int packetSize, char *address,
  int port) {

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = inet_addr(address);
  bzero(&(my_addr.sin_zero), 8);

  if (connect (sock, (struct sockaddr *) &my_addr, sizeof my_addr) != 0)
    error(1, 0, "Cannot connect to %s:%d\n", address, port);

  int max_idx = bufferSize / packetSize;
  for (int idx = 0; idx < max_idx; idx ++)
    write(sock, &buffer[idx * packetSize], packetSize * sizeof(char));

  // Send remaining bytes
  int remaining = bufferSize - packetSize * max_idx;
  if (remaining != 0)
    write(sock, &buffer[max_idx * packetSize], remaining * sizeof(char));

  close(sock);
};


void send_int(int to_send, char *address, int port) {

  char buf[BUFFSIZE];
  sprintf(buf, "%d", to_send);
  send_buffer(buf, BUFFSIZE, BUFFSIZE, address, port);

};


int recv_buffer(char *buffer, int bufferSize, int packetSize, char *address,
  int port) {

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = inet_addr(address);
  bzero(&(my_addr.sin_zero), 8);

  if (bind (sock, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) != 0)
    error(2, 0, "Cannot bind to %s:%d\n", address, port);

  listen(sock, 4);

  struct sockaddr_in from_addr;
  unsigned int size = 0;
  int sock2 = accept(sock, (struct sockaddr *) &from_addr, &size);

  int count = -1, received = 0, idx = 0;
  while (count != 0) {
    count = read(sock2, &buffer[idx * packetSize], packetSize * sizeof(char));
    received += count;
    idx ++;
  }

  if (received > bufferSize)
    error(3, 0, "Receiving too much from %s:%d\n", address, port);

  return received;
};


int recv_int(char *address, int port) {

  char buf[BUFFSIZE];
  recv_buffer(buf, BUFFSIZE, BUFFSIZE, address, port);
  return atoi(buf);

};
