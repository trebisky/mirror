#define	TOUNIXTIME(x)	((x) - 2208988800)
#ifdef	VXWORKS
#include <vxWorks.h>
#include <socket.h>
#include <in.h>
#include <systime.h>
#define	IPPORT_TIMESERVER	37
extern	char	sysBootHost[];
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

/* getutime - get unix time from iptime
 */
long
getutime (t)
unsigned long	t;
{
	long	u = TOUNIXTIME (t);

	return (u);
}

/* getiptime - get the time of day from a remote server
 */
unsigned long
getiptime (hostname)
char	*hostname;
{
	struct	sockaddr_in	sockaddr;
	int	s;
	unsigned long	t;
	char	*buffer;
	int	nbytes, nread;
#ifndef	VXWORKS
	struct	hostent	*hp;
#endif
#ifdef	VXWORKS
	struct	timeval	timeout;
#endif
	int	ip;

#ifdef	VXWORKS
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
#endif
	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (IPPORT_TIMESERVER);

#ifdef	VXWORKS
	if (hostname)
	    ip = hostGetByName (hostname);
	else
	    ip = hostGetByName (sysBootHost);
	if (ip == ERROR)
	    return (0);
	if (ip == getmyinetaddr())
	    return (0);
	sockaddr.sin_addr.s_addr = ip;
#else
	if ((hp = gethostbyname (hostname)) == 0)
	    return (0);
	bcopy ((char *)hp->h_addr, (char *)&sockaddr.sin_addr, hp->h_length);
#endif

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (0);

#ifndef	VXWORKS
	if (connect (s, &sockaddr, sizeof (sockaddr)) < 0) {
#else
	if (connectWithTimeout (s, &sockaddr, sizeof (sockaddr), &timeout)<0) {
#endif
	    close (s);
	    return (0);
	}
	buffer = (char *)&t;
	nbytes = sizeof(t);
	while (nbytes) {
	    nread = read (s, buffer, nbytes);
	    if (nread < 0) {
		close (s);
		return (0);
	    }
	    nbytes -= nread;
	    buffer += nread;
	}
	close (s);
	return (ntohl(t));
}
