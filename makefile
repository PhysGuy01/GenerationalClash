all: compila

compila: main.cpp
	g++ -o fight main.cpp

clean: 
	rm fight