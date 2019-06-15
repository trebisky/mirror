#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

jttig (n)
int	n;
{
	int	t = 0;

	t = menugoto ("ti");
	if (!t)
	    Gdntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	return (t);
}

jttio (n, s)
int	n;
char	*s;
{
	int	t = 0;
	DNTX	dntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	DNTX	Dntx = adjtic (Pdb, dntx);
	p_tic	*ptic = ptic_dntx (Pdb, dntx);
	p_tic	*qtic = ptic_dntx (Pdb, Dntx);
	d_tic	*dtic = dtic_dntx (Ddb, dntx);
	TEMP	jtmp = adjtmp (Pdb, Ddb, dntx);
	TEMP	diff;

	s += prdntx (s, dntx);
	s += prtempd (s, ptic->down, dtic->jtmp);
	s += prspace (s, 9);
	s += prdntx (s, Dntx);
	s += prtempd (s, qtic->down, jtmp);
	s += prspace (s, 1);
	if (IS_INDEFT (dtic->jtmp) || IS_INDEFT (jtmp))
	    diff = INDEFT;
	else
	    diff = dtic->jtmp - jtmp;
	s += prtemp (s, diff);
	*s++ = (!(IS_INDEFT (diff))&&(ABS(diff) > Pdb->misc.djtmp)) ? '!' : ' ';
	*s = 0;
	return (t);
}

jtnt ()
{
	int	n = 0;

	n = N_COUNTER*N_TIC;
	return (n);
}
