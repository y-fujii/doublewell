a.out: $(wildcard *.cpp *.hpp)
	g++44 -O3 -g -pedantic -Wall -Wextra -fdump-tree-optimized test.cpp
	#clang++ -O3 -march=native -DNDEBUG -pedantic -Wall -Wextra -Wno-mismatched-tags test.cpp

.PHONY: test
test: a.out
	./a.out
