#include "oven.h"

extern	database	*dbs;

extern	int	using_sysauxclk;
extern	int	holditasec;

/* isr.c - put all interrupt service routines here
 * and compile with -fsoft
 */
otick (db)
database *db;
{
	d_misc	*dmisc = &db->data.misc;
	i_misc	*imisc = &db->intermediate.misc;
	int	*msclock = &imisc->msclock;

	if (!using_sysauxclk)
	    usrClock ();
	if (holditasec)
	    return;
	*msclock += 1000/CLOCKRATE;
	*msclock %= PERIODC*N_MAP;
	imisc->pclock = *msclock/(PERIODC/MAX_POWER);
	imisc->cclock = *msclock/(PERIODC/N_PANEL)%N_PANEL;
	imisc->tclock = *msclock/PERIODT%TCN_MAXN;
	if (*msclock % 1000  ==   0) {
	    dmisc->iclock += 1;
	    dmisc->uclock += 1;
	    if (dmisc->uclock%3600 == imisc->ncomp*3600/N_COMP)
		semGive (imisc->ksem);
	}
	if (*msclock%(PERIODC/MAX_POWER)  ==   0)
	    semGive (imisc->psem);
	if (*msclock%PERIODT              ==  50)
	    semGive (imisc->tsem);
	if (*msclock%(PERIODC/N_PANEL)    == 300)
	    semGive (imisc->csem);
}

/* texcerror - report task restart on exception error
 */
texcerror ()
{
	e_misc	*emisc = &dbs->error.misc;

	emisc->texc++;
}
