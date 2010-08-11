#pragma once

#include <valarray>
#include <cmath>


template<class Result, class Func, class RandGen>
Result integrateMC( std::valarray<double> const& x0, std::valarray<double> const& x1, int n, Func f, RandGen randGen ) {
	assert( x0.size() == x1.size() );
	size_t dim = x0.size();

	Result dst = 0.0;
	std::valarray<double> vec( dim );
	for( int i = 0; i < n; ++i ) {
		for( size_t j = 0; j < dim; ++j ) {
			vec[j] = (x1[j] - x0[j]) * (1.0 / randGen.max()) * randGen() + x0[j];
		}
		dst += f( vec );
	}

	double a = 1.0 / n;
	for( size_t i = 0; i < dim; ++i ) {
		a *= x1[i] - x0[i];
	}

	return a * dst;
}
