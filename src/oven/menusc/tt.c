#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

tttig (n)
int	n;
{
	int	t = 0;

	t = menugoto ("ti");
	if (!t)
	    Gdntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	return (t);
}

tttio (n, s)
int	n;
char	*s;
{
	int	t = 0;
	DNTX	dntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	p_tc	*ptc = ptc_dntx (Pdb, dntx);
	d_tc	*dtc = dtc_dntx (Ddb, dntx);
	int	tc;

	s += prdntx (s, dntx);
	for (tc = 0; tc < N_TTMP; tc++, ptc++, dtc++)
	    s += prtempd (s, ptc->down, dtc->ttmp);
	return (t);
}

ttnt ()
{
	int	n = 0;

	n = N_COUNTER*N_TIC;
	return (n);
}
