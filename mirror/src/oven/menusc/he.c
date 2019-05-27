#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

heido (s)
char	*s;
{
	int	t = 0;

	*s++ = '[';
	s += prloc (s, &Phe->loc);
	*s++ = ']';
	*s++ = '(';
	s += prpfe (s, Gpfe);
	*s++ = ')';
	s += prtemp (s, Dhe->htmp);
	*s++ = ' ';
	s += prpower (s, Dhe->heat);
	return (t);
}

heloi (s)
char	*s;
{
	int	t = 0;

	t = scloc (s, &Phe->loc);
	return (t);
}

heloo (s)
char	*s;
{
	int	t = 0;

	prloc (s, &Phe->loc);
	return (t);
}

herli (s)
char	*s;
{
	int	t = 0;

	t = sclocr (s, &Phe->loc);
	return (t);
}

herlo (s)
char	*s;
{
	int	t = 0;

	prlocr (s, &Phe->loc);
	return (t);
}

hetli (s)
char	*s;
{
	int	t = 0;

	t = scloct (s, &Phe->loc);
	return (t);
}

hetlo (s)
char	*s;
{
	int	t = 0;

	prloct (s, &Phe->loc);
	return (t);
}

hezli (s)
char	*s;
{
	int	t = 0;

	t = sclocz (s, &Phe->loc);
	return (t);
}

hezlo (s)
char	*s;
{
	int	t = 0;

	prlocz (s, &Phe->loc);
	return (t);
}

heado (s)
char	*s;
{
	int	t = 0;

	prpfe (s, Gpfe);
	return (t);
}

hevst ()
{
	int	t = 0;

	t = toggle (&Phe->vs_down);
	return (t);
}

hevso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Phe->vs_down);
	return (t);
}

hecst ()
{
	int	t = 0;

	t = toggle (&Phe->cs_down);
	return (t);
}

hecso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Phe->cs_down);
	return (t);
}

herst ()
{
	int	t = 0;

	t = toggle (&Phe->rs_down);
	return (t);
}

herso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Phe->rs_down);
	return (t);
}

hestt ()
{
	int	t = 0;

	t = toggle (&Phe->he_down);
	return (t);
}

hesto (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Phe->he_down);
	return (t);
}

heigt ()
{
	int	t = 0;

	t = toggle (&Phe->ignore);
	return (t);
}

heigo (s)
char	*s;
{
	int	t = 0;

	prignore (s, Phe->ignore);
	return (t);
}

hezog ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

hezoi (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Phe->zone);
	return (t);
}

hezoo (s)
char	*s;
{
	int	t = 0;

	przone (s, Phe->zone);
	return (t);
}

hebmi (s)
char	*s;
{
	int	t = 0;

	t = scbeam (s, &Phe->beam);
	return (t);
}

hebmo (s)
char	*s;
{
	int	t = 0;

	prbeam (s, Phe->beam);
	return (t);
}

hetwi (s)
char	*s;
{
	int	t = 0;

	t = sctwist (s, &Phe->twist);
	return (t);
}

hetwo (s)
char	*s;
{
	int	t = 0;

	prtwist (s, Phe->twist);
	return (t);
}

heert ()
{
	int	t = 0;

	t = toggle (&Phe->ck_down);
	return (t);
}

heero (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Phe->ck_down);
	return (t);
}

hezrg ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

hezro (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzoneh->he_ck_down);
	return (t);
}

hehto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dhe->htmp);
	return (t);
}

heetg ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

heeto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzoneh->etmp);
	return (t);
}

hepeo (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dhe->p_error);
	return (t);
}

heedg ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

heedo (s)
char	*s;
{
	int	t = 0;

	if (Pzoneh->n_node == 0)
	    return (t);
	prtemp (s, Pnodeh->deviation);
	return (t);
}

hetdg ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

hetdo (s)
char	*s;
{
	int	t = 0;

	if (Pzoneh->n_node == 0)
	    return (t);
	prtemp (s, Pnodeh->tdeviation);
	return (t);
}

heipg ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

heipo (s)
char	*s;
{
	int	t = 0;

	if (Pzoneh->n_node == 0)
	    return (t);
	pridepth (s, Pnodeh->i_depth);
	return (t);
}

hepig ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

hepio (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Dhe->p_error);
	*s++ = ' ';
	if (Pzoneh->n_node == 0)
	    s += prspace (s, 13);
	else
	    s += prcoef (s, Pnodeh->p_coef);
	*s++ = ' ';
	s += prcoef (s, Dhe->p_product);
	return (t);
}

heiig ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

heiio (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Dhe->i_error);
	*s++ = ' ';
	if (Pzoneh->n_node == 0)
	    s += prspace (s, 13);
	else
	    s += prcoef (s, Pnodeh->i_coef);
	*s++ = ' ';
	s += prcoef (s, Dhe->i_product);
	return (t);
}

hedig ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

hedio (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Dhe->d_error);
	*s++ = ' ';
	if (Pzoneh->n_node == 0)
	    s += prspace (s, 13);
	else
	    s += prcoef (s, Pnodeh->d_coef);
	*s++ = ' ';
	s += prcoef (s, Dhe->d_product);
	return (t);
}

heplg ()
{
	int	t = 0;

	t = menugoto ("zo");
	if (!t)
	    Gzone = Phe->zone;
	return (t);
}

heplo (s)
char	*s;
{
	int	t = 0;

	if (Pzoneh->n_node == 0)
	    return (t);
	prpower (s, Pnodeh->limit);
	return (t);
}

hehpo (s)
char	*s;
{
	int	t = 0;

	prpower (s, Dhe->heat);
	return (t);
}

hefeg ()
{
	int	t = 0;

	t = menugoto ("fs");
	return (t);
}

hefeo (s)
char	*s;
{
	int	t = 0;

	prepsilon (s, Pfase->epsilon);
	return (t);
}

hefmg ()
{
	int	t = 0;

	t = menugoto ("fs");
	return (t);
}

hefmo (s)
char	*s;
{
	int	t = 0;

	prmaxload (s, Pfase->max_load);
	return (t);
}

hepsg ()
{
	int	t = 0;

	t = menugoto ("pp");
	return (t);
}

hepso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ppanel->ss_down);
	return (t);
}

hehag ()
{
	int	t = 0;

	t = menugoto ("ad");
	return (t);
}

hehao (s)
char	*s;
{
	int	t = 0;
	da_fase	*dafase;

	if (Gpfe/(N_FASE*N_ELEMENT) >= N_PANEL-1)
	    return (t);
	dafase = dafase_pfe (Ddb, Gpfe);
	prapf (s, Gpfe, dafase->Voltage, dafase->Current, Dfase->calc_load);
	return (t);
}

henhg (n)
int	n;
{
	int	t = 0;

	if (n >= Phe->n_nneighbor)
	    return (GOER_NOWHERE);
	t = menugoto ("tc");
	if (!t)
	    Gdntx = Phe->nneighbor[n];
	return (t);
}

henhi (n, s)
int	n;
char	*s;
{
	int	tt, t = -1;

	if (n != Phe->n_nneighbor)
	    if (scdel (s))
		for (Phe->n_nneighbor--; n < Phe->n_nneighbor; n++)
		    Phe->nneighbor[n] = Phe->nneighbor[n+1];
	    else
	        t = sctcloc (s, Pdb, &Phe->nneighbor[n]);
	else
	    if (!(tt = sctcloc (s, Pdb, &Phe->nneighbor[n])))
		Phe->n_nneighbor = n+1;
	    else
		t = tt;
	return (t);
}

henho (n, s)
int	n;
char	*s;
{
	int	t = 0;

	if (n >= Phe->n_nneighbor)
	    return (t);
	*s++ = '[';
	s += prloc (s, &Ptcn(n)->loc);
	*s++ = ']';
	*s++ = '(';
	s += prdntx (s, Phe->nneighbor[n]);
	*s++ = ')';
	*s++ = ' ';
	s += prstat (s, Ptcn(n)->down);
	s += prign (s, Ptcn(n)->ignore);
	s += prdeviant (s, Dhe->ndeviant[n]);
	s += prtemp (s, Dtcn(n)->ttmp);
	return (t);
}

henn ()
{
	int	n = 0;

	n = MIN (Phe->n_nneighbor+1, N_NNEIGHBOR);
	return (n);
}

henfg (n)
int	n;
{
	int	t = 0;

	if (n >= Phe->n_fneighbor)
	    return (GOER_NOWHERE);
	t = menugoto ("tc");
	if (!t)
	    Gdntx = Phe->fneighbor[n];
	return (t);
}

henfi (n, s)
int	n;
char	*s;
{
	int	tt, t = -1;

	if (n != Phe->n_fneighbor)
	    if (scdel (s))
		for (Phe->n_fneighbor--; n < Phe->n_fneighbor; n++)
		    Phe->fneighbor[n] = Phe->fneighbor[n+1];
	    else
	        t = sctcloc (s, Pdb, &Phe->fneighbor[n]);
	else
	    if (!(tt = sctcloc (s, Pdb, &Phe->fneighbor[n])))
		Phe->n_fneighbor = n+1;
	    else
		t = tt;
	return (t);
}

henfo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	if (n >= Phe->n_fneighbor)
	    return (t);
	*s++ = '[';
	s += prloc (s, &Ptcf(n)->loc);
	*s++ = ']';
	*s++ = '(';
	s += prdntx (s, Phe->fneighbor[n]);
	*s++ = ')';
	*s++ = ' ';
	s += prstat (s, Ptcf(n)->down);
	s += prign (s, Ptcf(n)->ignore);
	s += prdeviant (s, Dhe->fdeviant[n]);
	s += prtemp (s, Dtcf(n)->ttmp);
	return (t);
}

hefn ()
{
	int	n = 0;

	n = MIN (Phe->n_fneighbor+1, N_FNEIGHBOR);
	return (n);
}
