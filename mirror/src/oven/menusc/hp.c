#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

hpfsg (n)
int	n;
{
	int	t = 0;

	t = menugoto ("fs");
	if (!t)
	    Gpfe = n*N_ELEMENT;
	return (t);
}

hpfso (n, s)
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
	    s += prspace (s, 4);
	    s += prpowerd (s, pelement->he_down, delement->heat);
	}

	return (t);
}

hpnf ()
{
	int	n = 0;

	n = (N_PANEL-1)*N_FASE;
	return (n);
}
