#include <fcntl.h>
#include <sgtty.h>
#include <setjmp.h>
#include <signal.h>

typedef	int	(*PFI)();

jmp_buf	keybuf;

/* automkey - get a menu key with timeout
 */
automkey (period, offset)
int	period;
int	offset;
{
#ifdef	OLD_NBUKEY
	int	delay;
	long	now;
	PFI	oldsig;
	int	status;
	int	input;
	int	fd;
	char	cinput;
	struct	sgttyb	ttystat;
	short	sg_flags;
	void	keyalarm ();

	time (&now);
	delay = period - now%period + offset;
	if ((fd = open ("/dev/tty", O_RDONLY)) < 0)
	    return (-1);
	if (ioctl (fd, TIOCGETP, &ttystat) == -1) {
	    close (fd);
	    return (-1);
	}
	sg_flags = ttystat.sg_flags;
	ttystat.sg_flags &= ~ECHO;
	ttystat.sg_flags |=  CBREAK;
	if (ioctl (fd, TIOCSETP, &ttystat) == -1) {
	    close (fd);
	    return (-1);
	}
	oldsig = (PFI)signal (SIGALRM, keyalarm);
	status = setjmp (keybuf);
	if (status) {
	    input = 0;
	} else {
	    alarm (delay);
	    if (read (fd, &cinput, 1) != 1)
		cinput = 0;
	    input = cinput;
	}
	alarm (0);
	signal (SIGALRM, oldsig);
	ttystat.sg_flags = sg_flags;
	ioctl (fd, TIOCSETP, &ttystat);
	close (fd);
	return (input);
#else
	return ('q');
#endif
}

void
keyalarm (sig)
int	sig;
{
	longjmp (keybuf, 1);
}
