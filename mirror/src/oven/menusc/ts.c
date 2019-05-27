#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

tsido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	s += prspace (s, 8);
	s += prtemp (s, Dzone->etmp);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	return (t);
}

tszai (s)
char	*s;
{
	int	tt, t = -1;
	int	zone;

	if (!(tt = sczone (s, &zone))) {
	    Pzone->n_node = Pzonex(zone)->n_node;
	    bcopy ((char *)Pzonex(zone)->node, (char *)Pzone->node,
	        sizeof(Pzone->node));
	} else {
	    t = tt;
	}
	return (t);
}

tszao (s)
char	*s;
{
	int	t = 0;

	return (t);
}

tsaig (n)
int	n;
{
	int	t = 0;

	if (n >= Pzone->n_node)
	    return (GOER_NOWHERE);
	t = menugoto ("no");
	if (!t)
	    Gnode = n;
	return (t);
}

tsaii (n, s)
int	n;
char	*s;
{
	int	t = -1;
	int	m;

	if (Pzone->n_node < N_NODE && scins (s))
	    for (m = Pzone->n_node++; m >= n; m--)
	        if (m > 0)
	            bcopy((char *)Pnodex(m-1),(char *)Pnodex(m),sizeof(p_node));
		else
		    bzero ((char *)Pnodex(m), sizeof(p_node));
	else if (n == Pzone->n_node)
	    return (SCER_SYNTAX);
	else if (scdel (s))
	    for (Pzone->n_node--, m = n; m < Pzone->n_node; m++)
		bcopy ((char *)Pnodex(m+1), (char *)Pnodex(m), sizeof(p_node));
	else
	    t = sctemp (s, &Pnodex(n)->temp);
	return (t);
}

tsaio (n, s)
int	n;
char	*s;
{
	int	t = 0;
	TEMP	dtemp;
	TIME	dtime;

	if (n >= Pzone->n_node)
	    return (t);
	s += prnodel (s, Pnodex(n)->nlabel);
	*s++ = Dzone->c_node == n ? '*' : ' ';

	dtemp = Pnodex(n)->temp - Pnodex(n-1)->temp;
	dtime = Pnodex(n)->time - Pnodex(n-1)->time;

	s += prtemp (s, dtime ? (float)dtemp/(float)dtime*3600.0 : 0.0);
	s += prtemp (s, Pnodex(n)->deviation);
	s += prtemp (s, Pnodex(n)->tdeviation);
	s += prtemp (s, Pnodex(n)->temp);
	*s++ = ' ';
	s += prtime (s, Pnodex(n)->time);
	*s++ = ' ';
	s += prtemp (s, dtemp);
	*s++ = ' ';
	s += prtime (s, dtime);
	*s++ = ' ';
	s += prnodew (s, Pnodex(n));
	return (t);
}

tsna ()
{
	int	n = 0;

	n = MIN (Pzone->n_node+1, N_NODE);
	return (n);
}

tszbi (s)
char	*s;
{
	int	t = 0;

	t = tszai (s);
	return (t);
}

tszbo (s)
char	*s;
{
	int	t = 0;

	t = tszao (s);
	return (t);
}

tsbig (n)
int	n;
{
	int	t = 0;

	t = tsaig (n);
	return (t);
}

tsbii (n, s)
int	n;
char	*s;
{
	int	t = -1;
	int	m;

	if (Pzone->n_node < N_NODE && scins (s))
	    for (m = Pzone->n_node++; m >= n; m--)
	        if (m > 0)
	            bcopy((char *)Pnodex(m-1),(char *)Pnodex(m),sizeof(p_node));
		else
		    bzero ((char *)Pnodex(m), sizeof(p_node));
	else if (n == Pzone->n_node)
	    return (SCER_SYNTAX);
	else if (scdel (s))
	    for (Pzone->n_node--, m = n; m < Pzone->n_node; m++)
		bcopy ((char *)Pnodex(m+1), (char *)Pnodex(m), sizeof(p_node));
	else
	    t = scpower (s, &Pnodex(n)->limit);
	return (t);
}

tsbio (n, s)
int	n;
char	*s;
{
	int	t = 0;

	if (n >= Pzone->n_node)
	    return (t);
	s += prnodel (s, Pnodex(n)->nlabel);
	*s++ = Dzone->c_node == n ? '*' : ' ';
	*s++ = ' ';
	s += prcoef (s, Pnodex(n)->p_coef);
	s += prspace (s, 2);
	s += prcoef (s, Pnodex(n)->i_coef);
	s += prspace (s, 2);
	s += prcoef (s, Pnodex(n)->d_coef);
	s += prspace (s, 5);
	s += pridepth (s, Pnodex(n)->i_depth);
	s += prspace (s, 3);
	s += prpower (s, Pnodex(n)->limit);
	return (t);
}

tsnb ()
{
	int	n = 0;

	n = tsna ();
	return (n);
}
