#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

tcido (s)
char	*s;
{
	int	t = 0;

	*s++ = '[';
	s += prloc (s, &Ptc->loc);
	*s++ = ']';
	*s++ = '(';
	s += prdntx (s, Gdntx);
	*s++ = ')';
	s += prtemp (s, Dtc->ttmp);
	return (t);
}

tcloi (s)
char	*s;
{
	int	t = 0;

	t = scloc (s, &Ptc->loc);
	return (t);
}

tcloo (s)
char	*s;
{
	int	t = 0;

	prloc (s, &Ptc->loc);
	return (t);
}

tcado (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

tctpo (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dtc->ttmp);
	return (t);
}

tcsni (s)
char	*s;
{
	int	t = 0;

	t = scserial (s, &Ptc->serial);
	return (t);
}

tcsno (s)
char	*s;
{
	int	t = 0;

	prserial (s, Ptc->serial);
	return (t);
}

tcofi (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Ptc->offset);
	return (t);
}

tcofo (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Ptc->offset);
	return (t);
}

tcstt ()
{
	int	t = 0;

	t = toggle (&Ptc->down);
	return (t);
}

tcsto (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ptc->down);
	return (t);
}

tcigt ()
{
	int	t = 0;

	t = toggle (&Ptc->ignore);
	return (t);
}

tcigo (s)
char	*s;
{
	int	t = 0;

	prignore (s, Ptc->ignore);
	return (t);
}

tctyg ()
{
	int	t = 0;

	t = menugoto ("ty");
	if (!t)
	    Gtctype = Ptc->tctype;
	return (t);
}

tctyi (s)
char	*s;
{
	int	t = 0;

	t = sctctype (s, &Ptc->tctype);
	return (t);
}

tctyo (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Ptc->tctype);
	return (t);
}

tctjg ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

tctjo (s)
char	*s;
{
	int	t = 0;

	prijb (s, Ptic->ijblock);
	return (t);
}

tctng ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

tctno (s)
char	*s;
{
	int	t = 0;

	prserial (s, Ptic->serial);
	return (t);
}

tctag ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

tctao (s)
char	*s;
{
	int	t = 0;

	prdnt0 (s, Gdntx);
	return (t);
}

tcttg ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

tctto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dtic->jtmp);
	return (t);
}

tctvg ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

tctvo (s)
char	*s;
{
	int	t = 0;

	prrefv (s, Ptic->reference);
	return (t);
}

tctsg ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

tctso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ptic->down);
	return (t);
}

tcsag ()
{
	int	t = 0;

	t = menugoto ("sb");
	if (!t)
	    Gdsb = Ptic->select;
	return (t);
}

tcsao (s)
char	*s;
{
	int	t = 0;

	prdsb (s, Ptic->select);
	return (t);
}

tcssg ()
{
	int	t = 0;

	t = menugoto ("sb");
	if (!t)
	    Gdsb = Ptic->select;
	return (t);
}

tcsso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pbusst->down);
	return (t);
}

tcdag ()
{
	int	t = 0;

	t = menugoto ("dc");
	return (t);
}

tcdao (s)
char	*s;
{
	int	t = 0;

	prd000 (s, Gdntx);
	return (t);
}

tcdsg ()
{
	int	t = 0;

	t = menugoto ("dc");
	return (t);
}

tcdso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pdcu->down);
	return (t);
}

tcnhg (n)
int	n;
{
	int	t = 0;
	PFE	pfe;

	if (n >= n_nneigh (Pdb, Gdntx))
	    return (GOER_NOWHERE);
	pfe = nneigh (Pdb, Gdntx, n);
	if (pfe >= N_PANEL*N_FASE*N_ELEMENT)
	    return (GOER_INCONSISTENT);
	t = menugoto ("he");
	if (!t)
	    Gpfe = pfe;
	return (t);
}

tcnhi (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe, pfe2;
	p_element *phe;
	int	m;
	BOOL	delok = (n != n_nneigh (Pdb, Gdntx));

	if (delok) {
	    pfe = nneigh (Pdb, Gdntx, n);
	    if (pfe >= N_PANEL*N_FASE*N_ELEMENT)
	        return (SCER_INCONSISTENT);
	    phe = phe_pfe (Pdb, pfe);
	    for (m = 0; m < phe->n_nneighbor; m++)
		if (phe->nneighbor[m] == Gdntx)
		    break;
	    if (m >= phe->n_nneighbor)
		return (SCER_INCONSISTENT);
	    if (scdel (s)) {
		for (phe->n_nneighbor--; m < phe->n_nneighbor; m++)
		    phe->nneighbor[m] = phe->nneighbor[m+1];
		return (SCER_REPAINT);
	    }
	}
	if (!(t = scheloc (s, Pdb, &pfe2))) {
	    if (delok && (pfe == pfe2)) {
		return (t);
	    } else {
		p_element *phe = phe_pfe (Pdb, pfe2);
		int	m = phe->n_nneighbor;

		if (m >= N_NNEIGHBOR)
		    return (SCER_RANGE);
		phe->nneighbor[m] = Gdntx;
		phe->n_nneighbor = m+1;
	    }
	    if (!delok)
		return (SCER_REPAINT);
	    for (phe->n_nneighbor--; m < phe->n_nneighbor; m++)
		phe->nneighbor[m] = phe->nneighbor[m+1];
	}
	return (t);
}

tcnho (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe;
	p_element *phe;
	d_element *dhe;

	if (n >= n_nneigh (Pdb, Gdntx))
	    return (t);
	pfe = nneigh (Pdb, Gdntx, n);
	if (pfe >= N_PANEL*N_FASE*N_ELEMENT)
	    return (t);
	phe = phe_pfe (Pdb, pfe);
	dhe = dhe_pfe (Ddb, pfe);
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

tcnn ()
{
	int	n = 0;

	n = n_nneigh (Pdb, Gdntx) + 1;
	return (n);
}

tcnfg (n)
int	n;
{
	int	t = 0;
	PFE	pfe;

	if (n >= n_fneigh (Pdb, Gdntx))
	    return (GOER_NOWHERE);
	pfe = fneigh (Pdb, Gdntx, n);
	if (pfe >= N_PANEL*N_FASE*N_ELEMENT)
	    return (GOER_INCONSISTENT);
	t = menugoto ("he");
	if (!t)
	    Gpfe = pfe;
	return (t);
}

tcnfi (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe, pfe2;
	p_element *phe;
	int	m;
	BOOL	delok = (n != n_fneigh (Pdb, Gdntx));

	if (delok) {
	    pfe = fneigh (Pdb, Gdntx, n);
	    if (pfe >= N_PANEL*N_FASE*N_ELEMENT)
	        return (SCER_INCONSISTENT);
	    phe = phe_pfe (Pdb, pfe);
	    for (m = 0; m < phe->n_fneighbor; m++)
		if (phe->fneighbor[m] == Gdntx)
		    break;
	    if (m >= phe->n_fneighbor)
		return (SCER_INCONSISTENT);
	    if (scdel (s)) {
		for (phe->n_fneighbor--; m < phe->n_fneighbor; m++)
		    phe->fneighbor[m] = phe->fneighbor[m+1];
		return (SCER_REPAINT);
	    }
	}
	if (!(t = scheloc (s, Pdb, &pfe2))) {
	    if (delok && (pfe == pfe2)) {
		return (t);
	    } else {
		p_element *phe = phe_pfe (Pdb, pfe2);
		int	m = phe->n_fneighbor;

		if (m >= N_FNEIGHBOR)
		    return (SCER_RANGE);
		phe->fneighbor[m] = Gdntx;
		phe->n_fneighbor = m+1;
	    }
	    if (!delok)
		return (SCER_REPAINT);
	    for (phe->n_fneighbor--; m < phe->n_fneighbor; m++)
		phe->fneighbor[m] = phe->fneighbor[m+1];
	}
	return (t);
}

tcnfo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe;
	p_element *phe;
	d_element *dhe;

	if (n >= n_fneigh (Pdb, Gdntx))
	    return (t);
	pfe = fneigh (Pdb, Gdntx, n);
	if (pfe >= N_PANEL*N_FASE*N_ELEMENT)
	    return (t);
	phe = phe_pfe (Pdb, pfe);
	dhe = dhe_pfe (Ddb, pfe);
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

tcfn ()
{
	int	n = 0;

	n = n_fneigh (Pdb, Gdntx) + 1;
	return (n);
}
