#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

ztido (s)
char	*s;
{
	int	t = 0;

	s += prspace (s, 1);
	s += przone (s, Gzone);
	return (t);
}

ztfsg (n)
int	n;
{
	int	t = 0;

	t = menugoto ("fs");
	if (!t)
	    Gpfe = n*N_ELEMENT;
	return (t);
}

ztfso (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe = n*N_ELEMENT;
	p_element	*pelement = phe_pfe (Pdb, pfe);
	d_element	*delement = dhe_pfe (Ddb, pfe);
	int	element;

	s += prpfe (s, pfe);
	s += prspace (s, 1);
	for (element = 0; element < N_ELEMENT; element++,pelement++,delement++){
	    if (element == N_ELEMENT/2)
		s += prspace (s, 1);
	    if (pelement->zone == Gzone)
		s += prtempd (s, pelement->he_down, delement->htmp);
	    else
		s += prspace (s, 7);
	}

	return (t);
}

ztnf ()
{
	int	n = 0;

	n = N_PANEL*N_FASE;
	return (n);
}
