// Use me with 2 proc at least

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char received[5];

  if (rank == 0) {

    char to_send[] = "PING";
    while (1) {
      printf("[0] sending: %s\n", to_send);
      MPI_Send(to_send, 5, MPI_CHARACTER, 1, 0, MPI_COMM_WORLD);

      sleep(1);
      MPI_Recv(received, 5, MPI_CHARACTER, 1, 0, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
      printf("[0] received: %s\n", received);
    }

  }

  if (rank == 1) {

    char to_send[] = "PONG";
    while (1) {
      MPI_Recv(received, 5, MPI_CHARACTER, 0, 0, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
      printf("[1] received: %s\n", received);
      
      printf("[1] sending: %s\n", to_send);
      MPI_Send(to_send, 5, MPI_CHARACTER, 0, 0, MPI_COMM_WORLD);

    }

  }

  MPI_Finalize();

  return 0;
}
