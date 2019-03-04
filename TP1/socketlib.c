#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <error.h>

#include "socketlib.h"
#define BUFFSIZE 50


int create_socket() {
  int sz = 1;
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sz, 4);
  return sock;
}


void bind_socket(int sock, char *address, int port) {
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(my_addr.sin_zero), 8);
  if (bind (sock, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) != 0)
    error(2, 0, "Cannot bind to %s:%d\n", address, port);
  listen(sock, 4);
}


void connect_socket(int sock, char *address, int port) {
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = inet_addr(address);
  bzero(&(my_addr.sin_zero), 8);
  if (connect (sock, (struct sockaddr *) &my_addr, sizeof my_addr) != 0)
    error(1, 0, "Cannot connect to %s:%d\n", address, port);
};


int accept_socket(int sock) {
  struct sockaddr_in from_addr;
  unsigned int size = 0;
  return accept(sock, (struct sockaddr *) &from_addr, &size);
}


void send_buffer(int sock, char *buffer, int bufferSize, int packetSize) {

  int max_idx = bufferSize / packetSize;
  for (int idx = 0; idx < max_idx; idx ++)
    write(sock, &buffer[idx * packetSize], packetSize);

  // Send remaining bytes
  int remaining = bufferSize - packetSize * max_idx;
  if (remaining != 0)
    write(sock, &buffer[max_idx * packetSize], remaining);

};


int recv_buffer(int sock, char *buffer, int bufferSize, int packetSize) {

  int count = -1, received = 0, idx = 0;
  while (count != 0) {
    count = read(sock, &buffer[idx * packetSize], packetSize);
    received += count;
    idx ++;
  }

  if (received > bufferSize)
    error(3, 0, "Receiving too much from socket %d\n", sock);

  return received;
};


void send_int(int to_send, char *address, int port) {

  int sock = create_socket();
  connect_socket(sock, address, port);
  char buf[BUFFSIZE];
  sprintf(buf, "%d", to_send);
  send_buffer(sock, buf, BUFFSIZE, BUFFSIZE);
  close(sock);

};


int recv_int(char *address, int port) {

  int sock1 = create_socket();
  bind_socket(sock1, address, port);
  int sock2 = accept_socket(sock1);
  char buf[BUFFSIZE];
  recv_buffer(sock2, buf, BUFFSIZE, BUFFSIZE);
  return atoi(buf);

};


int ask_for_int(char *address, int port) {

  // Connect
  int sock = create_socket();
  connect_socket(sock, address, port);

  // Listen to response
  char buf[BUFFSIZE];
  recv_buffer(sock, buf, BUFFSIZE, BUFFSIZE);

  close(sock);
  return atoi(buf);
};


void serve_int(int to_send, char *address, int port) {
  int sock1 = create_socket();
  bind_socket(sock1, address, port);

  // Listen to connections
  int sock2 = accept_socket(sock1);

  // Send response
  char buf[BUFFSIZE];
  sprintf(buf, "%d", to_send);
  send_buffer(sock2, buf, BUFFSIZE, BUFFSIZE);
  close(sock2);
  close(sock1);
};
