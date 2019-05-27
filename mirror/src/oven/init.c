#include "oven.h"
#include "menus.h"
#include "context.h"
#define	GLOBALS
#include "global.h"

extern	char	*calloc();

init_globals (noven, ncomp)
int	noven;
int	ncomp;
{
	GLOBAL	*gp;
	int	status;

	if (globalp)
	    return (-1);
	gp = (GLOBAL *)calloc (1, sizeof(GLOBAL));
	status = (int)gp;
	switch (status) {
	case 0:
	    return (status+10);
	    break;
	default:
	    status = 0;
	    break;
	}
	gp->noven = noven;
	gp->ncomp = ncomp;
	globalp = gp;
	return (status);
}

init_context ()
{
	CONTEXT	*cp;
	int	status;

	cp = (CONTEXT *)calloc (1, sizeof(CONTEXT));
	status = (int)cp;
	switch (status) {
	case 0:
	    return (status+20);
	    break;
	default:
	    status = 0;
	    break;
	}
	globalp->contextp = cp;
	return (status);
}

init_databases (noven, ncomp, readonly)
int	noven;
int	ncomp;
int	readonly;
{
	int	oven;
	int	status;

	for (oven = 0; oven < N_OVEN; oven++)
	    status = init_database (oven, ncomp, readonly);
}

init_database (noven, ncomp, readonly)
int	noven;
int	ncomp;
int	readonly;
{
	database	*db;
	int	status;
	char	*shmalloc ();

	if (!iamthepilot())
	    readonly = 1;
	db = (database *)shmalloc (sizeof(database), noven, ncomp, readonly);
	status = (int)db;
	switch (status) {
	case 0:
	    return (status+30);
	    break;
	default:
	    status = 0;
	    break;
	}
	Gdb = db;
	Bdb = &db->biparameter;
	Pdb = &db->parameter;
	Idb = &db->intermediate;
	Ddb = &db->data;
/*	globalp->obdb = Bdb;		*/
/*	globalp->opdb = Pdb;		*/
	globalp->readonly = readonly;
	globalp->gdbn[noven] = Gdb;
	globalp->pdbn[noven] = (char *)Pdb;
	globalp->bdbn[noven] = (char *)Bdb;
	return (status);
}

init_menus ()
{
	MENUS	*msp;
	MENUS	*menus();
	int	status;

	msp = menus();
	status = (int)msp;
	switch (status) {
	case 0:
	case 1:
	case 2:
	    return (status+40);
	    break;
	default:
	    status = 0;
	    break;
	}
	globalp->msp = msp;
	return (status);
}

free_globals ()
{
	free ((char *)globalp);
	globalp = (GLOBAL *)0;
}

free_context ()
{
	free ((char *)globalp->contextp);
	globalp->contextp = (CONTEXT *)0;
}

free_databases (noven, ncomp, remove)
int	noven;
int	ncomp;
int	remove;
{
	int	oven;

	for (oven = 0; oven < N_OVEN; oven++)
	    free_database (oven, ncomp, remove);
}

free_database (noven, ncomp, remove)
int	noven;
int	ncomp;
int	remove;
{
	database	*db;

	if (db = globalp->gdbn[noven]) {
	    shmfree ((char *)db, noven, ncomp, remove);
	    globalp->gdbn[noven] = (database *)0;
	    globalp->pdbn[noven] = (char *)0;
	    globalp->bdbn[noven] = (char *)0;
	}
}

free_menus ()
{
	free ((char *)globalp->msp);
	globalp->msp = (MENUS *)0;
}

iamthepilot ()
{
	char	*getenv ();
	char	*user = getenv ("USER");

#define	TEST_PILOT
#ifndef	TEST_PILOT
	return (user && (!strcmp (user, "pilot") || !strcmp (user, "pilot2")));
#else
	/*
	return (user && !strcmp (user, "skip"));
	return (user && !strcmp (user, "tom"));
	*/
	return (user && ( !strcmp (user, "pilot") ||
		!strcmp (user, "pilot2" ||
		!strcmp (user, "tom"
		) ) );
	// return 1;
#endif
}
