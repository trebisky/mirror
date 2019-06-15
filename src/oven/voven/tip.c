#include <taskLib.h>
#include <ioLib.h>

static	int	runtipout;

tip (port)
int	port;
{
	int	fd = 0;		/* STDIN */
	int	options;
	int	tipout();
	char	c[2];

	c[1] = 0;
	runtipout = 1;
	taskSpawn ("tTipOut", 202, VX_STDIO, 1000, tipout, port);
	options = ioctl (fd, FIOGETOPTIONS);
	ioctl (fd, FIOSETOPTIONS, OPT_RAW);
	while ((read (fd, c, 1) == 1) && ((c[0] & 0177) != 4))
	    writepanel (port, c);
	ioctl (fd, FIOSETOPTIONS, options);
	runtipout = 0;
	taskDelete (taskNameToId ("tTipOut"));
}

tipout (port)
int	port;
{
	char	string[80];

	while (runtipout) {
	    string[0] = 0;
	    readpanel (port, string);
	    if (strlen (string) < 1)
		taskDelay (sysClkRateGet()/10);
	    else
		printf ("%s", string);
	}
}
