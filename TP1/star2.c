#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <error.h>

#include "socketlib.h"

#define IP "127.0.0.1"
#define BASE_PORT 6000

/*
 * Master has index 0, asks for values to all slaves (that have to be already be
 * running), then computes the minimum.
 * Slaves have a non-zero index and wait for the master to ask a value, then
 * send it.
 */

int main(int argc, char const *argv[]) {

  if (argc != 3)
    error(1, 0, "Usage: %s [machine_index] [total_machines_nb]", argv[0]);

  int index = atoi(argv[1]);

  // Slave
  if (index) {
    // Choose a random integer
    srand(time(NULL));
    int my_value = rand();
    printf("My value is: %d\n", my_value);

    // Wait for the master to ask for it
    serve_int(my_value, IP, BASE_PORT + index - 1);

  // Master
  } else {
    int n_slaves = atoi(argv[2]) - 1;

    // Gather all values
    int values[n_slaves];
    for (int slave_idx=0; slave_idx < n_slaves; slave_idx++) {
      values[slave_idx] = ask_for_int(IP, BASE_PORT + slave_idx);
      printf("Received %d from machine %d.\n", values[slave_idx], slave_idx+1);
    }

    // Get the min
    int min_idx = 0;
    for (int idx=1; idx < n_slaves; idx++)
      if (values[min_idx] > values[idx])
        min_idx = idx;

    printf("Min found: %d (from machine %d)\n", values[min_idx],
           min_idx + 1);
  }

  return 0;
}
