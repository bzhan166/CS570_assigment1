

all: load print change query clean

load: load.c header.h
	gcc -Wall -o load load.c header.c
print: print.c header.h
	gcc -o print print.c header.c
change: change.c header.h
	gcc -Wall -o change change.c header.c
query: query.c header.h
	gcc -o query query.c header.c
clean: clean.c header.h
	gcc -Wall -o clean clean.c header.c

reset:
	rm -f clean load print query create  change  *~ core