CXX = g++ -g -pedantic -Wall -Wextra -Wno-comment -O3

a.out: main.cpp numeric.hpp format.hpp
	$(CXX) main.cpp

run: a.out
	./a.out

even: a.out
	rm -rf even even.txt
	mkdir even
	cd even && ../a.out > ../even.txt

odd: a.out
	rm -rf odd odd.txt
	mkdir odd
	cd odd && ../a.out > ../odd.txt

plot: a.out
	./a.out > plot.txt
	gnuplot -e 'plot "plot.txt"' -

zeroplot: a.out
	./a.out > plot.txt
	gnuplot -e 'set yrange [0:]; plot "plot.txt"' -

clean:
	rm -rf a.out plot.txt odd odd.txt even even.txt
