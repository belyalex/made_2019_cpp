CC = g++
ccflags = -c -std=c++17 -O2
lflags = -pthread

all: test

test: build_test
	./test

build_test: test.o
	$(CC) $(lflags) test.o -o test

test.o: test.cpp ThreadPool.h
	$(CC) $(ccflags) test.cpp

run:
	@echo "В этом ДЗ цель run будет лишней"

clean: 
	rm -f *.o test
