#include "oxnames.h"

HOVEN (noven, ncomp, readonly, remove, period, offset)
int	*noven;
int	*ncomp;
int	*readonly;
int	*remove;
int	*period;
int	*offset;
{
	int	status;

	status = init_globals (*noven, *ncomp);
	if (status == -1) {
	    free_database (*noven, *ncomp, 0);
	    free_context ();
	    free_globals ();
	    status = init_globals (*noven, *ncomp);
	}
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	status = init_database (*noven, *ncomp, *readonly);
	if (status)
	    return (status);
	status = init_menus ();
	if (status)
	    return (status);
	status = do_menus (*period, *offset);
	free_menus ();
	free_database (*noven, *ncomp, *remove);
	free_context ();
	free_globals ();
	if (status)
	    return (status);
	return (status);
}

DOVEN (noven, ncomp, logdata, period, offset)
int	*noven;
int	*ncomp;
int	*logdata;
int	*period;
int	*offset;
{
	int	status;
	int	readonly = 0;
	int	remove	= 0;

	status = init_globals (*noven, *ncomp);
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	status = init_database (*noven, *ncomp, readonly);
	if (status)
	    return (status);
	status = do_ovend (*logdata, *period, *offset);
	free_database (*noven, *ncomp, remove);
	free_context ();
	free_globals ();
	if (status)
	    return (status);
	return (status);
}

EOVEN (noven, ncomp, period, offset)
int	*noven;
int	*ncomp;
int	*period;
int	*offset;
{
	int	status;
	int	readonly = 0;
	int	remove	= 0;

	status = init_globals (*noven, *ncomp);
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	status = init_database (*noven, *ncomp, readonly);
	if (status)
	    return (status);
	status = do_ovene (*period, *offset);
	free_database (*noven, *ncomp, remove);
	free_context ();
	free_globals ();
	if (status)
	    return (status);
	return (status);
}

GOVEN (noven, ncomp, period, offset)
int	*noven;
int	*ncomp;
int	*period;
int	*offset;
{
	int	status;
	int	readonly = 0;
	int	remove	= 0;

	status = init_globals (*noven, *ncomp);
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	status = init_database (*noven, *ncomp, readonly);
	if (status)
	    return (status);
	status = do_oveng (*period, *offset);
	free_database (*noven, *ncomp, remove);
	free_context ();
	free_globals ();
	if (status)
	    return (status);
	return (status);
}

WOVEN (noven, ncomp, period, offset)
int	*noven;
int	*ncomp;
int	*period;
int	*offset;
{
	int	status;
	int	readonly = 0;
	int	remove	= 0;

	status = do_ovenw (*period, *offset);
	return (status);
}

POVEN (noven, ncomp)
int	*noven;
int	*ncomp;
{
	int	status;
	int	readonly = 1;
	int	remove	= 0;

	status = init_globals (*noven, *ncomp);
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	init_databases (*noven, *ncomp, readonly);
	status = do_ovenp (*noven, *ncomp);
	free_databases (*noven, *ncomp, remove);
	free_context ();
	free_globals ();
	if (status)
	    return (status);
	return (status);
}

BOVEN (noven, ncomp)
int	*noven;
int	*ncomp;
{
	int	status;
	int	readonly = 1;
	int	remove	= 0;

	status = init_globals (*noven, *ncomp);
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	init_databases (*noven, *ncomp, readonly);
	status = do_ovenb (*noven, *ncomp);
	free_databases (*noven, *ncomp, remove);
	free_context ();
	free_globals ();
	if (status)
	    return (status);
	return (status);
}

OOVEN (noven, ncomp)
int	*noven;
int	*ncomp;
{
	int	status;
	int	readonly = 1;
	int	remove	= 0;

	status = init_globals (*noven, *ncomp);
	if (status == -1)
	    return (0);		/* assume same process uses same noven */
	if (status)
	    return (status);
	status = init_context ();
	if (status)
	    return (status);
	status = init_database (*noven, *ncomp, readonly);
	if (status)
	    return (status);
	return (status);
}

do_ovend (logdata, period, offset)
int	logdata;
int	period;
int	offset;
{
	long	now;
	int	status;

	while (1) {
	    if (period) {
		time (&now);
		sleep (period - now%period + offset);
	    }
	    status = db_dread_oven ();
	    stalereport ();
	    if (status && logdata)
		datalogger ();
	}
}

do_ovene (period, offset)
int	period;
int	offset;
{
	long	now;

	while (1) {
	    if (period) {
		time (&now);
		sleep (period - now%period + offset);
	    }
	    if (db_eread_oven ())
		errorreport ();
	}
}

do_oveng (period, offset)
int	period;
int	offset;
{
	long	now;
	int	gong;
	int	n;

	while (1) {
	    if (period) {
		time (&now);
		sleep (period - now%period + offset);
	    }
	    gong = db_gread_oven ();
	    gong = (gong+7)/8;			/* sunview */
	    for (n = 0; n < gong; n++) {
		sleep (2);			/* sunview */
		ring ();
	    }
	}
}

do_ovenw (period, offset)
int	period;
int	offset;
{
	long	now;

	if (period) {
	    time (&now);
	    sleep (period - now%period + offset);
	}
	return (0);
}
