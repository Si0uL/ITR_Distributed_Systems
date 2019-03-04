void send_buffer(char *buffer, int bufferSize, int packetSize, char *address,
  int port);

void send_int(int to_send, char *address, int port);

int recv_buffer(char *buffer, int bufferSize, int packetSize, char *address,
  int port);

int recv_int(char *address, int port);
