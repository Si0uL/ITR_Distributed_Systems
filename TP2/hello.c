// mpirun -np <n_proc> ./hello.out

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int rank, n_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
  printf("Hello, I'm number %d out of %d\n", rank, n_proc);

  MPI_Finalize();

  return 0;
}
