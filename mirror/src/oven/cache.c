#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"

/* cache_parameter - cache the parameter database.
 */
cache_parameter ()
{
	p_database	*pdb;
	char	*malloc ();

	pdb = (p_database *)malloc (sizeof(p_database));
	if (pdb == 0)
	    return (-1);

	memcpy ((char *)pdb, (char *)Pdb, sizeof(p_database));
	OPdb = Pdb;
	Pdb = pdb;

	return (0);
}

/* uncache_parameter - uncache the parameter database.
 */
uncache_parameter ()
{
	memcpy ((char *)OPdb, (char *)Pdb, sizeof(p_database));
	free ((char *)Pdb);
	Pdb = OPdb;
	OPdb = 0;
	return (0);
}

/* flcache_parameter - discard the parameter database cache.
 */
flcache_parameter ()
{
	free ((char *)Pdb);
	Pdb = OPdb;
	OPdb = 0;
	return (0);
}

/* cache_biparameter - cache the biparameter database.
 */
cache_biparameter ()
{
	b_database	*bdb;
	char	*malloc ();

	bdb = (b_database *)malloc (sizeof(b_database));
	if (bdb == 0)
	    return (-1);

	memcpy ((char *)bdb, (char *)Bdb, sizeof(b_database));
	OBdb = Bdb;
	Bdb = bdb;

	return (0);
}

/* uncache_biparameter - uncache the biparameter database.
 */
uncache_biparameter ()
{
	memcpy ((char *)OBdb, (char *)Bdb, sizeof(b_database));
	free ((char *)Bdb);
	Bdb = OBdb;
	OBdb = 0;
	return (0);
}

/* flcache_biparameter - discard the biparameter database cache.
 */
flcache_biparameter ()
{
	free ((char *)Bdb);
	Bdb = OBdb;
	OBdb = 0;
	return (0);
}
