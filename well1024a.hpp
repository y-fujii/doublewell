#pragma once

#include <stdint.h>
#include <stdlib.h>


// "Improved Long-Period Generators Based on Linear Recurrences Modulo 2"
//     Francois Panneton and Pierre L'Ecuyer
//     http://www.iro.umontreal.ca/~lecuyer/myftp/papers/lfsr04.pdf

struct Well1024a {
	typedef uint32_t result_type;

	static unsigned const M1 =  3;
	static unsigned const M2 = 24;
	static unsigned const M3 = 10;

	Well1024a(): _idx( 0 ) {
		for( int i = 0; i < 32; ++i ) {
			_state[i] = random(); // XXX
		}
	}

	uint32_t operator()() {
		uint32_t z0 = state( 31 );
		uint32_t z1 = state( 0 ) ^ mat0pos( 8, state( M1 ) );
		uint32_t z2 = mat0neg( 19, state( M2 ) ) ^ mat0neg( 14, state( M3 ) );

		state(  0 ) = z1 ^ z2; 
		state( 31 ) = mat0neg( 11, z0 ) ^ mat0neg( 7, z1 ) ^ mat0neg( 13, z2 );

		_idx = (_idx + 31) & 0x1f;

		return state( 0 );
	}

	uint32_t min() const {
		return 0u;
	}

	uint32_t max() const {
		return 0xffffffffu;
	}

	private:
		uint32_t& state( unsigned i ) {
			return _state[(_idx + i) & 0x1f];
		}

		uint32_t mat0pos( uint32_t t, uint32_t v ) {
			return v ^ (v >> t);
		}

		uint32_t mat0neg( uint32_t t, uint32_t v ) {
			return v ^ (v << t);
		}

		unsigned _idx;
		uint32_t _state[32];
};
