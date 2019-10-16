CC=g++ 
ccflags=-std=c++14

expr="2 + 3 * 4 -2"

all: test run

build_calc: calc.o main.o
	$(CC) -o calc main.o calc.o

main.o: main.cpp calc.h
	$(CC) $(ccflags) -c main.cpp

calc.o: calc.cpp calc.h
	$(CC) $(ccflags) -c calc.cpp

run: build_calc
	./calc ${expr}

build_test: test.o calc.o
	$(CC) -o test test.o calc.o

test.o: test.cpp calc.h
	$(CC) $(ccflags) -c test.cpp

test: build_test
	./test

clean:
	rm -f *.o calc test
