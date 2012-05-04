CCC =	g++ -Wall -ansi -pedantic


demo:	minixml.h minixml.cpp
	${CCC} minixml.cpp -o $@
