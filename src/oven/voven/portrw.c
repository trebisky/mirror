#include <vxWorks.h>
#include <stdlib.h>
#include <taskLib.h>
#include <socket.h>
#include <in.h>
#include <systime.h>
#include <inetLib.h>
#include <arch/mc68k/esfMc68k.h>

#define	PERROR	perror

#define	MAXNBYTES	4096
#define	MAXUBYTES	8192

#define	ABS(a)		( ((a) >= 0) ? (a) : -(a) )
#define	MIN(a,b)	( ((a) < (b)) ? (a) : (b) )
#define	MAX(a,b)	( ((a) > (b)) ? (a) : (b) )

int	nolistentoports;	/* set to one to shutdown	*/

portstart ()	{ nolistentoports = 0; }
portstop ()	{ nolistentoports = 1; }

/* getfptaskoptions - return options for spawn FP tasks
 */
getfptaskoptions ()
{
	return (VX_DEALLOC_STACK | VX_FP_TASK);
}

/* unsuspender - watchdog for tasks that take exceptions
 */
unsuspender (tid, vecNum, pEsf)
int	tid;
int	vecNum;
ESF0	*pEsf;
{
	texcerror ();
	taskRestart (tid);
}

/* uportread - listen for datagrams and read data into buffer
 */
uportread (sock, port, buf, nbytes, sbuf)
int	*sock;
int	port;
char	*buf;
int	nbytes;
char	**sbuf;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer = buf;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;

	if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	    return (-1);
	*sock = s;

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, &sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	/* allocate temporary buffer
	 */
	if ((buffer = (char *)malloc (nbytes)) == 0)
	    return (-5);
	*sbuf = buffer;

	while (!nolistentoports) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    r = sizeof(rsockaddr);
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    for (n = 0; n < nbytes; n += nxfer) {
	        if ((nxfer = recvfrom (s, buffer+n, MIN (MAXUBYTES, nbytes-n),
		    0, &rsockaddr, &r)) < 0)
		    break;
	        if (nxfer == 0)
		    break;
	    }
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    if (n == nbytes)
		bcopy (buffer, buf, nbytes);
	}
	free (buffer);
	close (s);
	return (0);
}

/* uportbroadcast - broadcast datagrams from buffer
 */
uportbroadcast (port, buf, nbytes)
int	port;
char	*buf;
int	nbytes;
{
	broadcast2neighbors (port, buf, nbytes);
}

/* uportwrite - write datagrams from buffer
 */
uportwrite (ip, port, buf, nbytes)
unsigned long	ip;
int	port;
char	*buf;
int	nbytes;
{
	struct	sockaddr_in	sockaddr;
	char	*buffer = buf;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;

	if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	    return (-1);

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= ip;

	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = sendto (s, buffer+n, MIN (MAXUBYTES, nbytes-n),
	      0, &sockaddr, sizeof(sockaddr))) < 0) {
		PERROR ("");
		break;
	      }
	}
	close (s);
	return (0);
}

/* portread - listen for connections and read data into buffer
 */
portread (sock, port, buf, nbytes, sbuf)
int	*sock;
int	port;
char	*buf;
int	nbytes;
char	**sbuf;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer = buf;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (-1);
	*sock = s;

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, &sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	if (listen (s, 1) < 0)
	    return (-4);

	/* allocate temporary buffer
	 */
	if ((buffer = (char *)malloc (nbytes)) == 0)
	    return (-5);
	*sbuf = buffer;

	while (!nolistentoports) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    n = sizeof(rsockaddr);
	    if ((r = accept (s, &rsockaddr, &n)) < 0)
	        continue;
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    for (n = 0; n < nbytes; n += nxfer) {
	        if ((nxfer = read (r, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0)
		    break;
	        if (nxfer == 0)
		    break;
	    }
	    close (r);
	    if (n == nbytes) {
		bcopy (buffer, buf, nbytes);
		savedatabaser (port, buf, nbytes);
	    }
	}
	free (buffer);
	close (s);
	return (0);
}

/* portwrite - listen for connections and write data from buffer
 */
portwrite (sock, port, buf, nbytes)
int	*sock;
int	port;
char	*buf;
int	nbytes;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer = buf;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (-1);
	*sock = s;

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, &sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	if (listen (s, 1) < 0)
	    return (-4);

	while (!nolistentoports) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    n = sizeof(rsockaddr);
	    if ((r = accept (s, &rsockaddr, &n)) < 0)
	        continue;
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    for (n = 0; n < nbytes; n += nxfer) {
	        if ((nxfer = write(r, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0)
		    break;
	    }
	    close (r);
	}
	close (s);
	return (0);
}

/* eportwrite - listen for connections and write data from buffer (and ZERO)
 */
eportwrite (sock, port, buf, nbytes, sbuf)
int	*sock;
int	port;
char	*buf;
int	nbytes;
char	**sbuf;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer = buf;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;
	int	*bpi, *bpo;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (-1);
	*sock = s;

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, &sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	if (listen (s, 1) < 0)
	    return (-4);

	/* allocate temporary buffer
	 */
	if ((buffer = (char *)malloc (nbytes)) == 0)
	    return (-5);
	*sbuf = buffer;

	while (!nolistentoports) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    n = sizeof(rsockaddr);
	    if ((r = accept (s, &rsockaddr, &n)) < 0)
	        continue;
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    for (bpi=(int *)buf,bpo=(int *)buffer; bpi<(int *)(buf+nbytes); ) {
		*bpo++ = *bpi;
		*bpi++ = 0;
	    }
	    for (n = 0; n < nbytes; n += nxfer) {
	        if ((nxfer = write(r, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0)
		    break;
	    }
	    close (r);
	}
	free (buffer);
	close (s);
	return (0);
}

/* rspeed - listen for connections and write data from buffer
 */
rspeed (sock, port, buf, nbytes)
int	*sock;
int	port;
char	*buf;
int	nbytes;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer = buf;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (-1);
	*sock = s;

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, &sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	if (listen (s, 1) < 0)
	    return (-4);

	while (!nolistentoports) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    n = sizeof(rsockaddr);
	    if ((r = accept (s, &rsockaddr, &n)) < 0)
	        continue;
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    do {
		getrspeedi (buffer);
		for (n = 0; n < nbytes; n += nxfer) {
		    if ((nxfer = write(r,buffer+n,MIN(MAXNBYTES,nbytes-n))) < 0)
			break;
		}
	    } while (nxfer >= 0);
	    close (r);
	}
	close (s);
	return (0);
}

/* getrspeedi - get the rspeed info and make it machine independent
 */
getrspeedi (buf)
char	*buf;
{
	long	t;
	int	s;
	long	ti, si;

	getrspeed (&t, &s);
	ti = htonl (t);
	si = htonl (s);
	bcopy ((char *)&ti, buf           , sizeof(ti));
	bcopy ((char *)&si, buf+sizeof(ti), sizeof(si));
}

extern	char	sysBootHost[];

/* getbroadcastaddr - get the current broadcast address
 */
unsigned long
getbroadcastaddr ()
{
	int	i;
	unsigned long	l;
	unsigned long	getnetpart();
	unsigned long	gethostpart();

	if (((i = hostGetByName (sysBootHost)) == -1) &&
	  ((i = (int)inet_addr (sysBootHost)) == -1))
	    return (0);
	l = *((unsigned long *)(&i));
	l = getnetpart (l) | gethostpart ((unsigned long)INADDR_BROADCAST);
	return (l);
}

/* gethostpart - get the host part of the address
 */
unsigned long
gethostpart (a)
unsigned long	a;
{
	unsigned long	getnetpart();

	a -= getnetpart (a);
	return (a);
}

/* getnetpart - get the network part of the address
 * should understand subnet mask currently set
 */
unsigned long
getnetpart (a)
unsigned long	a;
{
	struct	in_addr	d;
	unsigned long	n;
	char	a1[INET_ADDR_LEN];
	char	a2[INET_ADDR_LEN];

	d.s_addr = a;
	inet_ntoa_b (d, a1);
	inet_netof_string (a1, a2);
	n = inet_addr (a2);
	return (n);
}

/* getmyinetaddr - get my own internet address
 */
getmyinetaddr ()
{
/*
	char	bootDev[16], hostName[32], bootFile[160];
	char	ead[16], bad[16], had[16], gad[16];
	char	usr[16], passwd[16];
	int	pProcNum, pFlags;
	char	*s;
	int	i;
	char	*bootStringToParams();
	char	*index();

	if (*(s = bootStringToParams (sysBootLine, bootDev, hostName, bootFile,
	  ead, bad, had, gad, usr, passwd, &pProcNum, &pFlags)))
	    return (0);
	if (s = index (ead, ':'))
	    *s = 0;
 */
	int	i;
	char	ead[32];

	if (ifAddrGet ("ln0", ead) == -1)
	    return (0);
	if ((i = (int)inet_addr (ead)) == -1)
	    return (0);
	return (i);
}

/* tportread  - read  from an IP port.
 */
tportread (ip, port, buf, nbytes)
int	ip;
int	port;
char	*buf;
int	nbytes;
{
	struct	sockaddr_in	sockaddr;
	unsigned short	portnumber = port;
	long	ipnumber = ip;
	int	s;
	int	nxfer;
	int	n;
	char	*buffer;
	struct	timeval	timeout;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
	    return (-1);
	}

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= htonl (ipnumber);

	if (connectWithTimeout (s, &sockaddr, sizeof (sockaddr), &timeout)<0) {
	    return (-3);
	}
	if ((buffer = (char *)malloc (nbytes)) == 0) {
	    close (s);
	    return (-5);
	}
	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = read (s, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0) {
		PERROR ("read");
		free (buffer);
		close (s);
		return (-6);
	    }
	    if (nxfer == 0) {
		free (buffer);
		close (s);
		return (-7);
	    }
	}
	bcopy (buffer, buf, nbytes);
	free (buffer);
	close (s);
	return (0);
}
