CC = mpicc
CFLAGS = --std=c99 -Wall

%.out: %.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f *.o *.out logo2.pgm
