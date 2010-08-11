#!/usr/bin/env python2.6

import sys
import pylab

data = zip( *[ i.split() for i in file( sys.argv[1] ).read().splitlines() ] )
pylab.plot( data[0], data[1], ".", data[0], data[2], "." ) 
pylab.show()
