.PHONY = all compila clean

all: compila

compila: main.cpp
	rm -f generations.out
	g++ -o fight $^

clean: 
	rm -f generations.out