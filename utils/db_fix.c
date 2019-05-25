/* db_fix.c
 * Standalone utility to fix byte swapping in
 * binary oven database files.
 *
 * Tom Trebisky 5-25-2019
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "oven.h"

struct bp_database {
	b_database      biparameter;
        p_database      parameter;
};

struct bp_database buf;
struct stat sbuf;

void
fixer ( char *file )
{
	int nw = sizeof ( buf );
	int nr;
	int ifd;

	// printf ( "Size of BP is %d bytes\n", nw );
	// Size of BP is 133548 bytes (correct)

	ifd = open ( file, O_RDONLY );

	(void) fstat ( ifd, &sbuf );
	if ( sbuf.st_size != nw ) {
	    fprintf ( stderr, "Wrong file size: %d bytes\n", sbuf.st_size );
	    exit ( 1 );
	}

	nr = read ( ifd, (void *) &buf, nw );
	if ( nr != nw ) {
	    fprintf ( stderr, "Bad file read: %d bytes\n", nr );
	    exit ( 1 );
	}
	(void) close ( ifd );

#ifdef notdef
	/* It would be nice to recognize what endianism the file
	 * is automatically, and it is probably possible to do so.
	 * A magic number would make it easy.
	 */
	printf ( "Z0 clock: %08x\n", buf.biparameter.zone[0].clock );
	printf ( "Timestamp: %08x\n", buf.parameter.misc.timestamp );
#endif

	exit ( 0 );

}

int
main ( int argc, char **argv )
{
	fixer ( "database" );
}

/* THE END */
