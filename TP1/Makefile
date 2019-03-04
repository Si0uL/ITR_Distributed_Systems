CC = gcc
CFLAGS = -Wall -std=c99
CLIBS =

all: clean socketlib.o star1.o
	${CC} socketlib.o star1.o ${CLIBS} -o star1.out

socketlib.o: socketlib.c
	${CC} -o $@ -c $< ${CFLAGS}

star1.o: star1.c
	${CC} -o $@ -c $< ${CFLAGS}

clean:
	rm -f *.o *.out
