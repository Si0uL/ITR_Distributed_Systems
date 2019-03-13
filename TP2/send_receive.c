// Use me with 2 proc at least

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    char to_send[] = "Hello";
    printf("Sending: %s\n", to_send);
    MPI_Send(to_send, 6, MPI_CHARACTER, 1, 0, MPI_COMM_WORLD);
  }

  if (rank == 1) {
    sleep(2);
    char received[6];
    MPI_Recv(received, 6, MPI_CHARACTER, 0, 0, MPI_COMM_WORLD,
      MPI_STATUS_IGNORE);
    printf("Received: %s\n", received);
  }

  MPI_Finalize();

  return 0;
}
