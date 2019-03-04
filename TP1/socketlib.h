int create_socket();

void bind_socket(int sock, char *address, int port);

void connect_socket(int sock, char *address, int port);

int accept_socket(int sock);

void send_buffer(int sock, char *buffer, int bufferSize, int packetSize);

int recv_buffer(int sock, char *buffer, int bufferSize, int packetSize);

void send_int(int to_send, char *address, int port);

int recv_int(char *address, int port);

int ask_for_int(char *address, int port);

void serve_int(int to_send, char *address, int port);
