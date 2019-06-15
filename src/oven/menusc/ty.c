#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

tyido (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Gtctype);
	return (t);
}

tytyo (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Gtctype);
	return (t);
}

tymti (n, s)
int	n;
char	*s;
{
	int	t = 0;

	scpoly (s, &Ptctype->mt_polycoef[n]);
	return (t);
}

tymto (n, s)
int	n;
char	*s;
{
	int	t = 0;

	prpoly (s, Ptctype->mt_polycoef[n]);
	return (t);
}

tynm ()
{
	int	n = 0;

	n = N_POLYCOEF;
	return (n);
}

tycvi (n, s)
int	n;
char	*s;
{
	int	t = 0;

	scpoly (s, &Ptctype->cv_polycoef[n]);
	return (t);
}

tycvo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	prpoly (s, Ptctype->cv_polycoef[n]);
	return (t);
}

tync ()
{
	int	n = 0;

	n = N_POLYCOEF;
	return (n);
}

tyjti (n, s)
int	n;
char	*s;
{
	int	t = 0;

	scpoly (s, &Ptctype->jt_polycoef[n]);
	return (t);
}

tyjto (n, s)
int	n;
char	*s;
{
	int	t = 0;

	prpoly (s, Ptctype->jt_polycoef[n]);
	return (t);
}

tynj ()
{
	int	n = 0;

	n = N_POLYCOEF;
	return (n);
}
