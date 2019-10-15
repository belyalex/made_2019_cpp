CC=g++ 
ccflags=-std=c++14

expr="2 + 3 * 4 -2"

all: calc test run

calc: calc.o
	$(CC) -o calc calc.o

calc.o: calc.cpp
	$(CC) $(ccflags) -c calc.cpp

run: calc
	./calc ${expr}

test: calc
	./calc test

clean:
	rm *.o calc
