CC = g++
ccflags = -c -std=c++17 -O2

all: test

test: build_test
	./test

build_test: test.o format.o
	$(CC) test.o format.o -o test

test.o: test.cpp format.h
	$(CC) $(ccflags) test.cpp

format.o: format.cpp format.h
	$(CC) $(ccflags) format.cpp

run:
	@echo "В этом ДЗ цель run будет лишней"

clean: 
	rm -f *.o test
