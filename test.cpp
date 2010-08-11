
#include <algorithm>
#include <vector>
#include <valarray>
#include <cmath>
#include <iostream>
#include <tr1/random>
#include <fenv.h>
#include "format.hpp"
#include "sampler.hpp"
#include "mcint.hpp"
#include "well1024a.hpp"
#include "haltonQmc.hpp"

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

	double operator()( valarray<double> const& __restrict__ x ) const {
		return exp( -_isigma2 * norm2( x ) );
	}

	double const _isigma2;
};

template<int n>
struct GaussN {
	GaussN( double s ): _isigma2( 1.0 / (s * s * 2.0) ) {}

	double operator()( valarray<double> const& __restrict__ x ) const {
		double m = 1.0;
		for( size_t i = 0; i < x.size(); ++i ) {
			m *= std::pow( x[i], n );
		}
		return m * exp( -_isigma2 * norm2( x ) );
	}

	double const _isigma2;
};

template<int n>
struct Xn {
	double operator()( valarray<double> const& __restrict__ x ) const {
		double m = 1.0;
		for( size_t i = 0; i < x.size(); ++i ) {
			m *= std::pow( x[i], n );
		}
		return m;
	}
};

int main() {
	//feenableexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW );
	feenableexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW );

	/*
	{
		valarray<double> x( 12 );
		vector< valarray<double> > samples;

		generateSample( back_inserter( samples ), x, 2000000, 16, UniformMutator( 1.0 ), Gauss( 1.0 ) );

		double s1 = integrate<double>( samples.begin(), samples.end(), Xn<1>() );
		double s2 = integrate<double>( samples.begin(), samples.end(), Xn<2>() );
		double s3 = integrate<double>( samples.begin(), samples.end(), Xn<3>() );

		cout << format( "%f %f %f\n", s1, s2, s3 );
	}

	cout.flush();

	*/

	{
		int const n = 10000000;
		valarray<double> x0( -4.0, 20 );
		valarray<double> x1( +4.0, 20 );
		double r0 = integrateMC<double>( x0, x1, n, GaussN<0>( 1.0 ), std::tr1::mt19937() );
		double r1 = integrateMC<double>( x0, x1, n, GaussN<1>( 1.0 ), std::tr1::mt19937() );
		double r2 = integrateMC<double>( x0, x1, n, GaussN<2>( 1.0 ), std::tr1::mt19937() );
		double r3 = integrateMC<double>( x0, x1, n, GaussN<3>( 1.0 ), std::tr1::mt19937() );
		cout << format( "%f %f %f\n", r1 / r0, r2 / r0, r3 / r0 );
	}

	cout.flush();

	{
		int const n = 1000000;
		valarray<double> x0( -4.0, 20 );
		valarray<double> x1( +4.0, 20 );
		double r0 = integrateQMC<double>( x0, x1, n, GaussN<0>( 1.0 ) );
		double r1 = integrateQMC<double>( x0, x1, n, GaussN<1>( 1.0 ) );
		double r2 = integrateQMC<double>( x0, x1, n, GaussN<2>( 1.0 ) );
		double r3 = integrateQMC<double>( x0, x1, n, GaussN<3>( 1.0 ) );
		cout << format( "%f %f %f\n", r1 / r0, r2 / r0, r3 / r0 );
	}

	return 0;
}
