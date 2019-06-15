#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

rzido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prsped (s, Dzone->etmp);
	return (t);
}

rzadg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rzado (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

rzstt ()
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = toggle (&Bzone->clock);
	return (t);
}

rzsto (s)
char	*s;
{
	int	t = 0;

	prclstatus (s, Bzone->clock);
	return (t);
}

rzcti (s)
char	*s;
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = sctime (s, &Bzone->time);
	return (t);
}

rzcto (s)
char	*s;
{
	int	t = 0;

	prtime (s, Bzone->time);
	return (t);
}

rzetg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rzeto (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->etmp);
	return (t);
}

rzzto (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->ztmp);
	return (t);
}

rzdto (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->zdiff);
	return (t);
}

rzerg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rzero (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->delta*3600.0);
	return (t);
}

rzzso (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->zsct);
	return (t);
}

rzedg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rzedo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prsped (s, Pnode->deviation);
	return (t);
}

rzsct ()
{
	int	t = 0;

	t = toggle (&Pzone->ck_down);
	return (t);
}

rzsco (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->ck_down);
	return (t);
}

rzhrt ()
{
	int	t = 0;

	t = toggle (&Pzone->he_ck_down);
	return (t);
}

rzhro (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->he_ck_down);
	return (t);
}

rzzpo (s)
char	*s;
{
	int	t = 0;

	prpower (s, Dzone->zpower);
	return (t);
}

rznng ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznno (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnoden (s, Dzone->c_node);
	return (t);
}

rznlg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznlo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodel (s, Pnode->nlabel);
	return (t);
}

rznpg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznpo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->p_coef);
	return (t);
}

rznig ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznio (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->i_coef);
	return (t);
}

rzndg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rzndo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->d_coef);
	return (t);
}

rznhg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznho (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	pridepth (s, Pnode->i_depth);
	return (t);
}

rznqg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznqo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prpower (s, Pnode->limit);
	return (t);
}

rznwg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznwo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodew (s, Pnode);
	return (t);
}

rzncg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznco (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prsped (s, Pnode->temp);
	return (t);
}

rzngo (s)
char	*s;
{
	int	t = 0;

	prtime (s, Dzone->togo);
	return (t);
}

rzntg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

rznto (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtime (s, Pnode->time);
	return (t);
}

rznro (s)
char	*s;
{
	int	t = 0;

	prrtime (s, Dzone->togo);
	return (t);
}

rzaso (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prsped (s, Ddb->adc[ncomp].rztmp);
	}
	return (t);
}

rza0o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[0]);
	}
	return (t);
}

rza1o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[1]);
	}
	return (t);
}

rza2o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[2]);
	}
	return (t);
}

rza3o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[3]);
	}
	return (t);
}

rza4o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[4]);
	}
	return (t);
}

rza5o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[5]);
	}
	return (t);
}

rza6o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[6]);
	}
	return (t);
}

rza7o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[7]);
	}
	return (t);
}

rza8o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += pramps (s, Ddb->adc[ncomp].r[8]);
	}
	return (t);
}

rza9o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prpercent (s, Ddb->adc[ncomp].r[9]);
	}
	return (t);
}

rzaao (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prpercent (s, Ddb->adc[ncomp].r[10]);
	}
	return (t);
}

rzabo (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prpercent (s, Ddb->adc[ncomp].r[11]);
	}
	return (t);
}

rzlvo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp;

	s += prspace (s, 4);
	s += prnnb (s, n);
	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prtemp (s, Ddb->adc[ncomp].r[O_LVDT+n]);
	}

	return (t);
}

rzla ()
{
	int	n = 0;

	n = N_LVDT;
	return (n);
}

rzloi (n, s)
int	n;
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pdb->misc.lvdt[n]);
	return (t);
}

rzloo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	s += prspace (s, 4);
	s += prnnb (s, n);
	s += prspace (s, 3);
	s += prtemp (s, Pdb->misc.lvdt[n]);
	return (t);
}

rzlb ()
{
	int	n = 0;

	n = rzla ();
	return (n);
}
