CC = g++
ccflags = -c -std=c++14 -O2

all: test

test: build_test
	./test

build_test: test.o tokenizer.o
	$(CC) test.o tokenizer.o -o test

test.o: test.cpp tokenizer.h
	$(CC) $(ccflags) test.cpp

tokenizer.o: tokenizer.cpp tokenizer.h
	$(CC) $(ccflags) tokenizer.cpp

run:
	@echo "В этом ДЗ цель run будет лишней"

clean: 
	rm -f *.o test