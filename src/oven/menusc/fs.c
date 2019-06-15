#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

fsido (s)
char	*s;
{
	int	t = 0;

	prpf0 (s, Gpfe);
	return (t);
}

fsado (s)
char	*s;
{
	int	t = 0;

	prpf0 (s, Gpfe);
	return (t);
}

fsfei (s)
char	*s;
{
	int	t = 0;

	t = scepsilon (s, &Pfase->epsilon);
	return (t);
}

fsfeo (s)
char	*s;
{
	int	t = 0;

	prepsilon (s, Pfase->epsilon);
	return (t);
}

fsfmi (s)
char	*s;
{
	int	t = 0;

	t = scmaxload (s, &Pfase->max_load);
	return (t);
}

fsfmo (s)
char	*s;
{
	int	t = 0;

	prmaxload (s, Pfase->max_load);
	return (t);
}

fspsg ()
{
	int	t = 0;

	t = menugoto ("pp");
	return (t);
}

fspso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ppanel->ss_down);
	return (t);
}

fsfag ()
{
	int	t = 0;

	t = menugoto ("ad");
	return (t);
}

fsfao (s)
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

fsheg (n)
int	n;
{
	int	t = 0;

	t = menugoto ("he");
	if (!t)
	    Gpfe = Gpfe - Gpfe%N_ELEMENT + n;
	return (t);
}

fshet (n)
int	n;
{
	int	t = 0;
	PFE	pfe = Gpfe - Gpfe%N_ELEMENT + n;
	p_element	*phe = phe_pfe (Pdb, pfe);

	t = toggle (&phe->he_down);
	return (t);
}

fsheo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe = Gpfe - Gpfe%N_ELEMENT + n;
	p_element	*phe = phe_pfe (Pdb, pfe);
	d_element	*dhe = dhe_pfe (Ddb, pfe);

	*s++ = '[';
	s += prloc (s, &phe->loc);
	*s++ = ']';
	*s++ = '(';
	s += prpfe (s, pfe);
	*s++ = ')';
	*s++ = ' ';
	s += prstat (s, phe->he_down);
	s += prign (s, phe->ignore);
	s += prtemp (s, dhe->htmp);
	*s++ = ' ';
	s += prpower (s, dhe->heat);
	return (t);
}

fsne ()
{
	int	n = 0;

	n = N_ELEMENT;
	return (n);
}
