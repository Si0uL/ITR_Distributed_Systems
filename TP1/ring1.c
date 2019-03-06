#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <error.h>

#include "socketlib.h"

#define IP "127.0.0.1"
#define BASE_PORT 6000
#define BUFFSIZE 50

/*
 * Everyone except machine 0 start by witing for the (estimated) min value, then
 * updates it with its own value, before sending it to the next machine. Only
 * machine 0 starts by sending its value, then waits for the min to arrive from
 * the last machine, and release it.
 *
 * Machine n listens to BASE_PORT + n
 */

int main(int argc, char const *argv[]) {

  if (argc != 3)
    error(1, 0, "Usage: %s [machine_index] [total_machines_nb]", argv[0]);

  int index = atoi(argv[1]);
  int n_machines = atoi(argv[2]);
  if (n_machines < 2)
    error(2, 0, "Total number of machines should be >= 2");

  // Choose a random integer
  srand(time(NULL));
  int my_value = rand();
  printf("My value is: %d\n", my_value);


  // Non zero machine
  if (index) {
    // Wait for the previous machine to send a value
    int current_min = recv_int(IP, BASE_PORT + index);
    printf("Received %d as current min.\n", current_min);

    // Update the minimum
    if (my_value < current_min)
      current_min = my_value;

    // Wait for next node to be listening then send it the min
    int sock = create_socket();
    int next_machine = (index + 1) % n_machines;
    while (connect_socket(sock, IP, BASE_PORT + next_machine) != 0) {
      printf("\rWaiting for machine %d...", next_machine);
      fflush(stdout);
      sleep(1);
      printf("\r                         ");
    }
    printf("\r");
    char buf[BUFFSIZE];
    sprintf(buf, "%d", current_min);
    send_buffer(sock, buf, BUFFSIZE, BUFFSIZE);
    close(sock);

  // Machine zero
  } else {
    // Wait for next node to be listening then send it my_value
    int sock = create_socket();
    int next_machine = (index + 1) % n_machines;
    while (connect_socket(sock, IP, BASE_PORT + next_machine) != 0) {
      printf("\rWaiting for machine %d...", next_machine);
      fflush(stdout);
      sleep(1);
      printf("\r                         ");
    }
    printf("\r");
    char buf[BUFFSIZE];
    sprintf(buf, "%d", my_value);
    send_buffer(sock, buf, BUFFSIZE, BUFFSIZE);
    close(sock);

    // Wait for the previous machine to send a value
    int current_min = recv_int(IP, BASE_PORT + index);
    printf("Received %d as current min.\n", current_min);

    // Update the minimum
    if (my_value < current_min)
      current_min = my_value;

    printf("Global min found: %d\n", current_min);
  }

  return 0;
}
