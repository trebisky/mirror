#include "oven.h"

#define	PERROR	perror

#define	NOUSE_SYSAUXCLK
#ifdef	USE_SYSAUXCLK
#define	CLOCK_DISABLE		sysAuxClkDisable
#define	CLOCK_CONNECT		sysAuxClkConnect
#define	CLOCK_ENABLE		sysAuxClkEnable
#define	CLOCK_RATESET		sysAuxClkRateSet
int	using_sysauxclk = 1;
#else
#define	CLOCK_DISABLE		sysClkDisable
#define	CLOCK_CONNECT		sysClkConnect
#define	CLOCK_ENABLE		sysClkEnable
#define	CLOCK_RATESET		sysClkRateSet
int	using_sysauxclk = 0;
#endif

#ifdef	SLOW_START
#define	SLEEP(x)	sleep (x)
#define	PRINTF(x)	printf (x)
#else
#define	SLEEP(x)	noop(x)
#define	PRINTF(x)	noop(x)
#endif
noop(x) {}

#define	PERCENT	1		/* percent rate to adjust clock by */

#define	IPPORT_TIMESERVER	37
#define	PORTUC	IPPORT_TIMESERVER

#define F	( getfptaskoptions() )

typedef	int	*SEM_ID;

extern	char	sysBootHost[];

char	goodhost[64] = {""};
int	holditasec = 0;

database	*dbs;

static	char	*dcutask[] = {"tportdcu0", "tportdcu1", "tportdcu2",
				"tportdcu3", "tportdcu4", "tportdcu5"};
static	char	*adctask[] = {"tportadc0", "tportadc1", "tportadc2"};
static	int	sockdcu[N_DCU];
static	int	sockadc[N_COMP];
static	int	sockuc, sockrd, sockwp, sockrp, sockwb, sockrb, socker, sockgn;
static	int	sockrs;
static	long	bufrs[2];
static	char	*bufwp, *bufwb, *bufer, *bufgn;
static	char	*bufdcu[N_DCU];
static	char	*bufadc[N_COMP];

SEM_ID       semCreate () { return ((SEM_ID)semBCreate (0, 0)); }

tipdummy ()
{
	int	tip();

	return (tip());
}

rlogpri ()
{
	taskPrioritySet (taskNameToId ("tRlogOutTask"), 202);
	taskPrioritySet (taskNameToId ("tRlogInTask"), 202);
}

spypri ()
{
	taskPrioritySet (taskNameToId ("tSpyTask"), 171);
}

zapoven ()
{
	int	id;
	int	n;
	int	usrClock();

	if (using_sysauxclk)
	    CLOCK_DISABLE ();
	else
	    CLOCK_CONNECT (usrClock);
	if ((id = taskNameToId ("tportuc")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportrd")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportwp")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportrp")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportwb")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportrb")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tporter")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportgn")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportdcu")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tportadc")) != -1) taskDelete (id);
	for (n = 0; n < N_DCU; n++)
	    if ((id = taskNameToId (dcutask[n])) != -1) taskDelete (id);
	for (n = 0; n < N_COMP; n++)
	    if ((id = taskNameToId (adctask[n])) != -1) taskDelete (id);
	if ((id = taskNameToId ("trspeed")) != -1) taskDelete (id);
	if ((id = taskNameToId ("trwork")) != -1) taskDelete (id);
	if ((id = taskNameToId ("ttwork")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tcwork")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tpwork")) != -1) taskDelete (id);
	if ((id = taskNameToId ("tksync")) != -1) taskDelete (id);
	semDelete (dbs->intermediate.misc.dcusem);
	semDelete (dbs->intermediate.misc.adcsem);
	semDelete (dbs->intermediate.misc.tsem);
	semDelete (dbs->intermediate.misc.csem);
	semDelete (dbs->intermediate.misc.psem);
	semDelete (dbs->intermediate.misc.ksem);
	semDelete (dbs->intermediate.misc.rsem);
	free ((char *)dbs);
	close (sockuc);
	close (sockrd);
	close (sockwp);
	close (sockrp);
	close (sockwb);
	close (sockrb);
	close (socker);
	close (sockgn);
	close (sockrs);
	for (n = 0; n < N_DCU; n++)
	    close (sockdcu[n]);
	for (n = 0; n < N_COMP; n++)
	    close (sockadc[n]);
	free (bufwp);
	free (bufwb);
	free (bufer);
	free (bufgn);
	for (n = 0; n < N_DCU; n++)
	    free (bufdcu[n]);
	for (n = 0; n < N_COMP; n++)
	    free (bufadc[n]);
}

usermain ()
{
	int	oven();

	sp (oven);
}

oven ()
{
	char	*calloc();
	int	portwrite(), portread();
	int	eportwrite();
	int	uportread();
	int	dcubroadcast(), adcbroadcast();
	int	twork(), cwork(), pwork(), rwork(), ksync(), rspeed();
	int	csync();
	int	otick();
	int	unsuspender();
	SEM_ID	semCreate();
	int	n;

	sleep (5);
	loginUserAdd ("pilot", "SRzS9cyb9b");
	cd ("/u/pilot");
	PRINTF ("taskPrioritySet\n");
	taskPrioritySet (taskNameToId ("tNetTask"), 250);
	taskPrioritySet (taskNameToId ("tTelnetd"), 202);
	taskPrioritySet (taskNameToId ("tRlogind"), 202);
	taskPrioritySet (taskNameToId ("tShell"),   201);
	SLEEP (5);
	PRINTF ("sysClkRateSet\n");
	sysClkRateSet (100);

	/*hostAdd ("oven0v0", "128.196.32.10");
	hostAdd ("oven0v1", "128.196.32.11");
	hostAdd ("oven0v2", "128.196.32.12");
	hostAdd ("oven1v0", "128.196.32.13");
	hostAdd ("oven1v1", "128.196.32.14");
	hostAdd ("oven1v2", "128.196.32.15");
	hostAdd ("oven2v0", "128.196.32.16");
	hostAdd ("oven2v1", "128.196.32.17");
	hostAdd ("oven2v2", "128.196.32.18");
	hostAdd ("oven9v0", "128.196.176.20");
	hostAdd ("oven9v1", "128.196.176.21");
	hostAdd ("oven9v2", "128.196.176.22");
	hostAdd ("crater", "128.196.32.3");
	hostAdd ("dorado", "128.196.32.1");
	hostAdd ("astro", "128.196.176.1");*/
	
	hostAdd ("oven0v0", "192.168.1.40");
	hostAdd ("oven0v1", "192.168.1.41");
	hostAdd ("oven0v2", "192.168.1.42");
	hostAdd ("oven1v0", "192.168.1.50");
	hostAdd ("crater", "192.168.1.11");
	hostAdd ("dorado", "192.168.1.10");
	hostAdd ("astro", "128.196.176.1");


	if ((dbs = (database *)calloc (1, sizeof(database))) == 0) {
	    PERROR ("database calloc");
	    return (-1);
	}
	tinit (dbs);
	getparameters (dbs);
	/*
	toinit ();
	toprint ();
	*/
	dbs->intermediate.misc.ncomp = getmycompnumber ();
	if (dbs->intermediate.misc.ncomp < 0)
	    dbs->intermediate.misc.ncomp = 0;
	if ((dbs->intermediate.misc.tsem = semCreate ()) == 0) {
	    PERROR ("tsem semCreate");
	    return (-2);
	}
	if ((dbs->intermediate.misc.csem = semCreate ()) == 0) {
	    PERROR ("csem semCreate");
	    return (-3);
	}
	if ((dbs->intermediate.misc.psem = semCreate ()) == 0) {
	    PERROR ("psem semCreate");
	    return (-4);
	}
	if ((dbs->intermediate.misc.dcusem = semCreate ()) == 0) {
	    PERROR ("dcusem semCreate");
	    return (-5);
	}
	if ((dbs->intermediate.misc.adcsem = semCreate ()) == 0) {
	    PERROR ("adcsem semCreate");
	    return (-6);
	}
	if ((dbs->intermediate.misc.ksem = semCreate ()) == 0) {
	    PERROR ("ksem semCreate");
	    return (-7);
	}
	if ((dbs->intermediate.misc.rsem = semCreate ()) == 0) {
	    PERROR ("rsem semCreate");
	    return (-8);
	}
	SLEEP (1);
	PRINTF ("excHookAdd\n");
#ifndef	NOUNSUSPENDER
	excHookAdd (unsuspender);
#endif
	if (taskSpawn ("tportuc", 200, 0, 7000, portwrite, &sockuc, PORTUC,
	  (char *)&dbs->data.misc.iclock, sizeof(dbs->data.misc.iclock)) == -1){
	    PERROR ("portuc taskSpawn");
	    return (-9);
	}
	SLEEP (1);
	if (taskSpawn ("tportrd", 200, 0, 7000, portwrite, &sockrd, PORTRD,
	  (char *)&dbs->data, sizeof(dbs->data)) == -1) {
	    PERROR ("portrd taskSpawn");
	    return (-10);
	}
	SLEEP (1);
	if (taskSpawn ("tportwp", 200, 0, 7000, portread, &sockwp, PORTWP,
	  (char *)&dbs->parameter, sizeof(dbs->parameter), &bufwp) == -1) {
	    PERROR ("portwp taskSpawn");
	    return (-11);
	}
	SLEEP (1);
	if (taskSpawn ("tportrp", 200, 0, 7000, portwrite, &sockrp, PORTRP,
	  (char *)&dbs->parameter, sizeof(dbs->parameter)) == -1) {
	    PERROR ("portrp taskSpawn");
	    return (-12);
	}
	SLEEP (1);
	if (taskSpawn ("tportwb", 200, 0, 7000, portread, &sockwb, PORTWB,
	  (char *)&dbs->biparameter, sizeof(dbs->biparameter), &bufwb) == -1) {
	    PERROR ("portwb taskSpawn");
	    return (-13);
	}
	SLEEP (1);
	if (taskSpawn ("tportrb", 200, 0, 7000, portwrite, &sockrb, PORTRB,
	  (char *)&dbs->biparameter, sizeof(dbs->biparameter)) == -1) {
	    PERROR ("portrb taskSpawn");
	    return (-14);
	}
	SLEEP (1);
	if (taskSpawn ("tporter", 200, 0, 7000, eportwrite, &socker, PORTER,
	  (char *)&dbs->error, sizeof(dbs->error), &bufer) == -1) {
	    PERROR ("porter taskSpawn");
	    return (-15);
	}
	SLEEP (1);
	if (taskSpawn ("tportgn", 175, 0, 7000, eportwrite, &sockgn, PORTGN,
	  (char *)&(GONG), sizeof(GONG), &bufgn) == -1) {
	    PERROR ("portgn taskSpawn");
	    return (-16);
	}
	SLEEP (1);
	if (taskSpawn ("tportadc", 195, F, 7000, adcbroadcast, dbs) == -1) {
	    PERROR ("portadc taskSpawn");
	    return (-20);
	}
	SLEEP (1);
	if (taskSpawn ("tportdcu", 190, F, 7000, dcubroadcast, dbs) == -1) {
	    PERROR ("portdcu taskSpawn");
	    return (-21);
	}
	SLEEP (2);
	PRINTF ("taskSpawn (adctask[n])\n");
	for (n = 0; n < N_COMP; n++)
	    if (taskSpawn (adctask[n], 185 , 0, 7000, uportread,
	      &sockadc[n], PORTADC0+n, (char *)&dbs->data.adc[n],
	      sizeof(dbs->data.adc[n]), &bufadc[n]) == -1) {
		PERROR ("portadc[n] taskSpawn");
		return (-30-n);
	    } else {
		SLEEP (1);
	    }
	for (n = 0; n < N_DCU; n++)
	    if (taskSpawn (dcutask[n], 180, 0, 7000, uportread,
	      &sockdcu[n], PORTDCU0+n, (char *)&dbs->data.dcu[n],
	      sizeof(dbs->data.dcu[n]), &bufdcu[n]) == -1) {
		PERROR ("portdcu[n] taskSpawn");
		return (-33-n);
	    } else {
		SLEEP (1);
	    }
	SLEEP (2);
	PRINTF ("taskSpawn (tksync)\n");
	if (taskSpawn ("tksync", 172, F, 7000, ksync, dbs) == -1) {
	    PERROR ("ksync taskSpawn");
	    return (-39);
	}
	SLEEP (1);
	PRINTF ("taskSpawn (ttwork)\n");
	if (taskSpawn ("ttwork", 160, F, 7000, twork, dbs) == -1) {
	    PERROR ("twork taskSpawn");
	    return (-40);
	}
	SLEEP (1);
	PRINTF ("taskSpawn (tcwork)\n");
	if (taskSpawn ("tcwork", 170, F, 7000, cwork, dbs) == -1) {
	    PERROR ("cwork taskSpawn");
	    return (-41);
	}
	SLEEP (1);
	PRINTF ("taskSpawn (tpwork)\n");
	if (taskSpawn ("tpwork", 150, F, 7000, pwork, dbs) == -1) {
	    PERROR ("pwork taskSpawn");
	    return (-42);
	}
	SLEEP (1);
	PRINTF ("taskSpawn (trwork)\n");
	if (taskSpawn ("trwork", 198, F, 7000, rwork, dbs) == -1) {
	    PERROR ("rwork taskSpawn");
	    return (-43);
	}
	SLEEP (1);
	PRINTF ("taskSpawn (trspeed)\n");
	if (taskSpawn ("trspeed", 200, 0, 7000, rspeed, &sockrs, PORTRS,
	  (char *)bufrs, sizeof(bufrs)) == -1) {
	    PERROR ("rspeed taskSpawn");
	    return (-44);
	}
	SLEEP (2);
	PRINTF ("setoclock\n");
	SLEEP (5);
	if (setoclock (dbs) == 0)
	    if (taskSpawn ("tcsync", 172, F, 7000, csync, dbs) == -1) {
		PERROR ("csync taskSpawn");
		return (-45);
	    }
	strcpy (goodhost, "");
#ifdef	SUN3EAUXCLKRATESETBUGNOTFIXED
	CLOCK_RATESET (CLOCKRATE/2);
#else
	CLOCK_RATESET (CLOCKRATE);
#endif
	if (CLOCK_CONNECT (otick, dbs) == -1) {
	    PERROR ("otick CLOCK_CONNECT");
	    return (-50);
	}
	CLOCK_ENABLE ();
	PRINTF ("return\n");
	return (0);
}

twait () { semTake (dbs->intermediate.misc.tsem); }
cwait () { semTake (dbs->intermediate.misc.csem); }
pwait () { semTake (dbs->intermediate.misc.psem); }
kwait () { semTake (dbs->intermediate.misc.ksem); }
rwait () { semTake (dbs->intermediate.misc.rsem); }
rfire () { semGive (dbs->intermediate.misc.rsem); }
dcuwait () { semTake (dbs->intermediate.misc.dcusem); }
adcwait () { semTake (dbs->intermediate.misc.adcsem); }
dcufire () { semGive (dbs->intermediate.misc.dcusem); }
adcfire () { semGive (dbs->intermediate.misc.adcsem); }

int	adjtimeok = YES;
long	firstutime;

/* csync - clock sync retry
 */
csync (db)
database	*db;
{
	do {
	    sleep (120);
	} while (setoclock (db) == 0);
}

/* setoclock - get oven clock time of day
 */
setoclock (db)
database	*db;
{
	d_misc	*dmisc = &dbs->data.misc;
	i_misc	*imisc = &dbs->intermediate.misc;
	unsigned long	geticlock();
	long	getutime();
	unsigned long	iclock;
	long	uclock;
	int	msclock;
	int	timeok;

	iclock = geticlock ();
	timeok = (iclock > 30000000 ? 1 : 0);
	uclock = timeok ? getutime (iclock) : 0;
	msclock = (uclock%(PERIODS*N_MAP))*1000;
	holditasec = 1;
	dmisc->iclock = iclock;
	dmisc->uclock = uclock;
	imisc->msclock = msclock;
	holditasec = 0;
	adjtimeok = timeok;
	if (timeok)
	    firstutime = uclock;
	return (timeok);
}
unsigned long
Getiptime (host)
char	*host;
{
	unsigned long	clock;
	unsigned long	getiptime();

	printf ("getiptime (%s) = ", host);
	clock = getiptime (host);
	printf ("%u\n", clock);
	return (clock);
}

/* geticlock - get ip clock time of day
 */
unsigned long
geticlock ()
{
	unsigned long	uclock;
	unsigned long	getiptime();
	int	oven = getmyovennumber ();
	int	comp;
	char	host[32];

	if (oven < 0)
	    oven = 0;
	if (goodhost[0])
	    if ((uclock = getiptime (goodhost)) > 30000000)
		return (uclock);
	if ((uclock = getiptime (sysBootHost)) > 30000000)
	    return (uclock);
/*	for (oven = 0; oven < N_OVEN; oven++)				*/
	    for (comp = 0; comp < N_COMP; comp++) {
		sprintf (host, "oven%dv%d", oven, comp);
		if ((uclock = getiptime (host)) > 30000000)
		    return (uclock);
	    }
	if (strcmp ("crater", sysBootHost))
	    if ((uclock = getiptime ("crater")) > 30000000)
		return (uclock);
	if (strcmp ("dorado", sysBootHost))
	    if ((uclock = getiptime ("dorado")) > 30000000)
		return (uclock);
	if (strcmp ("astro", sysBootHost))
	    if ((uclock = getiptime ("astro")) > 30000000)
		return (uclock);
	return (0);
}

static	int	adjtime = YES;
yesadjtime () { adjtime = YES; }
noadjtime () { adjtime = NO; }

/* ksync - keep clocks synchronized
 */
ksync (db)
database	*db;
{
	unsigned long	iclock;
	unsigned long	geticlock();
	long	rt, lt, diff;
	long	getutime();

	forever {
	    kwait ();
	    if (adjtime == 0)
		continue;
	    if (adjtimeok == 0)
		continue;
	    if ((iclock = geticlock ()) == 0)
		continue;
	    rt = getutime (iclock);		/* remote time */
	    lt = db->data.misc.uclock;		/* local  time */
	    diff = ABS (lt-rt);
	    if (diff <= 1 || 10 <= diff)
		continue;
	    if (lt>rt)
		CLOCK_RATESET (CLOCKRATE*(100-PERCENT)/100);
	    else
		CLOCK_RATESET (CLOCKRATE*(100+PERCENT)/100);
	    sleep (diff*100/PERCENT);
	    CLOCK_RATESET (CLOCKRATE);
	}
}

/* getparameters - initialize parameters database from neighbors
 */
getparameters (db)
database	*db;
{
	int	comp = getmycompnumber ();
	int	status;

	status = getdatabase(PORTRP,(char *)&db->parameter,sizeof(p_database));
	if (status)
	    getdatabaser(PORTRP,(char *)&db->parameter,sizeof(p_database));
	if (comp >= 0)
	    db->parameter.misc.vnumber = comp;
	if (status ||
	  getdatabase(PORTRB,(char *)&db->biparameter,sizeof(b_database)))
	    getdatabaser(PORTRB,(char *)&db->biparameter,sizeof(b_database));
	savedatabases ();
}

/* getdatabase - initialize database from neighbors
 */
getdatabase (port, buffer, nbytes)
int	port;
char	*buffer;
int	nbytes;
{
	int	comp, oven = getmyovennumber ();
	char	host[32];
	int	ip, myip = getmyinetaddr ();

	if (goodhost[0])
	    if (getdatabasen (goodhost, port, buffer, nbytes) == 0)
		return (0);
	if (getdatabasen (sysBootHost, port, buffer, nbytes) == 0)
	    return (0);
	if (oven >= 0)
	    for (comp = 0; comp < N_COMP; comp++) {
		sprintf (host, "oven%dv%d", oven, comp);
		if ((ip = hostGetByName (host)) == -1)
		    continue;
		if (ip == myip)
		    continue;
		if (getdatabasen (host, port, buffer, nbytes) == 0)
		    return (0);
	    }
	if (strcmp ("crater", sysBootHost))
	    if (getdatabasen ("crater", port, buffer, nbytes) == 0)
		return (0);
	if (strcmp ("dorado", sysBootHost))
	    if (getdatabasen ("dorado", port, buffer, nbytes) == 0)
		return (0);
	if (strcmp ("astro", sysBootHost))
	    if (getdatabasen ("astro", port, buffer, nbytes) == 0)
		return (0);
	return (-1);
}

/* getdatabasen - initialize database from neighbor
 */
getdatabasen (host, port, buffer, nbytes)
char	*host;
int	port;
char	*buffer;
int	nbytes;
{
	int	ip;
	int	s;
	char	mess[80];

	if ((ip = hostGetByName (host)) == -1)
	    return (-1);
	s = tportread (ip, port, buffer, nbytes);
	if (s) {
	    sprintf (mess, "getdatabasen (%s, %d, buffer, %d) = %d",
	      host, port, nbytes, s);
	    PERROR (mess);
	} else {
	    strcpy (goodhost, host);
	}
	return (s);
}

/* broadcast2neighbors - implement broadcasting by writing to the other
 * V computers for this oven.
 */
broadcast2neighbors (port, buf, nbytes)
int	port;
char	*buf;
int	nbytes;
{
	int	comp, oven = getmyovennumber ();
	char	name[32];
	int	ip, myip = getmyinetaddr ();

	if (oven < 0)
	    return (-1);
	if (myip == 0)
	    return (-1);
	for (comp = 0; comp < N_COMP; comp++) {
	    sprintf (name, "oven%dv%d", oven, comp);
	    if ((ip = hostGetByName (name)) == -1)
		continue;
	    if (ip == myip)
		continue;
	    uportwrite (ip, port, buf, nbytes);
	}
	return (0);
}

/* getmyovennumber - find my oven number based on my internet address and
 * the hostnames in the table.
 */
getmyovennumber ()
{
	int	comp, oven;
	char	name[32];
	int	ip, myip = getmyinetaddr ();

	if (myip == 0)
	    return (-1);
	for (oven = 0; oven < N_OVEN; oven++)
	    for (comp = 0; comp < N_COMP; comp++) {
		sprintf (name, "oven%dv%d", oven, comp);
		if ((ip = hostGetByName (name)) == -1)
		    continue;
		if (ip == myip)
		    return (oven);
	    }
	return (-1);
}

/* getmycompnumber - find my comp number based on my internet address and
 * the hostnames in the table.
 */
getmycompnumber ()
{
	int	comp, oven;
	char	name[32];
	int	ip, myip = getmyinetaddr ();

	if (myip == 0)
	    return (-1);
	for (oven = 0; oven < N_OVEN; oven++)
	    for (comp = 0; comp < N_COMP; comp++) {
		sprintf (name, "oven%dv%d", oven, comp);
		if ((ip = hostGetByName (name)) == -1)
		    continue;
		if (ip == myip)
		    return (comp);
	    }
	return (-1);
}

sleep (s)
int	s;
{
	taskDelay (sysClkRateGet() * s);
}

/* getdatabaser - initialize database from nvram
 */
getdatabaser (port, buffer, nbytes)
int	port;
char	*buffer;
int	nbytes;
{
	char	*nvram_addrp();
	char	*nvram_addrb();

	if (nvram_dead ())
	    return (-1);
	if (port == PORTRP)
	    bcopy (nvram_addrp (), buffer, nbytes);
	if (port == PORTRB)
	    bcopy (nvram_addrb (), buffer, nbytes);
	return (0);
}

/* savedatabaser - save database to nvram
 */
savedatabaser (port, buffer, nbytes)
int	port;
char	*buffer;
int	nbytes;
{
	char	*nvram_addrp();
	char	*nvram_addrb();

	if (nvram_dead ())
	    return (-1);
	if (port == PORTWP)
	    bcopy (buffer, nvram_addrp (), nbytes);
	if (port == PORTWB)
	    bcopy (buffer, nvram_addrb (), nbytes);
	return (0);
}

/* savedatabases
 */
savedatabases ()
{
	savedatabaseb ();
	savedatabasep ();
}

/* savedatabasep
 */
savedatabasep ()
{
	saveparameters ((char *)&dbs->parameter, sizeof(p_database));
}

/* savedatabaseb
 */
savedatabaseb ()
{
	savebiparameters ((char *)&dbs->biparameter, sizeof(b_database));
}

/* saveparameters - save parameters in nvram
 */
saveparameters (buffer, nbytes)
char	*buffer;
int	nbytes;
{
	char	*nvram_addrp();

	if (nvram_dead ())
	    return;
	bcopy (buffer, nvram_addrp (), nbytes);
}

/* savebiparameters - save biparameters in nvram
 */
savebiparameters (buffer, nbytes)
char	*buffer;
int	nbytes;
{
	char	*nvram_addrb();

	if (nvram_dead ())
	    return;
	bcopy (buffer, nvram_addrb (), nbytes);
}

char *
nvram_addrp ()
{
	char	*nvram_addr();

	return (nvram_addr()+sizeof(b_database));
}

char *
nvram_addrb ()
{
	char	*nvram_addr();

	return (nvram_addr());
}

/* NVRAM routines
 */
nvram_dead ()
{
	char	*nvram_addr();
	char	*addr;
	char	probe;

	addr = nvram_addr ();
	if (addr)
	    return (vxMemProbe (addr, 0, sizeof(probe), &probe) != 0);
	else
	    return (1);
}

char *
nvram_addr ()
{
	char	*addr = (char *)0x400000;

	return (addr);
}
