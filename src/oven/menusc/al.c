#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

alvui (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp, panel, fase;
	PFE	pfe;
	pa_fase	*pafase;

	if ((ncomp = (n - ((N_FASE)*(N_PANEL-1)))) >= 0) {
	    t = scvolts (s, &Pdb->adc[ncomp].voltage.upper);
	} else {
	    panel = n/(N_FASE);
	    fase  = n%(N_FASE);
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    t = scvolts (s, &pafase->voltage.upper);
	}
	return (t);
}

alvuo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp, panel, fase;
	PFE	pfe;
	pa_fase	*pafase;
	da_fase	*dafase;

	if ((ncomp = (n - ((N_FASE)*(N_PANEL-1)))) >= 0) {
	    s += prahv (s, ncomp, Ddb->adc[ncomp].Voltage,
	      Pdb->adc[ncomp].voltage.upper);
	} else {
	    panel = n/(N_FASE);
	    fase  = n%(N_FASE);
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    dafase = dafase_pfe (Ddb, pfe);
	    s += prapfv (s, pfe, dafase->Voltage, pafase->voltage.upper);
	}
	return (t);
}

aluv ()
{
	int	n = 0;

	n = N_FASE;
	n *= (N_PANEL-1);
	n += N_COMP;		/* + housekeeping		*/
	return (n);
}

alvli (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp, panel, fase;
	PFE	pfe;
	pa_fase	*pafase;

	if ((ncomp = (n - ((N_FASE)*(N_PANEL-1)))) >= 0) {
	    t = scvolts (s, &Pdb->adc[ncomp].voltage.lower);
	} else {
	    panel = n/(N_FASE);
	    fase  = n%(N_FASE);
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    t = scvolts (s, &pafase->voltage.lower);
	}
	return (t);
}

alvlo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp, panel, fase;
	PFE	pfe;
	pa_fase	*pafase;
	da_fase	*dafase;

	if ((ncomp = (n - ((N_FASE)*(N_PANEL-1)))) >= 0) {
	    s += prahv (s, ncomp, Ddb->adc[ncomp].Voltage,
	      Pdb->adc[ncomp].voltage.lower);
	} else {
	    panel = n/(N_FASE);
	    fase  = n%(N_FASE);
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    dafase = dafase_pfe (Ddb, pfe);
	    s += prapfv (s, pfe, dafase->Voltage, pafase->voltage.lower);
	}
	return (t);
}

allv ()
{
	int	n = 0;

	n = aluv ();
	return (n);
}

alcui (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	panel, fase;
	PFE	pfe;
	pa_fase	*pafase;
	pa_panel *papanel;

	panel = n/(N_FASE+1);
	fase  = n%(N_FASE+1);
	if (fase == N_FASE) {
	    pfe = panel*(N_FASE*N_ELEMENT);
	    papanel = papanel_pfe (Pdb, pfe);
	    t = scamps (s, &papanel->current.upper);
	} else {
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    t = scamps (s, &pafase->current.upper);
	}
	return (t);
}

alcuo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	panel, fase;
	PFE	pfe;
	pa_fase	*pafase;
	da_fase	*dafase;
	pa_panel *papanel;
	da_panel *dapanel;

	panel = n/(N_FASE+1);
	fase  = n%(N_FASE+1);
	if (fase == N_FASE) {
	    pfe = panel*(N_FASE*N_ELEMENT);
	    papanel = papanel_pfe (Pdb, pfe);
	    dapanel = dapanel_pfe (Ddb, pfe);
	    s += prapnc (s, pfe, dapanel->Current, papanel->current.upper);
	} else {
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    dafase = dafase_pfe (Ddb, pfe);
	    s += prapfc (s, pfe, dafase->Current, pafase->current.upper);
	}
	return (t);
}

aluc ()
{
	int	n = 0;

	n = N_FASE;
	n += 1;			/* + neutral			*/
	n *= (N_PANEL-1);
	return (n);
}

alcli (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	panel, fase;
	PFE	pfe;
	pa_fase	*pafase;
	pa_panel *papanel;

	panel = n/(N_FASE+1);
	fase  = n%(N_FASE+1);
	if (fase == N_FASE) {
	    pfe = panel*(N_FASE*N_ELEMENT);
	    papanel = papanel_pfe (Pdb, pfe);
	    t = scamps (s, &papanel->current.lower);
	} else {
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    t = scamps (s, &pafase->current.lower);
	}
	return (t);
}

alclo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	panel, fase;
	PFE	pfe;
	pa_fase	*pafase;
	da_fase	*dafase;
	pa_panel *papanel;
	da_panel *dapanel;

	panel = n/(N_FASE+1);
	fase  = n%(N_FASE+1);
	if (fase == N_FASE) {
	    pfe = panel*(N_FASE*N_ELEMENT);
	    papanel = papanel_pfe (Pdb, pfe);
	    dapanel = dapanel_pfe (Ddb, pfe);
	    s += prapnc (s, pfe, dapanel->Current, papanel->current.lower);
	} else {
	    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
	    pafase = pafase_pfe (Pdb, pfe);
	    dafase = dafase_pfe (Ddb, pfe);
	    s += prapfc (s, pfe, dafase->Current, pafase->current.lower);
	}
	return (t);
}

allc ()
{
	int	n = 0;

	n = aluc ();
	return (n);
}
