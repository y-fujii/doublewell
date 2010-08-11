#include <algorithm>
#include <deque>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <fenv.h>
#include "numeric.hpp"
#include "format.hpp"

using namespace std;


struct DoubleWell {
	DoubleWell( double l ):
		lambda( l ) {}

	double operator()( double x ) const {
		return (x*x) * ((x*x) * lambda - 0.5) + 1.0 / (16.0 * lambda);
	}

	double const lambda;
};

struct Harmonic {
	double operator()( double x ) const {
		return x * x * 0.5;
	}
};

template<class Func>
struct Schrodinger {
	Schrodinger( Func v, double e ):
		 V( v ), E( e ) {}

	void operator()( double& d_phi, double& d_dphi, double phi, double dphi, double x ) const {
		d_dphi = 2.0 * (V( x ) - E) * phi;
		d_phi = dphi;
	}

	Func const V;
	double const E;
};


template<class Func>
struct TestEigenval {
	TestEigenval( Func v, double dx_, double phi0_, double dphi0_ ):
		V( v ), dx( dx_ ), phi0( phi0_ ), dphi0( dphi0_ ) {}

	double operator()( double E ) const {
		double x = 0.0;
		double phi = phi0;
		double dphi = dphi0;

		while( !isinf( phi ) ) {
			heun2d( phi, dphi, x, dx, Schrodinger<Func>( V, E ) );
		}

		return phi;
	}

	Func const V;
	double const dx, phi0, dphi0;
};

template<class Func>
void dumpWave( ostream& out, size_t n, size_t sub, double dx, double phi, double dphi, Func f ) {
	deque<double> data;

	for( size_t i = 0; i <= n * sub; ++i ) { // [0, n]
		// XXX
		if( i % sub == 0 ) {
			data.push_back( phi );
		}
		double x = i * dx;
		heun2d( phi, dphi, x, dx, f );
	}
	assert( data.size() == n + 1 );

	for( deque<double>::reverse_iterator it = data.rbegin(); it != data.rend() - 1; ++it ) {
		if( abs( *(it + 1) ) > abs( *it ) ) {
			break;
		}
		*it = 0.0;
	}

	deque<double> sorted( data );
	sort( sorted.begin(), sorted.end() );
	double sum = 0.0;
	for( deque<double>::iterator it = sorted.begin(); it != sorted.end(); ++it ) {
		sum += *it * *it;
	}

	for( size_t i = 0; i <= n; ++i ) {
		out << format( "%03.2f %08.7e\n", i * sub * dx, data[i] / sqrt( sum * 2.0 ) );
	}
}

int main() {
	//feenableexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW );
	feenableexcept( FE_DIVBYZERO | FE_INVALID | FE_UNDERFLOW );

#if 0 // even
	double const phi0 = 1.0;
	double const dphi0 = 0.0;
	double const dx = 1.0 / 8192.0;
	string prefix = "evn/";
#else // odd
	double const phi0 = 0.0;
	double const dphi0 = 1.0;
	double const dx = 1.0 / 16384.0;
	string prefix = "odd/";
#endif
	double const prec = 1e-8;
	double e0 = 0.0;
	double e1 = 1.0;

	for( size_t i = 1; i <= 100; ++i ) { // (a, b]
		double const lambda = i * 0.01;
		double const r1 = bisect( e0, e1, prec,
			TestEigenval<DoubleWell>( DoubleWell( lambda ), dx, phi0, dphi0 )
		);
		double const r2 = bisect( e0, e1, prec,
			TestEigenval<DoubleWell>( DoubleWell( lambda ), dx / 2.0, phi0, dphi0 )
		);
		if( abs( r2 - r1 ) > prec ) {
			throw exception();
		}

		cout << r2 << endl;
		ofstream ofs( (prefix + format( "%03.2f.txt", lambda )).c_str() );
		dumpWave( ofs, 200, 256, 10.0 / (200 * 256), phi0, dphi0,
			Schrodinger<DoubleWell>( DoubleWell( lambda ), r2 )
		);
		ofs.close();

		e0 = r2 - 0.25;
		e1 = r2 + 0.25;
	}

	return 0;
}
