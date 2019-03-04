CC = gcc
CFLAGS = -Wall
CLIBS =

all: clean generator.o receiver.o
	${CC} generator.o ${CLIBS} -o generator.out
	${CC} receiver.o ${CLIBS} -o receiver.out

generator.o: generator.c
	${CC} ${CFLAGS} -c generator.c -o generator.o

receiver.o: receiver.c
	${CC} ${CFLAGS} -c receiver.c -o receiver.o

clean:
	rm -f *.o *.out
