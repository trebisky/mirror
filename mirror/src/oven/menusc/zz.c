#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

zzung ()
{
	int	t = 0;

	if (!Geditp)
	    return (GOER_NOEDITP);
	time (&Pdb->misc.timestamp);
	uncache_parameter ();
	if (Geditb) {
	    uncache_biparameter ();
	    db_bwrite_oven ();
	}
	db_bpwrite_disk ();
	db_pwrite_oven ();
	Geditp = NO;
	Geditb = NO;
	t = GOER_EXECUTED;
	return (t);
}

zzflg ()
{
	int	t = 0;

	if (!Geditp)
	    return (GOER_NOEDITP);
	flcache_parameter ();
	if (Geditb)
	    flcache_biparameter ();
	Geditp = NO;
	Geditb = NO;
	t = GOER_EXECUTED;
	return (t);
}
