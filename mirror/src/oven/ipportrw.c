#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define	MAXNBYTES	4096

#define	ABS(a)		( ((a) >= 0) ? (a) : -(a) )
#define	MIN(a,b)	( ((a) < (b)) ? (a) : (b) )
#define	MAX(a,b)	( ((a) > (b)) ? (a) : (b) )
#define bzero(a,n)      memset(a,0,n)
#define bcopy(a,b,n)    memmove(b,a,n)

/* ipportwrite - write to   an IP port.
 */
ipportwrite (ip, port, buffer, nbytes)
int	ip;
int	port;
char	*buffer;
int	nbytes;
{
	struct	sockaddr_in	sockaddr;
	unsigned short	portnumber = port;
	long	ipnumber = ip;
	int	s;
	int	nxfer;
	int	n;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
	    perror ("socket");
	    return (s);
	}

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= htonl (ipnumber);

	if (connect (s, (struct sockaddr *)&sockaddr, sizeof (sockaddr)) < 0) {
/*	    perror ("connect");						*/
	    return (-1);
	}
	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = write (s, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0) {
		perror ("write");
		close (s);
		return (nxfer);
	    }
	}
	close (s);
	return (0);
}

/* ipportread  - read  from an IP port.
 */
ipportread (ip, port, buf, nbytes)
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
	char	*malloc();

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
	    perror ("socket");
	    return (s);
	}

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= htonl (ipnumber);

	if (connect (s, (struct sockaddr *)&sockaddr, sizeof (sockaddr)) < 0) {
/*	    perror ("connect");						*/
	    return (-1);
	}
	if ((buffer = malloc (nbytes)) == 0) {
	    close (s);
	    return (-1);
	}
	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = read (s, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0) {
		perror ("read");
		free (buffer);
		close (s);
		return (nxfer);
	    }
	    if (nxfer == 0) {
		free (buffer);
		close (s);
		return (-1);
	    }
	}
	bcopy (buffer, buf, nbytes);
	free (buffer);
	close (s);
	return (0);
}

/* getovenip - get the oven computer's ip number.
 */
getovenip (noven, ncomp)
int	noven;
int	ncomp;
{
	char	hostname[33];
	struct	hostent	*hp;
	long	ipnumber;
	int	ip;

	/* ovens #3 to #8 not implemented yet */
	if (noven >= 3 && noven <= 8)
	    return (0);
	sprintf (hostname, "oven%dv%d", noven, ncomp);
	if ((hp = gethostbyname (hostname)) == 0)
	    return (0);
	bcopy ((char *)hp->h_addr, (char *)&ipnumber,
	    MIN (hp->h_length, sizeof(ipnumber)));
	ip = ntohl (ipnumber);
	return (ip);
}

/* tportwrite - listen for connections and write data from buffer
 */
tportwrite (noven, ncomp, port, buf, nbytes)
int	noven;
int	ncomp;
int	port;
char	*buf[];
int	nbytes;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer;
	int	s, r;
	int	n, nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;
	int	iip;
	int	oven, comp;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (-1);

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	if (listen (s, 1) < 0)
	    return (-4);

	while (1) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    n = sizeof(rsockaddr);
	    if ((r = accept (s, (struct sockaddr *)&rsockaddr, &n)) < 0)
	        continue;
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    bcopy ((char *)&ip, (char *)&iip, sizeof(iip));
	    for (oven = 0; oven < noven; oven++) {
		for (comp = 0; comp < ncomp; comp++) {
		    if (iip == getovenip (oven, comp)) {
			if (buffer = buf[oven]) {
			    for (n = 0; n < nbytes; n += nxfer) {
				if ((nxfer = write(r, buffer+n,
				  MIN (MAXNBYTES, nbytes-n))) < 0)
				    break;
			    }
			}
			break;
		    }
		}
		if (comp < ncomp)
		    break;
	    }
	    close (r);
	}
}
