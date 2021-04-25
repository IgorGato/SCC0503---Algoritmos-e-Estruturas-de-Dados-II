

all: main.o warmup.o
	gcc -o main main.o warmup.o

main: main.c warmup.h
	gcc -c main.c

back: warmup.c warmup.h
	gcc -c warmup.c

run:
	./main

clean: rm *.o main