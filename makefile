all: main

main:	main.o
		gcc -o main main.o -lm

main.o:	main.c main.h
		gcc -o main.o -c main.c -lm -Wall -O

clean:
		rm -f *.o core