#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

zoido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prtemp (s, Dzone->etmp);
	return (t);
}

zoadg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zoado (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

zostt ()
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = toggle (&Bzone->clock);
	return (t);
}

zosto (s)
char	*s;
{
	int	t = 0;

	prclstatus (s, Bzone->clock);
	return (t);
}

zocti (s)
char	*s;
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = sctime (s, &Bzone->time);
	return (t);
}

zocto (s)
char	*s;
{
	int	t = 0;

	prtime (s, Bzone->time);
	return (t);
}

zoetg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zoeto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->etmp);
	return (t);
}

zozto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->ztmp);
	return (t);
}

zodto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->zdiff);
	return (t);
}

zoerg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zoero (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->delta*3600.0);
	return (t);
}

zozso (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->zsct);
	return (t);
}

zoedg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zoedo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtemp (s, Pnode->deviation);
	return (t);
}

zotdg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zotdo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtemp (s, Pnode->tdeviation);
	return (t);
}

zosct ()
{
	int	t = 0;

	t = toggle (&Pzone->ck_down);
	return (t);
}

zosco (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->ck_down);
	return (t);
}

zohrt ()
{
	int	t = 0;

	t = toggle (&Pzone->he_ck_down);
	return (t);
}

zohro (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->he_ck_down);
	return (t);
}

zozpo (s)
char	*s;
{
	int	t = 0;

	prpower (s, Dzone->zpower);
	return (t);
}

zonng ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonno (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnoden (s, Dzone->c_node);
	return (t);
}

zonlg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonlo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodel (s, Pnode->nlabel);
	return (t);
}

zonpg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonpo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->p_coef);
	return (t);
}

zonig ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonio (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->i_coef);
	return (t);
}

zondg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zondo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->d_coef);
	return (t);
}

zonhg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonho (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	pridepth (s, Pnode->i_depth);
	return (t);
}

zonqg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonqo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prpower (s, Pnode->limit);
	return (t);
}

zonwg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonwo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodew (s, Pnode);
	return (t);
}

zoncg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonco (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtemp (s, Pnode->temp);
	return (t);
}

zongo (s)
char	*s;
{
	int	t = 0;

	prtime (s, Dzone->togo);
	return (t);
}

zontg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

zonto (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtime (s, Pnode->time);
	return (t);
}

zonro (s)
char	*s;
{
	int	t = 0;

	prrtime (s, Dzone->togo);
	return (t);
}
