#pragma once

#include <stdint.h>


// "Xorshift RNGs", G. Marsaglia
// http://www.jstatsoft.org/v08/i14/paper
struct XorShift128 {
	typedef uint32_t result_type;

	XorShift128():
		_x( 123456789 ),
		_y( 362436069 ),
		_z( 521288629 ),
		_w(  88675123 ) {
	}

	uint32_t operator()() {
		uint32_t t = _x ^ (_x << 11);
		_x = _y;
		_y = _z;
		_z = _w;
		_w = (_w ^ (_w >> 19)) ^ (t ^ (t >> 8));
		return _w;
	}

	uint32_t min() const {
		return 0;
	}

	uint32_t max() const {
		return 0xffffffffu;
	}

	private:
		uint32_t _x, _y, _z, _w;
};

struct XorShift64 {
	typedef uint32_t result_type;

	XorShift64(): _x( 88172645463325252ul ) {}

	uint32_t operator()() {
		_x ^= _x << 13;
		_x ^= _x >>  7;
		_x ^= _x << 17;
		return _x /* & 0xffffffff */;
	}

	uint32_t min() const {
		return 0;
	}

	uint32_t max() const {
		return 0xffffffffu;
	}

	private:
		uint64_t _x;
};

struct Mwc {
	typedef uint32_t result_type;

	Mwc():
		_x( 123456789 ),
		_y( 362436069 ),
		_z(  77465321 ),
		_c(     13579 ) {
	}

	uint32_t operator()() {
		uint64_t t = 916905990ul * _x + _c;
		_x = _y;
		_y = _z;
		_c = t >> 32;
		_z = t /* & 0xffffffff */;
		return _z;
	}

	uint32_t min() const {
		return 0;
	}

	uint32_t max() const {
		return 0xffffffffu;
	}

	private:
		uint32_t _x, _y, _z, _c;
};
