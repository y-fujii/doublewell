#pragma once

#include <cassert>
#include <cmath>

// XXX
#include <tr1/random>
std::tr1::mt19937 randGen;


template<class DstIter, class Vector, class Mutator, class Weight>
DstIter generateSample( DstIter dstIt, Vector& __restrict__ x, int n, int step, Mutator mutate, Weight weight ) {
	double wp = weight( x );

	for( int i = 0; i < n; ++i ) {
		for( int j = 0; j < step; ++j ) {
			int axis = randGen() % x.size(); // XXX
			double xp = x[axis];
			x[axis] = mutate( xp );

			double wn = weight( x );
			assert( wn >= 0.0 && wp >= 0.0 );
			if( wn * randGen.max() > wp * randGen() ) {
				wp = wn;
			}
			else {
				x[axis] = xp;
			}
		}
		*dstIt++ = x;
	}

	return dstIt;
}

template<class Result, class SrcIter, class Func>
Result integrate( SrcIter bgn, SrcIter end, Func f ) {
	Result x = 0.0;
	int n = 0;
	for( SrcIter it = bgn; it != end; ++it ) {
		x += f( *it );
		++n;
	}
	return x / n;
}

struct UniformMutator {
	UniformMutator( double dx ): _dx( dx ) {}

	double operator()( double x ) const {
		return x + _dx * (randGen() * (2.0 / randGen.max()) - 1.0);
	}

	double const _dx;
};

struct GaussMutator {
	GaussMutator( double s ): _sigma( s ) {}

	double operator()( double x ) const {
		double r0 = (randGen() + 1.0) * (1.0 / (randGen.max() + 1.0)); // (0.0, 1.0]
		double r1 = randGen() * (1.0 / (randGen.max() + 1.0)); // [0.0, 1.0)
		return _sigma * sqrt( -2.0 * log( r0 ) ) * cos( (2.0 * M_PI) * r1 ) + x;
	}

	double const _sigma;
};
