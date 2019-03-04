#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <error.h>

#include "socketlib.h"

#define IP "127.0.0.1"
#define BASE_PORT 6000

/*
 * Master has index 0, waits for all slaves to send him their values, then
 * computes the minimum.
 * Slaves have a non-zero index and send their random value to the master.
 * Master has to be running and slaves have to be launched in order.
 */

int main(int argc, char const *argv[]) {

  if (argc != 3)
    error(1, 0, "Usage: %s [processus_index] [Machines_nb]", argv[0]);

  int index = atoi(argv[1]);

  // Slave
  if (index) {
    // Choose a random integer
    srand(time(NULL));
    int my_value = rand();
    printf("My value is: %d\n", my_value);

    // Send it to the master
    send_int(my_value, IP, BASE_PORT + index - 1);

  // Master
  } else {
    int n_slaves = atoi(argv[2]) - 1;

    // Received all values
    int values[n_slaves];
    for (int slave_idx=0; slave_idx < n_slaves; slave_idx++) {
      values[slave_idx] = recv_int(IP, BASE_PORT + slave_idx);
      printf("Received value from machine %d.\n", slave_idx + 1);
    }

    // Get the max
    int min_idx = 0;
    for (int idx=1; idx < n_slaves; idx++)
      if (values[min_idx] > values[idx])
        min_idx = idx;

    printf("Min found: %d (from machine %d)\n", values[min_idx],
           min_idx + 1);
  }

  return 0;
}
