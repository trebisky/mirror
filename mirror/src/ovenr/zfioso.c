/* ZFIOSO -- FIO interface to TCPIP sockets.
 * Skip Schaller, Steward Observatory, Aug 5, 1991
 * This IRAF-style kernel interface to TCP sockets supports both client and
 * server end connections to TCP sockets.  It has been tested on IRAF
 * hosts such as sun3, sun4 (sunos 4.1.1), vax (bsd 4.3), and aosvs.
 * It also works on non-IRAF hosts, Wind River Systems VxWorks (5.0, mv147),
 * and MSDOS (PCNFS toolkit).
 */

/* pick up the IRAF definitions
 */
#ifdef	vw
#define	NOTIRAFHOST
#endif
#ifdef	MSDOS
#define	NOTIRAFHOST
#endif

#ifndef	NOTIRAFHOST
#define	import_zfstat
#define import_spp
#include <iraf.h>
#include "sknames.h"
#define bzero(a,n)      memset(a,0,n)
#define bcopy(a,b,n)    memmove(b,a,n)
#else
#define	SZ_FNAME	63
#define	ERR		(-1)
#define	XERR		(-1)
#define	XOK		0
#define	XCHAR		short
#define	XINT		int
#define	PKCHAR		XCHAR
#define	XLONG		long
#define	READ_ONLY	1
#define	READ_WRITE	2
#define	WRITE_ONLY	3
#define	APPEND		4
#define	NEW_FILE	5
#define	FSTT_BLKSIZE	1
#define	FSTT_FILSIZE	2
#define	FSTT_OPTBUFSIZE	3
#define	FSTT_MAXBUFSIZE	4
#define	ZWMSEC	zwmsec_
#define	ZOPNSO	zopnso_
#define	ZCLSSO	zclsso_
#define	ZARDSO	zardso_
#define	ZAWRSO	zawrso_
#define	ZAWTSO	zawtso_
#define	ZSTTSO	zsttso_
#endif


#ifndef	NOT_IMPLEMENTED		/* define this to get stubb routines */

#ifndef	MSDOS			/* includes for most UNIX hosts */
#ifndef	vw
#ifndef	AOSVS
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define	tcp_close	close
#define	tcp_read	read
#define	tcp_write	write
#endif
#endif
#endif

/* specific host includes and definitions
 */

#ifdef	AOSVS	/* definitions for AOSVS */
#define	gethostbyname	tcp_gethostbyname
struct	hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;	/* host address type */
	int	h_length;	/* length of address */
	char	*h_addr;	/* address */
};
#define	h_addr_list	h_addr		/* no-op under AOSVS */
struct	servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	int	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};
struct in_addr {
	unsigned long s_addr;
};
struct sockaddr_in {
	short	sin_family;
	unsigned short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};
struct hostent	*gethostbyname();
#define	getservbyname(a,b)	( 0 )
#define	inet_addr(x)	( 0xffffffff )
#define	AF_INET		0
#define	INADDR_ANY	0
#define	SOCK_STREAM	0
#define	SOMAXCONN	0

/* server mode not supported for AOSVS
 */
static	socket()	{ return (-1); }
static	bind  ()	{ return (-1); }
static	listen()	{ return (-1); }
static	accept()	{ return (-1); }
#endif

#ifdef	vw		/* definitions for VxWorks */
#define	tcp_close	close
#define	tcp_read	read
#define	tcp_write	write
#include <vxWorks.h>
#include <socket.h>
#include <in.h>
#include <inetLib.h>
struct	hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;	/* host address type */
	int	h_length;	/* length of address */
	char	**h_addr_list;	/* address */
};
#define	h_addr	h_addr_list[0]
struct	servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	int	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};
extern	unsigned long	getHostByName();
#define	getservbyname(a,b)	( 0 )

static	ZWMSEC (milliseconds)
int	milliseconds;
{
	int	nticks = milliseconds * sysClkRateGet () / 1000;

	taskDelay (nticks ? nticks : 1);
}

static	atoi(c)
char	*c;
{
	int	i;

	if (sscanf (c, "%d", &i))
	    return (i);
	else
	    return (0);
}
#endif

#ifdef	MSDOS		/* definitions for MSDOS */
#define	tcp_close	close
#define	tcp_read	read
#define	tcp_write(a,b,c)	send(a,b,c,0)
#include </toolkit/include/sys/types.h>
#define	_SIZE_T_DEFINED
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
extern unsigned long inet_addr ();
#define	h_addr_list	h_addr		/* no-op under MSDOS */

static	ZWMSEC (milliseconds)
int	milliseconds;
{
}
#endif


#define	MAXSOFILES	64
#define	SO_OPTBUFSIZE	4096
#define	SO_MAXBUFSIZE	4096
#define	DELIM		','

static	int	zsonbytes[MAXSOFILES];
static	int	zsofd[MAXSOFILES];


/* ZOPNSO - Open.
 * The connection name should be a character string consisting of "host,port".
 * The host part is a hostname like "astro" or "astro.as.arizona.edu"
 * (or on some systems, an internet address in decimal dot notation
 * like "128.196.176.1").
 * The port part is a decimal "well-known" port number like "5432"
 * (or on some systems, a service name like "timed").
 * If the host part is the null string (the connection name begins with
 * the ',' delimiter), server mode is indicated.  (Not implemented on all
 * systems.)
 * The file access mode should always be READ_WRITE.
 */
ZOPNSO (connectionname, mode, chan)
PKCHAR	*connectionname;	/* name of host,port		*/
XINT	*mode;			/* file access mode		*/
XINT	*chan;			/* file number (output)		*/
{
	int fd;
	char	host[SZ_FNAME+SZ_FNAME];
	char	*p;
	int	port;
	struct	hostent	*hp;
	int	s;
	int	r;
	struct	sockaddr_in	sockaddr;
	unsigned short sport;
	struct	servent	*serv;
	unsigned long haddr;
	struct	hostent hps;
	unsigned long hpsa;
	char	*hpsal;
	struct {			/* used only by MSDOS */
	    int	send_packet_size;
	    int	recv_packet_size;
	    int	send_buf_size;
	    int	recv_buf_size;
	    int	ack_delay;
	} sp;

	switch (*mode) {
	case READ_ONLY:
	    fd = ERR;
	    break;

	case WRITE_ONLY:
	    fd = ERR;
	    break;

	case READ_WRITE:
	    fd = ERR;
	   
	    /* parse connectionname into host and port number
	     */
	    strncpy (host, (char *)connectionname, SZ_FNAME+SZ_FNAME);
	    for (p = host; *p && p < host+SZ_FNAME; p++)
		if (*p == DELIM) {
		    *p++ = 0;
		    break;
		}
	    if (*p == 0 || p >= host+SZ_FNAME)
		break;

	    /* try to lookup service name and get a port number.
	     * If that fails or is not implemented,
	     * interpret the service name as a decimal port number
	     */
	    serv = getservbyname (p, 0L);
	    if (serv == 0) {
		port = atoi (p);
		sport = htons((short)port);
	    } else {
		sport = serv->s_port;
	    }

	    /* check if server or client mode
	     */
	    if (strlen (host) != 0) {

		/* client mode - try looking up the symbolic hostname
		 * and getting an internet address.
		 */

#ifndef	vw
		hp = gethostbyname (host);
#else
		bzero ((char *)&hps, sizeof(hps));
		hps.h_addr_list = &hpsal;
		hps.h_addr = (char *)&hpsa;
		hp = &hps;
		haddr = hostGetByName (host);
		if (haddr == 0xffffffff)
		    hp = 0;
		else {
		    hp->h_length = sizeof(haddr);
		    bcopy ((char *)&haddr, (char *)hp->h_addr, hp->h_length);
		}
#endif

		/* If the symbolic hostname lookup fails, try interpreting
		 * the string as a dotted decimal internet address.
		 */
		if (hp == 0) {
		    bzero ((char *)&hps, sizeof(hps));
		    hps.h_addr_list = &hpsal;
		    hps.h_addr = (char *)&hpsa;
		    hp = &hps;
		    haddr = inet_addr (host);
		    if (haddr == 0xffffffff)
			break;
		    hp->h_length = sizeof(haddr);
		    bcopy ((char *)&haddr, (char *)hp->h_addr, hp->h_length);
		}

		/* now try to open the TCP connection
		 */
#ifndef	AOSVS
		bzero ((char *)&sockaddr, sizeof(sockaddr));
		bcopy ((char *)hp->h_addr,(char *)&sockaddr.sin_addr,
		  hp->h_length);
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = sport;
		if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		    break;
		if (connect (s,(struct sockaddr *)&sockaddr,sizeof(sockaddr))<0)
		    break;
#else
		if ((s = tcp_open (*((unsigned long *)(hp->h_addr)), port)) < 0)
		    break;
#endif
		fd = s;
		zsofd[fd] = s;
	    } else {

		/* server mode.
		 */
#ifdef	MSDOS
		/* do the following under MSDOS to improve efficiency
		 */
		sp.send_packet_size = 1024;
		sp.recv_packet_size = 512;
		sp.send_buf_size = 1024;
		sp.recv_buf_size = 1024;
		sp.ack_delay = 0;
		if (setpparm (IPPROTO_TCP, (char *)&sp) == -1)
		    break;
#endif

		/* start listening to the given port
		 */
		if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		    break;
		}
		bzero ((char *)&sockaddr, sizeof(sockaddr));
		sockaddr.sin_family		= AF_INET;
		sockaddr.sin_port		= sport;
		sockaddr.sin_addr.s_addr	= INADDR_ANY;
		if (bind(s,(struct sockaddr *)&sockaddr,sizeof(sockaddr)) < 0) {
		    break;
		}
		if (listen (s, SOMAXCONN) < 0) {
		    break;
		}
		if ((r = accept (s, (struct sockaddr *)0, (int *)0)) < 0) {
		    break;
		}
		fd = s;
		zsofd[fd] = r;
	    }
	    break;

	case NEW_FILE:
	    fd = ERR;
	    break;

	case APPEND:
	    fd = ERR;
	    break;

	default:
	    fd = ERR;
	}

	if ((*chan = fd) == ERR) {
	    *chan = XERR;
	} else if (fd >= MAXSOFILES) {
	    tcp_close (fd);
	    *chan = XERR;
	} else {
	    zsonbytes[fd] = 0;
	}
}


/* ZCLSSO -- Close.
 */
ZCLSSO (fd, status)
XINT	*fd;
XINT	*status;
{
	if (zsofd[*fd] != *fd)
	    tcp_close (zsofd[*fd]);
	*status = (tcp_close (*fd) == ERR) ? XERR : XOK;
}


/* ZARDSO -- "Asynchronous" read.  Initiate a read of at most
 * maxbytes bytes into the buffer BUF.  Status is returned
 * in a subsequent call to ZAWTSO.
 */
ZARDSO (chan, buf, maxbytes, offset)
XINT	*chan;			/* UNIX file number			*/
XCHAR	*buf;			/* output buffer			*/
XINT	*maxbytes;		/* max bytes to read			*/
XLONG	*offset;		/* 1-indexed file offset to read at	*/
{
	int	fd;
	int	n = *maxbytes;
	int	nxfer = 0;
	int	nread;
	char	*buffer = (char *)buf;
	int	r, s;

	fd = *chan;
	if (*offset > 0) {
	    zsonbytes[fd] = ERR;
	    return;
	}

	while (n) {
	    nread = (n < SO_MAXBUFSIZE) ? n : SO_MAXBUFSIZE;
	    nread = tcp_read (zsofd[fd], buffer, nread);
	    if (nread < 0) {
		nxfer = ERR;
		break;
	    } else if (nread == 0) {
		if (zsofd[fd] != fd) {	/* server mode only */
		    s = fd;
		    r = zsofd[fd];
		    close (r);
	            if ((r = accept (s, (struct sockaddr *)0, (int *)0)) < 0) {
			nxfer = ERR;
			break;
	            }
		    zsofd[fd] = r;
		}
		break;
	    } else {
		n      -= nread;
		nxfer  += nread;
		buffer += nread;
	    }
	}
	zsonbytes[fd] = nxfer;
}


/* ZAWRSO -- "Asynchronous" write.  Initiate a write of exactly
 * nbytes bytes from the buffer BUF.  Status is returned in a
 * subsequent call to ZAWTSO.
 */
ZAWRSO (chan, buf, nbytes, offset)
XINT	*chan;			/* UNIX file number		*/
XCHAR	*buf;			/* buffer containing data	*/
XINT	*nbytes;		/* nbytes to be written		*/
XLONG	*offset;		/* 1-indexed file offset	*/
{
	int	fd;
	int	n = *nbytes;
	int	nxfer = 0;
	int	nread;
	char	*buffer = (char *)buf;

	fd = *chan;
	if (*offset > 0) {
	    zsonbytes[fd] = ERR;
	    return;
	}

	while (n) {
	    nread = (n < SO_MAXBUFSIZE) ? n : SO_MAXBUFSIZE;
	    nread = tcp_write (zsofd[fd], buffer, nread);

	    if (nread < 0) {
		nxfer = ERR;
		break;
	    } else if (nread == 0) {
		ZWMSEC (10);
	    } else {
		n      -= nread;
		nxfer  += nread;
		buffer += nread;
	    }
	}
	zsonbytes[fd] = nxfer;
}


/* ZAWTSO -- "Wait" for an "asynchronous" read or write to complete, and
 * return the number of bytes read or written, or ERR.
 */
ZAWTSO (fd, status)
XINT	*fd;
XINT	*status;
{
	if ((*status = zsonbytes[*fd]) == ERR)
	    *status = XERR;
}


/* ZSTTSO -- Return status.
 */
ZSTTSO (fd, param, lvalue)
XINT	*fd;
XINT	*param;
XLONG	*lvalue;
{

	switch (*param) {
	case FSTT_BLKSIZE:
	    (*lvalue) = 1L;
	    break;

	case FSTT_FILSIZE:
	    /* The file size is undefined if the file is a streaming file.
	     */
	    (*lvalue) = 0L;
	    break;

	case FSTT_OPTBUFSIZE:
	    (*lvalue) = SO_OPTBUFSIZE;
	    break;

	case FSTT_MAXBUFSIZE:
	    (*lvalue) = SO_MAXBUFSIZE;
	    break;

	default:
	    (*lvalue) = XERR;
	    break;
	}
}

#else

ZOPNSO (connectionname, mode, chan)
PKCHAR	*connectionname;	/* name of host,port		*/
XINT	*mode;			/* file access mode		*/
XINT	*chan;			/* file number (output)		*/
{
	*chan = XERR;
}
ZCLSSO (fd, status)
XINT	*fd;
XINT	*status;
{
	*status = XERR;
}
ZARDSO (chan, buf, maxbytes, offset)
XINT	*chan;			/* UNIX file number			*/
XCHAR	*buf;			/* output buffer			*/
XINT	*maxbytes;		/* max bytes to read			*/
XLONG	*offset;		/* 1-indexed file offset to read at	*/
{
}
ZAWRSO (chan, buf, nbytes, offset)
XINT	*chan;			/* UNIX file number		*/
XCHAR	*buf;			/* buffer containing data	*/
XINT	*nbytes;		/* nbytes to be written		*/
XLONG	*offset;		/* 1-indexed file offset	*/
{
}
ZAWTSO (fd, status)
XINT	*fd;
XINT	*status;
{
	*status = XERR;
}
ZSTTSO (fd, param, lvalue)
XINT	*fd;
XINT	*param;
XLONG	*lvalue;
{
	(*lvalue) = XERR;
}
#endif
