#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

sbido (s)
char	*s;
{
	int	t = 0;

	prdsb (s, Gdsb);
	return (t);
}

sbado (s)
char	*s;
{
	int	t = 0;

	prdsb (s, Gdsb);
	return (t);
}

sbstt ()
{
	int	t = 0;

	t = toggle (&Pbuss->down);
	return (t);
}

sbsto (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pbuss->down);
	return (t);
}

sbtcg (n)
int	n;
{
	int	t = 0;
	DNTX	dntx = dntx_dsb (Pdb, Gdsb, n/N_TTMP);

	if (dntx >= N_DCU*N_COUNTER*N_TIC*N_TC)
	    return (GOER_INCONSISTENT);
	dntx += n%N_TTMP;
	t = menugoto ("tc");
	if (!t)
	    Gdntx = dntx;
	return (t);
}

sbtct (n)
int	n;
{
	int	t = 0;
	DNTX	dntx = dntx_dsb (Pdb, Gdsb, n/N_TTMP);
	p_tc	*ptc;

	if (dntx >= N_DCU*N_COUNTER*N_TIC*N_TC)
	    return (SCER_INCONSISTENT);
	dntx += n%N_TTMP;
	ptc = ptc_dntx (Pdb, dntx);

	t = toggle (&ptc->down);
	return (t);
}

sbtco (n, s)
int	n;
char	*s;
{
	int	t = 0;
	DNTX	dntx = dntx_dsb (Pdb, Gdsb, n/N_TTMP);
	p_tc	*ptc;
	d_tc	*dtc;

	if (dntx >= N_DCU*N_COUNTER*N_TIC*N_TC)
	    return (t);
	dntx += n%N_TTMP;
	ptc = ptc_dntx (Pdb, dntx);
	dtc = dtc_dntx (Ddb, dntx);
	*s++ = '[';
	s += prloc (s, &ptc->loc);
	*s++ = ']';
	*s++ = '(';
	s += prdntx (s, dntx);
	*s++ = ')';
	*s++ = ' ';
	s += prstat (s, ptc->down);
	s += prign (s, ptc->ignore);
	s += prtemp (s, dtc->ttmp);

	return (t);
}

sbnt ()
{
	int	n = 0;

	n = MIN (N_POS, n_dntx_dsb (Pdb, Gdsb)) * N_TTMP;
	return (n);
}
