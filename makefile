.PHONY = all compila clean

all: compila

main: main.cpp
	g++ -c main.c -fsanitize=address -g

compila: main.cpp
	rm -f generazioni.out
	g++ main.cpp -o fight -fsanitize=address 

clean: 
	rm -f fight dati_casate.out generazioni.out