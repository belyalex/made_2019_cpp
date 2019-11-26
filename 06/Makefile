CC = g++
ccflags = -c -std=c++14 -O2

all: test

test: build_test
	./test

build_test: test.o BigInt.o
	$(CC) test.o BigInt.o -o test

BigInt.o: BigInt.cpp BigInt.h
	$(CC) $(ccflags) BigInt.cpp

test.o: test.cpp BigInt.h
	$(CC) $(ccflags) test.cpp

run:
	@echo "В этом ДЗ цель run будет лишней"

clean: 
	rm -f *.o test