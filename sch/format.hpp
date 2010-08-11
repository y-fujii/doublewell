#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>


string format( char const* fmt, ... ) {
	va_list va;
	va_start( va, fmt );
	int n = vsnprintf( NULL, 0, fmt, va );
	va_end( va );

	char* tmp = reinterpret_cast<char*>( alloca( n + 1 ) );
	va_start( va, fmt );
	vsnprintf( tmp, n + 1, fmt, va );
	va_end( va );

	return string( tmp );
}
