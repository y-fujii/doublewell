data = [ float( i.split()[1] ) for i in open( "l0_04.txt" ).read().splitlines() ]
print data
print sum( i  * i for i in data )
