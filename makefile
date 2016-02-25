C=g++

flags=-Wall -Werror -ansi -pedantic

all: bin rshell

bin: bin
	mkdir bin;

rshell: bin
	$(C) src/main.cpp $(flags) -o bin/rshell

clean:
	rm -r bin

run:
	./bin/rshell
