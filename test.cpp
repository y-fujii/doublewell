
#include <algorithm>
#include <vector>
#include <valarray>
#include <cmath>
#include <iostream>
#include <fenv.h>
#include "format.hpp"
#include "sampler.hpp"

using namespace std;


ostream& operator<<( ostream& out, valarray<double> const& x ) {
	out << "[";
	for( size_t i = 0; i < x.size() - 1; ++i ) {
		out << format( "% 03.3f,", x[i] );
	}
	out << format( "% 03.3f]", x[x.size() - 1] );

	return out;
}

inline double norm2( valarray<double> const& x ) {
	double s = 0.0;
	for( size_t i = 0; i < x.size(); ++i ) {
		s += x[i] * x[i];
	} 
	return s;
}

struct Gauss {
	Gauss( double s ): _isigma2( 1.0 / (s * s * 2.0) ) {}

	double operator()( valarray<double> const& __restrict__ x ) {
		return exp( -_isigma2 * norm2( x ) );
	}

	double const _isigma2;
};

template<int n>
struct Xn {
	double operator()( valarray<double> const& __restrict__ x ) {
		double m = 1.0;
		for( size_t i = 0; i < x.size(); ++i ) {
			m *= std::pow( x[i], n );
		}
		return m;
	}
};


void testGauss() {
	valarray<double> x( 6 );
	vector< valarray<double> > samples;

	generateSample( back_inserter( samples ), x, 1000000, 10, UniformMutator( 1.0 ), Gauss( 1.0 ) );

	double s1 = integrate<double>( samples.begin(), samples.end(), Xn<1>() );
	double s2 = integrate<double>( samples.begin(), samples.end(), Xn<2>() );
	double s3 = integrate<double>( samples.begin(), samples.end(), Xn<3>() );

	cout << s1 << "\n";
	cout << s2 << "\n";
	cout << s3 << "\n";
}

int main() {
	feenableexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW );
	testGauss();

	return 0;
}
