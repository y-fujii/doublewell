#pragma once

#include <algorithm>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <cassert>

using namespace std;


double const snan = numeric_limits<double>::signaling_NaN();

template<class Func>
inline void euler2d( double& x, double& y, double& t, double dt, Func f ) {
	double dxdt, dydt;
	f( dxdt, dydt, x, y, t );
	x += dxdt * dt;
	y += dydt * dt;
	t += dt;
}

template<class Func>
inline void heun2d( double& x, double& y, double& t, double dt, Func f ) {
	double dxdt0, dydt0, dxdt1, dydt1;
	f( dxdt0, dydt0, x, y, t );
	f( dxdt1, dydt1, x + dxdt0 * dt, y + dydt0 * dt, t + dt );
	x += (dxdt0 + dxdt1) * (dt * 0.5);
	y += (dydt0 + dydt1) * (dt * 0.5);
	t += dt;
}

template<class Func>
double bisect( double s0, double s1, double eps, Func f ) {
	// precond {
		double const f0 = f( s0 );
		double const f1 = f( s1 );
		double x0, x1;
		if( f0 < 0.0 && 0.0 < f1 ) {
			x0 = s0;
			x1 = s1;
		}
		else if( f1 < 0.0 && 0.0 < f0 ) {
			x0 = s1;
			x1 = s0;
		}
		else {
			throw runtime_error( "bisect(): invalid args" );
		}
	// }
	
	while( abs( x1 - x0 ) > eps ) {
		double const mx = (x0 + x1) / 2.0;
		if( f( mx ) < 0.0 ) {
			x0 = mx;
		}
		else {
			x1 = mx;
		}
	}

	// postcond {
		if( x0 == s0 || x0 == s1 || x1 == s0 || x1 == s1 ) {
			throw runtime_error( "bisect(): unreliable result" );
		}
	// }

	return (x0 + x1) / 2.0;
}
