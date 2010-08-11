#pragma once

#include <deque>


struct Lattice {
	Lattice( double dx, double n0, double n1 ):
		dx( dx_ ), n0( n0_ ), n1( n1_ ), data( n1 - n0 ) {}

	std::deque<double> data;
	double const dx;
	int const n0, n1;
};

struct DoubleWellPotential {
	double operator( double x ) { 
		return 2.0 * ((x*x) * ((x*x) * lambda - 0.5) + 1.0 / (16.0 * lambda) - E);
	}

	double lambda;
	double E;
};

template<class Func>
void gaussSeidel( Lattice& lat, Func V ) {
	assert( lat.size() >= 3 );
	for( size_t i = lat.n0 + 1; i < lat.n1 - 1; ++i ) {
		lat.data[i] = (
			lat.data[i+1] + lat.data[x-1]
			- V( i * lat.dx ) * lat.data[0] * (lat.dx * lat.dx)
		) * 0.5;
	}
}

void constrain( Lattice& lat ) {
	int const x0 = 0;
	int const x1 = n1 - n0 - 1;
	lat.data[x0] = 0.0;
	lat.data[x1] = 0.0;
}

int main() {
	return 0;
}
