CC = gcc
CFLAGS = -Wall -std=c99
CLIBS =

all: clean socketlib.o star1.o star2.o star3.o ring1.o
	${CC} socketlib.o star1.o ${CLIBS} -o star1.out
	${CC} socketlib.o star2.o ${CLIBS} -o star2.out
	${CC} socketlib.o star3.o ${CLIBS} -o star3.out
	${CC} socketlib.o ring1.o ${CLIBS} -o ring1.out

socketlib.o: socketlib.c
	${CC} -o $@ -c $< ${CFLAGS}

star1.o: star1.c
	${CC} -o $@ -c $< ${CFLAGS}

star2.o: star2.c
	${CC} -o $@ -c $< ${CFLAGS}

star3.o: star3.c
	${CC} -o $@ -c $< ${CFLAGS}

ring1.o: ring1.c
	${CC} -o $@ -c $< ${CFLAGS}

clean:
	rm -f *.o *.out
