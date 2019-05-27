#define	import_libc
#define	import_stdio
#define	import_spp
#include <iraf.h>
#include <time.h>
#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "oxnames.h"
#include "mcodes.h"

#define	XINTCHAR	'\003'

#define	DOER_OK		0
#define	DOER_REPAINT	1
#define	DOER_ERROR	(-1)

do_menus (period, offset)
int	period;
int	offset;
{
	int	tty_fd = fileno (stdout);
	int	tty = c_ttyodes ("terminal");
	MENU	*mp;
	ITEM	*ip;
	int	iitems, itime;
	int	mline, top_mline, bot_mline;	/* menu   line numbers	*/
	int	nline, top_nline, bot_nline;	/* screen line numbers	*/
	int	ncols, nlines;			/* screen size		*/
	BOOL	repaint;
	BOOL	refresh;
	int	status = M_REPAINT;
	XINT	one = 1, two = 2;
	int	key;

	while (Gmenu >= 0) {
	    c_xttysize (&ncols, &nlines);
	    mp = Menus[Gmenu];
	    repaint = status & M_REPAINT;
	    refresh = status & M_REFRESH;

	    if (repaint)
		c_ttyclear (tty_fd, tty);

	    if (repaint || !strcmp (mp->id, "er")) {

		/* cache and calculate the menu line count
		 */
		mp->mlines = 0;
		for (iitems = 0; iitems < mp->nitems; iitems++) {
		    ip = mp->item[iitems];
		    ip->mtimes = (ip->ntimes) ? (*(ip->ntimes))() : 1;
		    mp->mlines += ip->mtimes;
		}
	    }

	    /* calculate the scrolling region
	     */
	    Gmline = MAX (0, MIN ((mp->mlines-1) - (nlines-2), Gmline));
	    top_nline = mp->item[0]->mtimes;
	    top_mline = Gmline + top_nline;
	    bot_mline = MIN (Gmline + nlines-2, mp->mlines-1);
	    bot_nline = bot_mline - top_mline + top_nline;
	    nline = top_nline;
	    mline = 0;
	    Gnline = MAX (top_mline, MIN (bot_mline, Gnline));

	    /* repaint or refresh the scrolling region
	     */
	    for (iitems = 0; iitems < mp->nitems; iitems++) {
		ip = mp->item[iitems];
		for (itime = 0; itime < ip->mtimes; mline++, itime++) {
		    if (mline < top_mline)
			continue;
		    if (mline > bot_mline)
			break;
		    doline (tty_fd, tty, ip, itime, nline, repaint, refresh);
		    nline++;
		    FLGKEY (&key);
		}
		if (mline > bot_mline)
		    break;
	    }

	    /* now do the ID line and the status line
	     */
	    nline = 0;
	    mline = 0;
	    ip = mp->item[0];
	    for (itime = 0; itime < ip->mtimes; mline++, itime++) {
		doline (tty_fd, tty, ip, itime, nline, repaint, refresh);
		nline++;
		FLGKEY (&key);
	    }
	    dostline (tty_fd, tty, nlines, top_mline, bot_mline, mp->mlines,
	      status & M_AUTO);
	    FLGKEY (&key);

	    /* now get the cursor mode key
	     */
	    THGKEY (&two);
	    status = domkey (tty_fd,tty,nlines,mp->mlines,top_mline,bot_mline
	        ,ncols, period, offset, status);
	    THGKEY (&one);
	}
	THGKEY (&one);
	c_ttygoto (tty_fd, tty, 1, nlines);
	c_ttyclearln (tty_fd, tty);
	c_ttycdes (tty);
	return (0);
}

/* nmline - return the number of visible menu lines
 */
nmline ()
{
	int	ncols, nlines;			/* screen size		*/

	c_xttysize (&ncols, &nlines);
	return (nlines-2);
}

/* doline - do a line out of the menu
 */
doline (tty_fd, tty, ip, itime, nline, repaint, refresh)
int	tty_fd;
int	tty;
ITEM	*ip;
int	itime;
int	nline;
BOOL	repaint;
BOOL	refresh;
{
	int	map_cc = 0;
	int	n;
	char	string[256];

	if ((repaint) && ip->text) {
	    c_ttygoto (tty_fd, tty, ip->text_start+1, nline+1);
	    c_ttyputline (tty_fd, tty, ip->text, map_cc);
	}
	if ((repaint || refresh) && ip->ofunc) {
	    string[0] = 0;
	    if (ip->ntimes)
		(*(ip->ofunc))(itime, string);
	    else
		(*(ip->ofunc))(string);
#ifdef	OLD_CLEARLINE
	    for (n = strlen (string); n < ip->func_end - ip->func_start+1; n++)
		string[n] = ' ';
	    string[n] = 0;
	    c_ttygoto (tty_fd, tty, ip->func_start+1, nline+1);
	    c_ttyputline (tty_fd, tty, string, map_cc);
#else
	    c_ttygoto (tty_fd, tty, ip->func_start+1, nline+1);
	    c_ttyputline (tty_fd, tty, string, map_cc);
	    c_ttyctrl (tty_fd, tty, "ce", 1);
#endif
	}
}

/* dostline - do status line: print %done info
 */
dostline (tty_fd, tty, nlines, top, bot, total, autom)
int	tty_fd;
int	tty;
int	nlines;
int	top;
int	bot;
int	total;
int	autom;
{
	char	string[80];

	sprintf (string, "%c%c%c lines %d to %d out of %d",
	  OPdb ? (OBdb ? 'K' : 'P') : ' ',
	  eunseen() ? 'E' : ' ',
	  autom ? 'A' : ' ',
	  top+1, bot+1, total);
	statusline (tty_fd, tty, nlines, string);
	dopcur (tty_fd, tty);
}

/* statusline - write the status line
 */
statusline (tty_fd, tty, nlines, string)
int	tty_fd;
int	tty;
int	nlines;
char	*string;
{
	int	map_cc = 0;

	c_ttygoto (tty_fd, tty, 1, nlines);
	c_ttyclearln (tty_fd, tty);
	c_ttyputline (tty_fd, tty, string, map_cc);
}

/* ring -- ring the bell
 */
ring ()
{
	int	tty_fd = fileno (stdout);
	int	tty = c_ttyodes ("terminal");
	int	map_cc = 0;
	int	key;

	c_ttyputline (tty_fd, tty, "\007", map_cc);
	FLGKEY (&key);
}

/* domgoto - do menu goto
 */
domgoto (tty_fd, tty, nlines, eswitch)
int	tty_fd;
int	tty;
int	nlines;
int	eswitch;
{
	MENU	*mp = Menus[Gmenu];
	ITEM	*ip = mp->item[Gitem];
	int	itime = Gtime;
	int	status = GOER_OK;
	int	key;

	if (eswitch == 0 && ip->gfunc == 0) {
	    statusline (tty_fd, tty, nlines,
		"\007no where to go to");
	    return (DOER_ERROR);
	}
	if (push_context () != 0) {
	    statusline (tty_fd, tty, nlines,
		"\007could not push context");
	    return (DOER_ERROR);
	}
	c_ttyputline (tty_fd, tty, "Executing", 0);
	FLGKEY (&key);

	if (eswitch)
	    status = menugoto ("er");
	else if (ip->ntimes)
	    status = (*(ip->gfunc)) (itime);
	else
	    status = (*(ip->gfunc)) ();

	if (status != GOER_OK) {
	    pop_context ();
	    switch (status) {
	    case GOER_EXECUTED:
	        statusline (tty_fd, tty, nlines, "Executed");
		break;
	    case GOER_NOMENU:
	        statusline (tty_fd, tty, nlines, "\007goto error");
		break;
	    case GOER_NOWHERE:
	        statusline (tty_fd, tty, nlines, "\007No where to go to");
		break;
	    case GOER_INCONSISTENT:
	        statusline (tty_fd, tty, nlines,"\007inconsistent goto  error");
		break;
	    case GOER_NOEDITP:
	        statusline (tty_fd, tty, nlines,
		  "\007parameter editting not enabled");
		break;
	    default:
	        statusline (tty_fd, tty, nlines, "\007unknown goto  error");
		break;
	    }
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* domwritep - do menu writep
 */
domwritep (tty_fd, tty, nlines)
int	tty_fd;
int	tty;
int	nlines;
{
	int	status = GOER_OK;

	if (!Geditp) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter editting not enabled");
	    return (DOER_ERROR);
	}

	if (push_context () != 0) {
	    statusline (tty_fd, tty, nlines,
		"\007could not push context");
	    return (DOER_ERROR);
	}

	status = menugoto ("zz");

	if (status != GOER_OK) {
	    pop_context ();
	    switch (status) {
	    case GOER_EXECUTED:
	        statusline (tty_fd, tty, nlines, "Executed");
		break;
	    case GOER_NOMENU:
	        statusline (tty_fd, tty, nlines, "\007goto error");
		break;
	    case GOER_NOWHERE:
	        statusline (tty_fd, tty, nlines, "\007No where to go to");
		break;
	    case GOER_INCONSISTENT:
	        statusline (tty_fd, tty, nlines,"\007inconsistent goto  error");
		break;
	    case GOER_NOEDITP:
	        statusline (tty_fd, tty, nlines,
		  "\007parameter editting not enabled");
		break;
	    default:
	        statusline (tty_fd, tty, nlines, "\007unknown goto  error");
		break;
	    }
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* domeditp - do menu editp
 */
domeditp (tty_fd, tty, nlines)
int	tty_fd;
int	tty;
int	nlines;
{
	int	status = GOER_OK;

	if (globalp->readonly) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter database is readonly");
	    return (DOER_ERROR);
	}
	if (OPdb) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter database is already cached");
	    return (DOER_ERROR);
	}
	if (Geditp) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter database cacheing is not necessary");
	    return (DOER_ERROR);
	}
	if (cache_parameter ()) {
	    statusline (tty_fd, tty, nlines,
		"\007could not cache parameters");
	    return (DOER_ERROR);
	}
	Geditp = YES;
	return (DOER_OK);
}

/* domeditb - do menu editb
 */
domeditb (tty_fd, tty, nlines)
int	tty_fd;
int	tty;
int	nlines;
{
	int	status = GOER_OK;


	if (!Geditp) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter editting not enabled");
	    return (DOER_ERROR);
	}
	if (globalp->readonly) {
	    statusline (tty_fd, tty, nlines,
		"\007clock parameter database is readonly");
	    return (DOER_ERROR);
	}
	if (OBdb) {
	    statusline (tty_fd, tty, nlines,
		"\007clock parameter database is already cached");
	    return (DOER_ERROR);
	}
	if (Geditb) {
	    statusline (tty_fd, tty, nlines,
		"\007clock parameter database cacheing is not necessary");
	    return (DOER_ERROR);
	}
	if (cache_biparameter ()) {
	    statusline (tty_fd, tty, nlines,
		"\007could not cache clock parameters");
	    return (DOER_ERROR);
	}
	Geditb = YES;
	return (DOER_OK);
}

/* menugoto - set the global menu number according to the id string
 */
menugoto (id)
char	*id;
{
	int	nmenu;

	for (nmenu = 0; nmenu < globalp->msp->nmenus; nmenu++)
	    if (strcmp (Menus[nmenu]->id, id) == 0) {
		Gmenu = nmenu;
		return (GOER_OK);
	    }
	return (GOER_NOMENU);
}

/* domreturn - do menu return
 */
domreturn (tty_fd, tty, nlines)
int	tty_fd;
int	tty;
int	nlines;
{
	if (pop_context () != 0) {
	    statusline (tty_fd, tty, nlines,
		"\007no where to return to");
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* domenter - do menu enter
 */
domenter (tty_fd, tty, nlines, ncols, iswitch)
int	tty_fd;
int	tty;
int	nlines;
int	ncols;
int	iswitch;
{
	MENU	*mp = Menus[Gmenu];
	ITEM	*ip = mp->item[Gitem];
	int	status = SCER_OK;
	int	map_cc = 0;
	int	n, nmax;
	char	string[256];
	char	clear_string[256];
	XINT	one = 1;
	XINT	two = 2;

	if (ip->ifunc == 0 && ip->tfunc == 0) {
	    statusline (tty_fd, tty, nlines,
		"\007cannot enter data");
	    return (DOER_ERROR);
	}

	if (!Geditp && strcmp (mp->id, "cs")) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter editting not enabled");
	    return (DOER_ERROR);
	}

	if (ip->ifunc) {
#ifdef	OLD_CLEARLINE
	    nmax = MIN(ip->func_end+1 - ip->func_start, ncols-1 - ip->func_end);
	    for (n = 0; n < nmax; n++)
		clear_string[n] = ' ';
	    clear_string[n] = 0;
	    c_ttyputline (tty_fd, tty, clear_string, map_cc);
#else
	    c_ttyctrl (tty_fd, tty, "ce", 1);
#endif
	    dopcur (tty_fd, tty);
	    string[0] = 0;
	    if (iswitch) {
		strcpy (string, "imcur");
	    } else {
		THGKEY (&one);
		clgstr ("input", string, sizeof(string));
		THGKEY (&two);
	    }
	    if (c_timcur (string, sizeof(string))) {
#ifdef	OLD_CLEARLINE
		for (n = 0; n < nmax; n++)
		    clear_string[n] = ' ';
		clear_string[n] = 0;
		c_ttyputline (tty_fd, tty, clear_string, map_cc);
#else
		c_ttyctrl (tty_fd, tty, "ce", 1);
#endif
		dopcur (tty_fd, tty);
		c_ttyputline (tty_fd, tty, string, map_cc);
		dopcur (tty_fd, tty);
	    }
	}

	if (ip->ifunc)
	    if (ip->ntimes)
		status = (*(ip->ifunc)) (Gtime, string);
	    else
		status = (*(ip->ifunc)) (string);
	else
	    if (ip->ntimes)
		status = (*(ip->tfunc)) (Gtime);
	    else
		status = (*(ip->tfunc)) ();

	if (status != SCER_OK) {
	    switch (status) {
	    case SCER_REPAINT:
	        return (DOER_REPAINT);
		break;
	    case SCER_SYNTAX:
	        statusline (tty_fd, tty, nlines, "\007Syntax error");
		break;
	    case SCER_RANGE:
	        statusline (tty_fd, tty, nlines, "\007Out of range");
		break;
	    case SCER_INCONSISTENT:
	        statusline (tty_fd, tty, nlines,"\007inconsistent enter error");
		break;
	    case SCER_NOEDITB:
	        statusline (tty_fd, tty, nlines,
		  "\007clock parameter editting not enabled");
		break;
	    default:
	        statusline (tty_fd, tty, nlines, "\007unknown enter error");
		break;
	    }
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* dopcur - position cursor
 */
dopcur (tty_fd, tty)
int	tty_fd;
int	tty;
{
	int	nline = Gnline - Gmline;
	int	ncol;
	MENU	*mp;
	ITEM	*ip;

	updateitem ();
	mp = Menus[Gmenu];
	ip = mp->item[Gitem];

	if (ip->ifunc)
	    ncol = ip->func_end+1;
	else if (ip->tfunc)
	    ncol = ip->func_end;
	else if (ip->ofunc)
	    ncol = ip->func_start;
	else if (ip->text)
	    ncol = ip->text_end+1;
	else
	    ncol = 0;

	c_ttygoto (tty_fd, tty, ncol+1, nline+1);
}

/* updateitem -- update the item pointers (Gitem & Gtime) from Gnline
 * (the cursor line number)
 */
updateitem ()
{
	MENU	*mp = Menus[Gmenu];
	ITEM	*ip;
	int	mline = 0;

	for (Gitem = 0; Gitem < mp->nitems; Gitem = Gitem+1) {
	    ip = mp->item[Gitem];
	    for (Gtime = 0; Gtime < ip->mtimes; mline++, Gtime = Gtime+1)
		if (mline == Gnline)
		    return;
	}
}

/* domkey - do menu key
 */
domkey (tty_fd, tty, nlines, mlines, top, bot, ncols, period, offset, oldstatus)
int	tty_fd;
int	tty;
int	nlines;
int	mlines;
int	top;
int	bot;
int	ncols;
int	period;
int	offset;
int	oldstatus;
{
	int	mcode;
	int	status = 0;
	int	gmline = Gmline;
	XINT	three = 3;

	do {
	    dopcur (tty_fd, tty);
	    mcode = mkey (oldstatus & M_AUTO, period, offset);
	    oldstatus &= ~M_AUTO;
	    switch (mcode) {
	    case SCR_U0:
		Gmline = 0;
		break;
	    case SCR_D0:
		Gmline = mlines;
		break;
	    case SCR_U1:
		Gmline -= nlines - 3;
		break;
	    case SCR_D1:
		Gmline += nlines - 3;
		break;
	    case SCR_U2:
		Gmline -= (nlines-2)/2;
		break;
	    case SCR_D2:
		Gmline += (nlines-2)/2;
		break;
	    case CUR_U:
		if (Gnline == top)
		    Gmline -= 1;
		else
		    mcode = UNKNOWN;
		Gnline -= 1;
		break;
	    case CUR_D:
		if (Gnline == bot)
		    Gmline += 1;
		else
		    mcode = UNKNOWN;
		Gnline += 1;
		break;
	    case REPAINT:
		status |= M_REPAINT;
		break;
	    case REFRESH:
		status |= M_REFRESH;
		break;
	    case AUTO:
		status |= M_REFRESH | M_AUTO;
		break;
	    case WRITEP:
		switch (domwritep (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REPAINT;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case CACHEP:
		switch (domeditp (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REFRESH;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case CACHEB:
		switch (domeditb (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REFRESH;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case RETURN:
		switch (domreturn (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REPAINT;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case ENTERG:
	    case IMCURG:
	    case ENTER:
	    case IMCUR:
		switch (domenter (tty_fd, tty, nlines, ncols,
			mcode == IMCUR || mcode == IMCURG)) {
		case DOER_REPAINT:
		    status |= M_REPAINT;
		    break;
		case DOER_OK:
		    status |= M_REFRESH;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		if (mcode == ENTERG || mcode == IMCURG)
		    mcode = GOTO;
		else
		    break;
	    case GOERR:
	    case GOTO:
		switch (domgoto (tty_fd, tty, nlines, mcode == GOERR)) {
		case DOER_OK:
		    status |= M_REPAINT;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case HELP:
		HPAGEH ();
		status |= M_REPAINT;
		break;
	    default:
		statusline (tty_fd, tty, nlines,
		    "\007unrecognized key - press `?' for help");
		break;
	    }
	} while (mcode == UNKNOWN || mcode == HELP);

	/* Normalize Gmline
	 */
	if ((status & M_REPAINT) == 0) {
	    Gmline = MAX (0, MIN ((mlines-1) - (nlines-2), Gmline));
	    if (Gmline != gmline)
	        status |= M_REPAINT;
	}
	if (status & M_REPAINT || status & M_REFRESH)
	    THGKEY (&three);
	return (status);
}

/* mkey - return a menu encoded key
 */
mkey (autom, period, offset)
int	autom;
int	period;
int	offset;
{
	int	key;
	int	mcode;

	if (autom) {
	    FLGKEY (&key);
	    if (!(key = automkey (period, offset)))
		return (AUTO);
	} else {
	    forever {
		HGKEY (&key, &period, &offset);
		if (key == '\033')
		    ;
		else if (key == '[')
		    ;
		else if (key == 'O')
		    ;
		else
		    break;
	    }
	}

	switch (key) {
	case 'd':
	    mcode = SCR_D2;
	    break;
	case 'u':
	    mcode = SCR_U2;
	    break;
	case 'f':
	case ' ':
	    mcode = SCR_D1;
	    break;
	case 'b':
	    mcode = SCR_U1;
	    break;
	case 'g':
	    mcode = SCR_D0;
	    break;
	case '.':
	    mcode = SCR_U0;
	    break;
	case '?':
	    mcode = HELP;
	    break;
	case 'B':
	case 'j':
	case '\012':
	case '\015':
	case '\032':
	case XEOF:
	case XINTCHAR:
	    mcode = CUR_D;
	    break;
	case 'A':
	case 'k':
	case '\027':
	    mcode = CUR_U;
	    break;
	case 'D':
	case 'n':
	case '\031':
	    mcode = GOTO;
	    break;
	case '!':
	    mcode = GOERR;
	    break;
	case 'C':
	case 'p':
	case '\030':
	    mcode = RETURN;
	    break;
	case 'l':
	    mcode = REPAINT;
	    break;
	case 'q':
	    mcode = REFRESH;
	    break;
	case 'a':
	    mcode = AUTO;
	    break;
	case 'e':
	    mcode = ENTER;
	    break;
	case 'i':
	    mcode = IMCUR;
	    break;
	case 'm':
	    mcode = ENTERG;
	    break;
	case 'o':
	    mcode = IMCURG;
	    break;
	case 'P':
	    mcode = CACHEP;
	    break;
	case 'K':
	    mcode = CACHEB;
	    break;
	case 'W':
	    mcode = WRITEP;
	    break;
	default:
	    mcode = UNKNOWN;
	    break;
	}
	return (mcode);
}

/* clgstr - CL get string parameter
 */
clgstr (param, outstr, maxch)
char	*param;
char	*outstr;
int	maxch;
{
	XCHAR	x_param[256];
	XCHAR	x_outstr[256];
	XINT	x_maxch = maxch;

	c_strupk (param, x_param, strlen (param)+1);
	HCLGST (x_param, x_outstr, &x_maxch);
	c_strpak (x_outstr, outstr, maxch);
}

/* timcur - Translate image cursor
 */
c_timcur (outstr, maxch)
char	*outstr;
int	maxch;
{
	XCHAR	x_outstr[256];
	XINT	x_maxch = maxch;
	XINT	status;

	c_strupk (outstr, x_outstr, strlen (outstr)+1);
	status = TIMCUR (x_outstr, &x_maxch);
	c_strpak (x_outstr, outstr, maxch);
	return (status == XYES);
}

DECISLEEP ()
{
	struct timespec rqt;

	rqt.tv_sec = 0;
	rqt.tv_nsec = 100000000;
	nanosleep (&rqt, NULL);
}
