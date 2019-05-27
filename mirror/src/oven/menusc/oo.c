#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

ooido (s)
char	*s;
{
	int	t = 0;

	sprintf (s, "oven%dv%d ", globalp->noven, globalp->ncomp);
	s += strlen (s);
	s += prrtime (s, 0);
	return (t);
}

ooaig (n)
int	n;
{
	int	t = 0;
	int	zone = nth_schedule (Pdb, n);

	if (zone >= N_ZONE)
	    return (GOER_NOWHERE);
	if (zone == ROT_ZONE)
	    t = menugoto ("rz");
	else
	    t = menugoto ("zo");
	if (!t)
	    Gzone = zone;
	return (t);
}

ooaio (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	zone = nth_schedule (Pdb, n);
	b_zone	*bzone;
	p_zone	*pzone;
	d_zone	*dzone;
	p_node	*pnode;

	if (zone >= N_ZONE)
	    return (t);
	bzone = Bdb->zone + zone;
	pzone = Pdb->zone + zone;
	dzone = Ddb->zone + zone;
	pnode = &pzone->node[MIN (pzone->n_node-1, dzone->c_node)];

	s += prspace (s, 1);
	s += przone (s, zone);
	s += prspace (s, 3);
	s += prclstat (s, bzone->clock);
	s += prspace (s, 4);
	s += prtime (s, bzone->time);
	if (zone != ROT_ZONE) {
	    s += prspace (s, 3);
	    s += prtemp (s, dzone->etmp);
	    s += prspace (s, 3);
	    s += prtemp (s, dzone->ztmp);
	    s += prspace (s, 3);
	    s += prtemp (s, dzone->zdiff);
	    s += prspace (s, 3);
	    s += prtemp (s, dzone->delta*3600.0);
	    s += prspace (s, 3);
	    s += prtemp (s, dzone->zsct);
	    s += prspace (s, 3);
	    s += prtemp (s, pnode->deviation);
	} else {
	    s += prspace (s, 3);
	    s += prsped (s, dzone->etmp);
	    s += prspace (s, 3);
	    s += prsped (s, dzone->ztmp);
	    s += prspace (s, 3);
	    s += prsped (s, dzone->zdiff);
	    s += prspace (s, 3);
	    s += prsped (s, dzone->delta*3600.0);
	    s += prspace (s, 3);
	    s += prspace (s, 7);
	    s += prspace (s, 3);
	    s += prsped (s, pnode->deviation);
	}

	return (t);
}

oona ()
{
	int	n = 0;

	n = n_schedule (Pdb);
	return (n);
}

oobig (n)
int	n;
{
	int	t = 0;

	t = ooaig (n);
	return (t);
}

oobio (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	zone = nth_schedule (Pdb, n);
	b_zone	*bzone;
	p_zone	*pzone;
	d_zone	*dzone;
	p_node	*pnode;

	if (zone >= N_ZONE)
	    return (t);
	bzone = Bdb->zone + zone;
	pzone = Pdb->zone + zone;
	dzone = Ddb->zone + zone;
	pnode = &pzone->node[MIN (pzone->n_node-1, dzone->c_node)];

	s += prspace (s, 1);
	s += przone (s, zone);
	s += prspace (s, 1);
	s += prnodel (s, pnode->nlabel);
	s += prspace (s, 3);
	s += prstat (s, pzone->ck_down);
	if (zone != ROT_ZONE) {
	    s += prspace (s, 9);
	    s += prpower (s, dzone->zpower);
	    s += prspace (s, 3);
	    s += prtemp (s, pnode->temp);
	} else {
	    s += prspace (s, 9);
	    s += prspace (s, 3);
	    s += prspace (s, 3);
	    s += prsped (s, pnode->temp);
	}
	s += prspace (s, 2);
	s += prtime (s, dzone->togo);
	s += prspace (s, 3);
	s += prtime (s, pnode->time);
	s += prspace (s, 3);
	s += prrtime (s, dzone->togo);

	return (t);
}

oonb ()
{
	int	n = 0;

	n = oona ();
	return (n);
}

ooaao (s)
char	*s;
{
	int	t = 0;

	adaao ((adna () - N_COMP) - 1, s);
	return (t);
}
