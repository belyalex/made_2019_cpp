CC = g++
ccflags = -c -std=c++14 -O2

all: test

test: build_test
	./test

build_test: test.o matrix.o
	$(CC) test.o matrix.o -o test

test.o: test.cpp matrix.h
	$(CC) $(ccflags) test.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) $(ccflags) matrix.cpp

run:
	@echo "В этом ДЗ цель run будет лишней"

clean: 
	rm -f *.o test