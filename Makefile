all:
	#g++ -pg -pedantic -Wall -Wextra test.cpp
	g++ -O3 -march=native -DNDEBUG -pedantic -Wall -Wextra test.cpp

test: all
	./a.out
