CC = g++
ccflags = -c -std=c++14 -O2

all: test

test: build_test
	./test

build_test: test.o allocator.o
	$(CC) test.o allocator.o -o test

test.o: test.cpp allocator.h
	$(CC) $(ccflags) test.cpp

allocator.o: allocator.cpp allocator.h
	$(CC) $(ccflags) allocator.cpp

run:
	@echo "В этом ДЗ цель run будет лишней"

clean: 
	rm -f *.o test