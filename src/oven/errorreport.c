#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "errnum.h"
#include "errmes.h"

#define	NSEEN	( globalp->db->intermediate.misc.msclock )
#define	ELOG(count,errnum,addr)	if (count) elogger (db,fp,&count,errnum,addr);
#define	LEN_ELOGLINE	79

/* stalereport - do stale reporting
 */
stalereport ()
{
	stalejtmp (globalp->db);
	staleadc (globalp->db);
}

/* timeoutreport - do timeout reporting
 */
timeoutreport ()
{
	FILE	*fp;
	int	fd;
	int	count = 1;
	database *db = globalp->db;

	while ((fd = open ("errors.lok", O_RDWR | O_CREAT | O_EXCL)) < 0)
	    sleep (1);
	close (fd);
	fp = fopen ("errors.log", "a");

	ELOG (count, ERRTIME, 0)

	fclose (fp);
	unlink ("errors.lok");
}

/* errorreport - do error reporting
 */
errorreport ()
{
	FILE	*fp;
	int	fd;

	while ((fd = open ("errors.lok", O_RDWR | O_CREAT | O_EXCL)) < 0)
	    sleep (1);
	close (fd);
	fp = fopen ("errors.log", "a");

	ereport (globalp->db, fp);

	fclose (fp);
	unlink ("errors.lok");
}

/* ereport - do error reporting
 */
ereport (db, fp)
database	*db;
FILE		*fp;
{
	pripanelereport (db, fp);
	miscereport (db, fp);
	panelereport (db, fp);
	dcuereport (db, fp);
	zoneereport (db, fp);
	adcereport (db, fp);
}

/* miscereport - do misc error reporting
 */
miscereport (db, fp)
database	*db;
FILE		*fp;
{
	e_misc	*emisc = &db->error.misc;

	ELOG (emisc->texc, ERRTEXC, 0)
}

/* adcereport - do adc error reporting
 */
adcereport (db, fp)
database	*db;
FILE		*fp;
{
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    e_adc	*eadc = db->error.adc  + panel/N_COMP;
	    ea_panel	*eapanel = eadc->panel + panel%N_COMP;
	    ea_fase	*eafase = eapanel->fase;

	    if (!(panel%N_COMP)) {
		ELOG (eadc->voltage.lower, ERRHVL, pfe)
		ELOG (eadc->voltage.upper, ERRHVU, pfe)
		ELOG (eadc->voltage.indef, ERRHVI, pfe)
	    }

	    ELOG (eapanel->current.lower, ERRPCL, pfe)
	    ELOG (eapanel->current.upper, ERRPCU, pfe)
	    ELOG (eapanel->current.indef, ERRPCI, pfe)

	    for (fase = 0; fase < N_FASE; FASE__, eafase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		ELOG (eafase->current.lower, ERRFCL, pfe)
		ELOG (eafase->current.upper, ERRFCU, pfe)
		ELOG (eafase->current.indef, ERRFCI, pfe)
		ELOG (eafase->voltage.lower, ERRFVL, pfe)
		ELOG (eafase->voltage.upper, ERRFVU, pfe)
		ELOG (eafase->voltage.indef, ERRFVI, pfe)
	    }
	}
}

/* dcuereport - do dcu error reporting
 */
dcuereport (db, fp)
database	*db;
FILE		*fp;
{
	DCL_DCU
	e_dcu	*edcu = db->error.dcu;

	for (dcu = 0; dcu < N_DCU; DCU__, edcu++) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_COUNTER
	    e_counter	*ecounter = edcu->counter;
	    DCL_SELECT
	    e_select	*eselect = edcu->select;

	    ELOG (edcu->ton, ERRTON, dntx)
	    ELOG (edcu->tof, ERRTOF, dntx)
	    ELOG (edcu->dst, ERRDST, dntx)

	    for (counter = 0; counter < N_COUNTER; COUNTER__, ecounter++) {

		DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC)*N_TC;
		DCL_TIC
		e_tic	*etic = ecounter->tic;

		for (tic = 0; tic < N_TIC; TIC__, etic++) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC;
		    p_tc	*ptc = ptic->tc;
		    i_tc	*itc = itic->tc;
		    d_tc	*dtc = dtic->tc;
		    int		tc;
		    e_tc	*etc = etic->tc;

		    ELOG (etic->jtl, ERRJTL, dntx)
		    ELOG (etic->ijt, ERRIJT, dntx)

		    for (tc = 0; tc < N_TC; ptc++, itc++, dtc++, tc++, etc++) {

			DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC+tc;

			if (tc >= N_TTMP)
			    ;
			ELOG (etc->fpe, ERRFPE, dntx)
			ELOG (etc->back, ERRBACK, dntx)
			ELOG (etc->shrt, ERRSHRT, dntx)
			ELOG (etc->melt, ERRMELT, dntx)
			ELOG (etc->open, ERROPEN, dntx)
		    }
		}
	    }

	    for (select = 0; select < N_SELECT; SELECT__, eselect++) {

		DSB	dsb = (dcu*N_SELECT+select)*N_BUSS;
		DCL_BUSS
		e_buss	*ebuss = eselect->buss;

		for (buss = 0; buss < N_BUSS; BUSS__, ebuss++) {

		    DSB	dsb = (dcu*N_SELECT+select)*N_BUSS+buss;

		    ELOG (ebuss->mer, ERRMER, dsb)
		}
	    }
	}
}

/* pripanelereport - do priority panel error reporting
 */
pripanelereport (db, fp)
database	*db;
FILE		*fp;
{
	DCL_PANEL
	e_panel	*epanel = db->error.panel;

	for (panel = 0; panel < N_PANEL; PANEL__, epanel++) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    e_fase	*efase = epanel->fase;

	    for (fase = 0; fase < N_FASE; FASE__, efase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT
		e_element	*eelement = efase->element;

		for (element = 0; element < N_ELEMENT; ELEMENT__, eelement++) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    ELOG (eelement->hon, ERRHON, pfe)
		}
	    }
	}
}

/* panelereport - do panel error reporting
 */
panelereport (db, fp)
database	*db;
FILE		*fp;
{
	DCL_PANEL
	e_panel	*epanel = db->error.panel;

	for (panel = 0; panel < N_PANEL; PANEL__, epanel++) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    e_fase	*efase = epanel->fase;

	    ELOG (epanel->ppe, ERRPPE, pfe)
	    ELOG (epanel->pkp, ERRPKP, pfe)

	    for (fase = 0; fase < N_FASE; FASE__, efase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT
		e_element	*eelement = efase->element;

		for (element = 0; element < N_ELEMENT; ELEMENT__, eelement++) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    ELOG (eelement->con, ERRCON, pfe)
		    ELOG (eelement->von, ERRVON, pfe)
		    ELOG (eelement->hof, ERRHOF, pfe)
		    ELOG (eelement->vof, ERRVOF, pfe)
		    ELOG (eelement->cof, ERRCOF, pfe)
		    ELOG (eelement->her, ERRHER, pfe)
		    ELOG (eelement->hsc, ERRHSC, pfe)
		    ELOG (eelement->rof, ERRROF, pfe)
		    ELOG (eelement->ron, ERRRON, pfe)
		    ELOG (eelement->ooii, ERROOII, pfe)
		    ELOG (eelement->oioi, ERROIOI, pfe)
		    ELOG (eelement->oiio, ERROIIO, pfe)
		    ELOG (eelement->iooi, ERRIOOI, pfe)
		    ELOG (eelement->ioio, ERRIOIO, pfe)
		    ELOG (eelement->iioo, ERRIIOO, pfe)
		}
	    }
	}
}

/* zoneereport - do zone error reporting
 */
zoneereport (db, fp)
database	*db;
FILE		*fp;
{
	DCL_ZONE
	e_zone	*ezone = db->error.zone;

	for (zone = 0; zone < N_ZONE; ZONE__, ezone++) {
	    ELOG (ezone->zsc, ERRZSC, zone)
	}
}

/* elogger - error logger
 */
elogger (db, fp, count, errnum, addr)
database	*db;
FILE		*fp;
int	*count;
int	errnum;
int	addr;
{
	char	saddr[6];
	char	sloc[12];
	char	string[80];

	     if (!strcmp (errmenu[errnum], "zo")) przone (saddr, addr);
	else if (!strcmp (errmenu[errnum], "he")) prpfe  (saddr, addr);
	else if (!strcmp (errmenu[errnum], "pp")) prp00  (saddr, addr);
	else if (!strcmp (errmenu[errnum], "tc")) prdntx (saddr, addr);
	else if (!strcmp (errmenu[errnum], "sb")) prdsb  (saddr, addr);
	else if (!strcmp (errmenu[errnum], "dc")) prd000 (saddr, addr);
	else if (!strcmp (errmenu[errnum], "ti")) prdnt0 (saddr, addr);
	else if (!strcmp (errmenu[errnum], "al")) prpfe  (saddr, addr);
	else saddr[0] = 0;

	if (!strcmp (errmenu[errnum], "he"))
	    prloc (sloc, &phe_pfe (&db->parameter, addr)->loc);
	else if (!strcmp (errmenu[errnum], "tc"))
	    if (addr%N_TC < N_TTMP)
		prloc (sloc, &ptc_dntx(&db->parameter, addr)->loc);
	    else
		strcpy (sloc, saddr);
	else
	    strcpy (sloc, saddr);

	timestamp (string);
#ifdef	VERBOSE_ERRORS
	fprintf (fp, "%s %s\n", string, errmes[errnum]);
	fprintf (fp, "oven%dv%d %3d error count = %6d @ %-5s<%s %s>\n",
	  globalp->noven,
	  globalp->ncomp,
	  errnum,
	  *count,
	  saddr,
	  errmenu[errnum],
	  sloc);
#else
	/* should be LEN_ELOGLINE long
	 */
	fprintf (fp,
	  "%16.16s %1.1dv%1.1d#%6.6d@%-4.4s<%2.2s %-11.11s>%2.2d %-27.27s\n",
	  string+4,
	  globalp->noven,
	  globalp->ncomp,
	  *count,
	  saddr,
	  errmenu[errnum],
	  sloc,
	  errnum,
	  errmes[errnum]);
#endif
	*count = 0;
}

/* timestamp - write out a timestamp
 */
timestamp (string)
char	*string;
{
	long	now;
	char	*ctime();
	char	*strchr ();

	time (&now);
	strcpy (string, ctime (&now));
	if (strchr (string, '\n'))
	    *(strchr (string, '\n')) = 0;
	string[strlen(string)-5] = 0;
}

/* nelogline - return the number of lines in the error log
 */
nelogline ()
{
	struct	stat st;
	int	nlines;

	nlines = stat ("errors.log", &st) ? 0 : st.st_size/LEN_ELOGLINE;
	if (NSEEN > nlines)
	    if (!globalp->readonly)
		NSEEN = 0;
	return (nlines);
}

/* eunseen - check for unseen errors
 */
eunseen ()
{
	return (nelogline () > NSEEN);
}

/* elogline - return the n'th line of the error log
 */
elogline (n, s)
int	n;
char	*s;
{
	int	fd;
	off_t	offset = n * LEN_ELOGLINE;
	int	nlines = nelogline ();

	if ((n < nlines) && ((fd = open ("errors.log", O_RDONLY)) >= 0)) {
	    lseek (fd, offset, L_SET);
	    if (read (fd, s, LEN_ELOGLINE) == LEN_ELOGLINE) {
		s[LEN_ELOGLINE-1] = 0;
		if (n == nlines-1)
		    if (!globalp->readonly)
			NSEEN = MAX (n+1, NSEEN);
	    } else {
		*s = 0;
	    }
	    close (fd);
	}
}

/* eloggoto - go to a menu based on where we are in the error log
 */
eloggoto (s)
char	*s;
{
	char	mm[3];
	char	sa[5];
	char	*cp;
	char	*strchr();
	int	t;
	int	zone;
	DNTX	dntx;
	DSB	dsb;
	PFE	pfe;

	if ((cp = strchr (s, '<')) == 0)
	    return (GOER_NOWHERE);
	cp++;
	mm[0] = *cp++;
	mm[1] = *cp++;
	mm[2] = 0;

	if ((cp = strchr (s, '@')) == 0)
	    return (GOER_NOWHERE);
	cp++;
	sa[0] = *cp++;
	sa[1] = *cp++;
	sa[2] = *cp++;
	sa[3] = *cp++;
	sa[4] = 0;

	     if (!strcmp (mm, "zo")) t = sczone (sa, &zone);
	else if (!strcmp (mm, "he")) t = scpfe  (sa, &pfe);
	else if (!strcmp (mm, "pp")) t = scpfe  (sa, &pfe);
	else if (!strcmp (mm, "tc")) t = scdntx (sa, &dntx);
	else if (!strcmp (mm, "sb")) t = scdsb  (sa, &dsb);
	else if (!strcmp (mm, "dc")) t = scdntx (sa, &dntx);
	else if (!strcmp (mm, "ti")) t = scdntx (sa, &dntx);
	else if (!strcmp (mm, "al")) t = scpfe  (sa, &pfe);
	else if (!strcmp (mm, "ms")) t = SCER_OK;
	else return (GOER_NOWHERE);
	if (t)
	    return (GOER_NOWHERE);

	t = menugoto (mm);
	if (t)
	    return (GOER_NOWHERE);

	     if (!strcmp (mm, "zo")) Gzone = zone;
	else if (!strcmp (mm, "he")) Gpfe  = pfe;
	else if (!strcmp (mm, "pp")) Gpfe  = pfe;
	else if (!strcmp (mm, "tc")) Gdntx = dntx;
	else if (!strcmp (mm, "sb")) Gdsb  = dsb;
	else if (!strcmp (mm, "dc")) Gdntx = dntx;
	else if (!strcmp (mm, "ti")) Gdntx = dntx;
	else if (!strcmp (mm, "al")) Gpfe  = pfe;
	else if (!strcmp (mm, "ms")) ;
	else return (GOER_NOWHERE);

	return (t);
}
