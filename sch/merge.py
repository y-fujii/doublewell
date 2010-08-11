#!/usr/bin/env python2.6

evs = file( "even.txt" ).read().splitlines()
ods = file( "odd.txt" ).read().splitlines()
dst = file( "eigenval.txt", "w" )

for (ev, od) in zip( evs, ods ):
	(_, v) = od.split( " " )
	dst.write( "%s %s\n" % (ev, v) )
