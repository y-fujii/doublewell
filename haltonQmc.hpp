#pragma once

#include <valarray>


#define ARR_SIZE( arr ) (sizeof( arr ) / sizeof( (arr)[0] ))

int const primes[] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
	73, 79, 83, 89, 97,
};

inline double vanDerCorput( int i, int const base ) {
	double h = 0.0;
	double f = 1.0;
	while( i > 0 ) {
		f *= (1.0 / base);
		h += (i % base) * f;
		i /= base;
	}

	return h;
}

inline void halton( std::valarray<double>& xs, int i ) {
	assert( xs.size() <= ARR_SIZE( primes ) );
	for( size_t j = 0; j < xs.size(); ++j ) {
		xs[j] = vanDerCorput( i, primes[j] );
	}
}

template<class Result, class Func>
Result integrateQMC( std::valarray<double> const& x0, std::valarray<double> const& x1, int n, Func f /*, SeqGen seqGen */ ) {
	assert( x0.size() == x1.size() );
	size_t dim = x0.size();

	Result dst = 0.0;
	std::valarray<double> vec( dim );
	for( int i = 0; i < n; ++i ) {
		halton( vec, i );
		for( size_t j = 0; j < dim; ++j ) {
			vec[j] = vec[j] * (x1[j] - x0[j]) + x0[j];
		}
		dst += f( vec );
	}

	double a = 1.0 / n;
	for( size_t i = 0; i < dim; ++i ) {
		a *= x1[i] - x0[i];
	}

	return a * dst;
}
