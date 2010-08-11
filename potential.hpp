
inline double sq( double x ) {
	return x * x;
}

struct Action {
	double operator()( valarray<double> const& vec ) {
		assert( vec.size() % 2 == 0 );
		assert( vec.size() > 0 );

		// kinetic term
		double K = 0.0;
		for( int i = 1; i < vec.size(); ++i ) {
			K += sq( vec[i] - vec[i-1] );
		}
		K += sq( vec[vec.size()/2] - vec[vec.size()/2-1] );
		K *= _m / -4.0;

		// potential
		double V = 0.0;
		for( int i = 0; i < vec.size(); ++i ) {
			V += (-1.0/2.0) * sq( vec[i] ) + _lmbd * sq( sq( vec[i] ) );
		}
		V *= -1.0 / 2.0;

		// dissipative term
		double D = 0.0;
		for( int di = 1; di <= vec.size() / 2; ++di ) {
			double Dn = 0.0;
			for( int i = di; i < vec.size(); ++i ) {
				Dn += sq( vec[i] - vec[i-di] );
			}
			// XXX: this may bottle-necked?
			D += Dn / pow( di, _p );
		}
		D *= _eta / (-2.0 * M_PI);

		return exp( K + V + D );
	}

	double _m;
	double _lmbd;
	double _p;
	double _eta;
};
