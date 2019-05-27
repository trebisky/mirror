#define	UTIL
#include "oven.h"

/* address to pointer utility routines
 */

p_buss	*
pbuss_dsb (pdb, addr)
p_database	*pdb;
DSB		addr;
{
	int	b = addr%N_BUSS;
	int	s = addr/N_BUSS%N_SELECT;
	int	d = addr/N_BUSS/N_SELECT%N_DCU;

	return (&pdb->dcu[d].select[s].buss[b]);
}

i_buss	*
ibuss_dsb (idb, addr)
i_database	*idb;
DSB		addr;
{
	int	b = addr%N_BUSS;
	int	s = addr/N_BUSS%N_SELECT;
	int	d = addr/N_BUSS/N_SELECT%N_DCU;

	return (&idb->dcu[d].select[s].buss[b]);
}

p_tc	*
ptc_dntx (pdb, addr)
p_database	*pdb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&pdb->dcu[d].counter[n].tic[t].tc[x]);
}

i_tc	*
itc_dntx (idb, addr)
i_database	*idb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&idb->dcu[d].counter[n].tic[t].tc[x]);
}

d_tc	*
dtc_dntx (ddb, addr)
d_database	*ddb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&ddb->dcu[d].counter[n].tic[t].tc[x]);
}

p_tic	*
ptic_dntx (pdb, addr)
p_database	*pdb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&pdb->dcu[d].counter[n].tic[t]);
}

i_tic	*
itic_dntx (idb, addr)
i_database	*idb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&idb->dcu[d].counter[n].tic[t]);
}

d_tic	*
dtic_dntx (ddb, addr)
d_database	*ddb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&ddb->dcu[d].counter[n].tic[t]);
}

p_dcu	*
pdcu_dntx (pdb, addr)
p_database	*pdb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&pdb->dcu[d]);
}

i_dcu	*
idcu_dntx (idb, addr)
i_database	*idb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&idb->dcu[d]);
}

d_dcu	*
ddcu_dntx (ddb, addr)
d_database	*ddb;
DNTX		addr;
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	return (&ddb->dcu[d]);
}

p_element	*
phe_pfe (pdb, addr)
p_database	*pdb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&pdb->panel[p].fase[f].element[e]);
}

i_element	*
ihe_pfe (idb, addr)
i_database	*idb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&idb->panel[p].fase[f].element[e]);
}

d_element	*
dhe_pfe (ddb, addr)
d_database	*ddb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&ddb->panel[p].fase[f].element[e]);
}

p_fase	*
pfase_pfe (pdb, addr)
p_database	*pdb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&pdb->panel[p].fase[f]);
}

pa_fase	*
pafase_pfe (pdb, addr)
p_database	*pdb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = (addr/N_ELEMENT/N_FASE%N_PANEL)%(N_PANEL/N_COMP);
	int	c = (addr/N_ELEMENT/N_FASE%N_PANEL)/(N_PANEL/N_COMP);

	return (&pdb->adc[c].panel[p].fase[f]);
}

i_fase	*
ifase_pfe (idb, addr)
i_database	*idb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&idb->panel[p].fase[f]);
}

d_fase	*
dfase_pfe (ddb, addr)
d_database	*ddb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&ddb->panel[p].fase[f]);
}

da_fase	*
dafase_pfe (ddb, addr)
d_database	*ddb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = (addr/N_ELEMENT/N_FASE%N_PANEL)%(N_PANEL/N_COMP);
	int	c = (addr/N_ELEMENT/N_FASE%N_PANEL)/(N_PANEL/N_COMP);

	return (&ddb->adc[c].panel[p].fase[f]);
}

p_panel	*
ppanel_pfe (pdb, addr)
p_database	*pdb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&pdb->panel[p]);
}

pa_panel	*
papanel_pfe (pdb, addr)
p_database	*pdb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = (addr/N_ELEMENT/N_FASE%N_PANEL)%(N_PANEL/N_COMP);
	int	c = (addr/N_ELEMENT/N_FASE%N_PANEL)/(N_PANEL/N_COMP);

	return (&pdb->adc[c].panel[p]);
}

i_panel	*
ipanel_pfe (idb, addr)
i_database	*idb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&idb->panel[p]);
}

d_panel	*
dpanel_pfe (ddb, addr)
d_database	*ddb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	return (&ddb->panel[p]);
}

da_panel	*
dapanel_pfe (ddb, addr)
d_database	*ddb;
PFE		addr;
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = (addr/N_ELEMENT/N_FASE%N_PANEL)%(N_PANEL/N_COMP);
	int	c = (addr/N_ELEMENT/N_FASE%N_PANEL)/(N_PANEL/N_COMP);

	return (&ddb->adc[c].panel[p]);
}

n_nneigh (pdb, dntx)
p_database	*pdb;
DNTX		dntx;
{
	PFE	pfe;
	p_element *phe;
	int	n;
	int	number = 0;

	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe++) {
	    phe = phe_pfe (pdb, pfe);
	    for (n = 0; n < phe->n_nneighbor; n++) {
		if (phe->nneighbor[n] == dntx)
		    number++;
	    }
	}
	return (number);
}

PFE
nneigh (pdb, dntx, num)
p_database	*pdb;
DNTX		dntx;
int	num;
{
	PFE	pfe;
	p_element *phe;
	int	n;
	int	number = 0;

	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe++) {
	    phe = phe_pfe (pdb, pfe);
	    for (n = 0; n < phe->n_nneighbor; n++) {
		if (phe->nneighbor[n] == dntx)
		    if (number++ == num)
		        return (pfe);
	    }
	}
	return (pfe);
}

n_fneigh (pdb, dntx)
p_database	*pdb;
DNTX		dntx;
{
	PFE	pfe;
	p_element *phe;
	int	n;
	int	number = 0;

	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe++) {
	    phe = phe_pfe (pdb, pfe);
	    for (n = 0; n < phe->n_fneighbor; n++) {
		if (phe->fneighbor[n] == dntx)
		    number++;
	    }
	}
	return (number);
}

PFE
fneigh (pdb, dntx, num)
p_database	*pdb;
DNTX		dntx;
int	num;
{
	PFE	pfe;
	p_element *phe;
	int	n;
	int	number = 0;

	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe++) {
	    phe = phe_pfe (pdb, pfe);
	    for (n = 0; n < phe->n_fneighbor; n++) {
		if (phe->fneighbor[n] == dntx)
		    if (number++ == num)
		        return (pfe);
	    }
	}
	return (pfe);
}

DNTX
adjtic (pdb, addr)
p_database	*pdb;
DNTX		addr;
{
	DNTX	dntx;
	int	ijblock = ptic_dntx (pdb, addr)->ijblock;

	for (dntx = 0; dntx < N_DCU*N_COUNTER*N_TIC*N_TC; dntx += N_TC)
	    if (dntx == addr)
		continue;
	    else if (ptic_dntx (pdb, dntx)->ijblock == ijblock)
		break;
	return (dntx);
}

TEMP
adjtmp (pdb, ddb, addr)
p_database	*pdb;
d_database	*ddb;
DNTX		addr;
{
	DNTX	dntx = adjtic (pdb, addr);

	if (dntx >= N_DCU*N_COUNTER*N_TIC*N_TC)
	    return (INDEFT);
	else
	    return (dtic_dntx (ddb, dntx)->jtmp);
}

n_dntx_dsb (pdb, select)
p_database	*pdb;
DSB		select;
{
	DNTX	dntx;
	int	number = 0;

	for (dntx = 0; dntx < N_DCU*N_COUNTER*N_TIC*N_TC; dntx += N_TC)
	    if (ptic_dntx (pdb, dntx)->select == select)
		number++;
	return (number);
}

DNTX
dntx_dsb (pdb, select, num)
p_database	*pdb;
DSB		select;
int	num;
{
	DNTX	dntx;
	int	number = 0;

	for (dntx = 0; dntx < N_DCU*N_COUNTER*N_TIC*N_TC; dntx += N_TC)
	    if (ptic_dntx (pdb, dntx)->select == select)
		if (number++ == num)
		    break;
	return (dntx);
}

n_schedule (pdb)
p_database	*pdb;
{
	int	zone;
	int	number = 0;

	for (zone = 0; zone < N_ZONE; zone++)
	    if (pdb->zone[zone].n_node)
		number++;
	return (number);
}

nth_schedule (pdb, num)
p_database	*pdb;
int	num;
{
	int	zone;
	int	number = 0;

	for (zone = 0; zone < N_ZONE; zone++)
	    if (pdb->zone[zone].n_node)
		if (number++ == num)
		    break;
	return (zone);
}
