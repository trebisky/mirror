#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

msvct (n)
int	n;
{
	int	t = 0;

	t = toggle (&Pdb->misc.vup[n]);
	return (t);
}

msvco (n, s)
int	n;
char	*s;
{
	int	t = 0;

	s += prvnumb (s, n+1);
	s += prspace (s, 1);
	s += prenable (s, Pdb->misc.vup[n]);
	return (t);
}

msvn ()
{
	int	n = 0;

	n = N_COMP;
	return (n);
}

msjti (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pdb->misc.djtmp);
	return (t);
}

msjto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Pdb->misc.djtmp);
	return (t);
}

msxjt ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.xjtmp);
	return (t);
}

msxjo (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.xjtmp);
	return (t);
}

msxat ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.xadc);
	return (t);
}

msxao (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.xadc);
	return (t);
}

mscft ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.config);
	return (t);
}

mscfo (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.config);
	return (t);
}

mslrt ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.logrotv);
	return (t);
}

mslro (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.logrotv);
	return (t);
}

msrpi (n, s)
int	n;
char	*s;
{
	int	t = 0;

	t = scvportp (s, &Pdb->misc.rport[n]);
	return (t);
}

msrpo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	s += prvnumb (s, n+1);
	s += prspace (s, 1);
	s += prvport (s, Pdb->misc.rport[n]);
	return (t);
}

msvr ()
{
	int	n = 0;

	n = N_COMP;
	return (n);
}
