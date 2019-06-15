#include "oven.h"

/* cwork - control work for each tick
 */
cwork (db)
database	*db;
{
	int	cclock;

	forever {
	    cwait ();
	    cclock = db->intermediate.misc.cclock;
	    if (cclock == N_PANEL-1)
		break;
	}
	forever {
	    cwait ();
	    cclock = db->intermediate.misc.cclock;
							/*  adc?	*/
	    if (cclock == 0) {
		scalc	(db);		/* calculate schedule		*/
		savedatabaseb ();
		wrrdata	(db);		/* write rotation data		*/
	    }
	    hcalc	(db, cclock);	/* calculate heater power	*/
	    pcalc	(db, cclock);	/* calculate panel ssr maps	*/
	    if (!CONFIG)
		hcalc2	(db, cclock);	/* recalculate heater power	*/
	    if (cclock == N_PANEL-3) {
		ckjtmp  (db);		/* check junction temperatures	*/
		adcfire ();
	    }
	    if (cclock == N_PANEL-2) {
		ckadc  (db);		/* check ADC values		*/
	    }
	    if (cclock == N_PANEL-1) {
		zcalc	(db);		/* calculate zone values	*/
		kcalc	(db);		/* calculate clock status	*/
		savedatabaseb ();
	    }
	}
}

/* scalc - calculate schedule
 */
scalc (db)
database	*db;
{
	DCL_ZONE

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node;
	    p_node	*Pnode;			/* previous node	*/
	    int		node;

	    if (pzone->n_node == 0)
		continue;			/* no schedule		*/

	    if (bzone->clock)
		bzone->time += PERIODS;

	    /* find current node
	     */
	    for (node = 0; node < pzone->n_node; NODE__)
		if (pnode->time >= bzone->time)
		    break;
	    if (node >= pzone->n_node) {
		pnode--; node--;
		bzone->time = pnode->time;	/* stay within sched.	*/
	    }
	    dzone->c_node = node;

	    /* find previous node
	     */
	    Pnode = (node) ? pnode-1 : pnode;

	    /* calculate rate of change of temperature with respect to time
	     */
	    if (pnode->time == Pnode->time) {
		dzone->delta = 0;
	    } else {			/* cmath? - should be safe	*/
		dzone->delta = (float)(pnode->temp - Pnode->temp)
			     / (float)(pnode->time - Pnode->time);
	    }
	    dzone->togo = pnode->time - bzone->time;

	    /* calculate scheduled temperature
	     */				/* cmath? - should be safe	*/
	    dzone->etmp = pnode->temp - (float)dzone->togo * dzone->delta;
	    if (!(IS_INDEFT (dzone->ztmp)))
		dzone->zdiff  = dzone->etmp - dzone->ztmp;
	}
}

/* kcalc - calculate clock status
 */
kcalc (db)
database	*db;
{
	DCL_ZONE

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node + dzone->c_node;
	    b_zone	*Bzone = bzone;

	    if (pzone->n_node == 0)
		continue;			/* no schedule		*/

	    if (bzone->time != pnode->time)	/* close ?		*/
		continue;

	    if (pnode->wflag == 0)
		continue;			/* nothing to wait on	*/

	    if (pnode->wflag & WFLAG_T) {
		if (IS_INDEFT (dzone->zdiff)) {
		    bzone->clock = NO;
		    continue;
		} else if (ABS (dzone->zdiff) > WFLAG_TOLERANCE) {
		    bzone->clock = NO;
		    continue;
		} else {
		    ;
		}
	    }
	    if (pnode->wflag & WFLAG_D) {
		if (IS_INDEFT (dzone->zsct)) {
		    bzone->clock = NO;
		    continue;
		} else if (dzone->zsct > pnode->deviation) {
		    bzone->clock = NO;
		    continue;
		} else {
		    ;
		}
	    }
	    if (pnode->wflag & WFLAG_W) {

		DCL_ZONE
		int	n_zone;
		p_node	*Pnode = pnode;
		BOOL	wait	= NO;

		if (pnode->wzone == N_ZONE) {
		    zone = 0;			/* wait on all zones	*/
		    n_zone = N_ZONE;
		} else {
		    zone = pnode->wzone;	/* wait on this zone	*/
		    n_zone = zone+1;
		}
		bzone += zone;
		pzone += zone;
		izone += zone;
		dzone += zone;

		for (; zone < n_zone; ZONE__) {

		    int	node = dzone->c_node+1;	/* start in the future	*/
		    p_node *pnode = pzone->node + node;

		    if (pzone->n_node == 0)
			continue;		/* no schedule		*/

		    /* check if label is defined in the future of this zone
		     */
		    for (; node < pzone->n_node; NODE__) {
			if (pnode->nlabel == Pnode->wlabel) {
			    wait = YES;
			}
		    }
		    if (wait)
			break;
		}
		if (wait) {
		    Bzone->clock = NO;
		    continue;
		}
	    }
	    bzone->clock = YES;
	}
}

/* hcalc - calculate heater temperatures and heater powers
 */
hcalc (db, panel)
database	*db;
int	panel;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	p_panel	*ppanel = pdb->panel + panel;
	i_panel	*ipanel = idb->panel + panel;
	d_panel	*dpanel = ddb->panel + panel;

	{

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT
		float		calc_load = 0;
		long		low_load = 0;

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    b_zone *bzone = db->biparameter.zone + pelement->zone;
		    p_zone *pzone = db->parameter.zone + pelement->zone;
		    i_zone *izone = db->intermediate.zone + pelement->zone;
		    d_zone *dzone = db->data.zone + pelement->zone;
		    p_node *pnode = NULL;
		    TEMP scatter;
		    register int nb;

		    if (pzone->n_node)
			pnode = pzone->node + dzone->c_node;

		    /* calculate heater temperature
		     */
		    if (pelement->he_down) {
			for (nb = 0; nb < N_NNEIGHBOR; nb++)
			    delement->ndeviant[nb] = NO;
			for (nb = 0; nb < N_FNEIGHBOR; nb++)
			    delement->fdeviant[nb] = NO;
			delement->htmp = INDEFT;
			delement->heat = 0;
#ifdef	OLD_CONFIG
		    } else if (CONFIG) {
			delement->heat = 1;
#endif
		    } else {

			/* insert all good neighbors into sorted list
			 */
			register TEMP	s;
			TEMP	t[N_NNEIGHBOR+N_FNEIGHBOR];
			int	w[N_NNEIGHBOR+N_FNEIGHBOR];
			BOOL	*p[N_NNEIGHBOR+N_FNEIGHBOR];
			register int	nneighbor = 0;
			register int	n;

			for (nb = 0; nb < pelement->n_nneighbor; nb++) {

			    p_tc *ptc = ptc_dntx (pdb, pelement->nneighbor[nb]);
			    d_tc *dtc = dtc_dntx (ddb, pelement->nneighbor[nb]);
			    d_dcu *ddcu=ddcu_dntx(ddb, pelement->nneighbor[nb]);

			    if (ptc->ignore || IS_INDEFT (dtc->ttmp))
				continue;
			    if (IS_STALE (ddcu->timestamp))
				continue;

			    s = dtc->ttmp;
			    for (n = nneighbor++; n != 0; n--) {
				if (t[n-1] > s) {
				    t[n] = t[n-1];
				    w[n] = w[n-1];
				    p[n] = p[n-1];
				} else {
				    break;
				}
			    }
			    t[n] = s;
			    w[n] = NNEIGHBOR_WEIGHT;
			    p[n] = delement->ndeviant+nb;
			}
			for (nb = 0; nb < pelement->n_fneighbor; nb++) {

			    p_tc *ptc = ptc_dntx (pdb, pelement->fneighbor[nb]);
			    d_tc *dtc = dtc_dntx (ddb, pelement->fneighbor[nb]);
			    d_dcu *ddcu=ddcu_dntx(ddb, pelement->fneighbor[nb]);

			    if (ptc->ignore || IS_INDEFT (dtc->ttmp))
				continue;
			    if (IS_STALE (ddcu->timestamp))
				continue;

			    s = dtc->ttmp;
			    for (n = nneighbor++; n != 0; n--) {
				if (t[n-1] > s) {
				    t[n] = t[n-1];
				    w[n] = w[n-1];
				    p[n] = p[n-1];
				} else {
				    break;
				}
			    }
			    t[n] = s;
			    w[n] = FNEIGHBOR_WEIGHT;
			    p[n] = delement->fdeviant+nb;
			}

			/* throw away extreme temperatures that are more than
			 * half a deviation from the median.  Then calculate
			 * a weighted mean of the remaining temperatures.
			 */
			if (nneighbor == 0) {
			    for (nb = 0; nb < N_NNEIGHBOR; nb++)
				delement->ndeviant[nb] = NO;
			    for (nb = 0; nb < N_FNEIGHBOR; nb++)
				delement->fdeviant[nb] = NO;
			    delement->htmp = INDEFT;
			} else {

			    TEMP median = t[nneighbor/2];
			    TEMP deviation2 = 0;
			    register TEMP sum = 0;
			    register int  weight = 0;

			    scatter = t[nneighbor-1] - t[0];
			    if (pnode)
				deviation2 = pnode->tdeviation/2;
			    for (n = 0; n < nneighbor; n++) {
				if (pnode == NULL)
				    *(p[n]) = NO;
				else if (ABS (t[n] - median) > deviation2)
				    *(p[n]) = YES;
				else
				    *(p[n]) = NO;
				if (*(p[n]) == NO) {
				    weight += w[n];
				    sum    += w[n] * t[n];
				}
			    }
			    delement->htmp = weight ? sum/weight : INDEFT;
			}
		    }

		    /* now calculate the heater power from the temperature
		     */
#ifdef	OLD_CONFIG
		    if (CONFIG) {
			;
		    } else if (IS_INDEFT (delement->htmp) || pelement->ignore
		      || !pnode) {
#else
		    if (IS_INDEFT (delement->htmp) || pelement->ignore
		      || !pnode) {
#endif
			delement->p_error	= 0;
			delement->i_error	= 0;
			delement->d_error	= 0;
			delement->l_error	= 0;
			delement->p_product	= 0;
			delement->i_product	= 0;
			delement->d_product	= 0;
			delement->heat		= 0;
		    } else {

			float	heat;

			if (pnode->i_depth == 0)
			    pnode->i_depth = 1;

			/* FPE ?? - cmath */

			delement->p_error= dzone->etmp - delement->htmp;
			delement->d_error= delement->p_error-delement->l_error;
			delement->l_error= delement->p_error;
			delement->i_error= (delement->i_error*(pnode->i_depth-1)
					 +  delement->p_error)/pnode->i_depth;
			delement->p_product = delement->p_error * pnode->p_coef;
			delement->i_product = delement->i_error * pnode->i_coef;
			delement->d_product = delement->d_error * pnode->d_coef;
			heat	= delement->p_product
				+ delement->i_product
				+ delement->d_product
				+ 0.5;
			if (heat < 0)
			    heat = 0;
			if (heat > MAX_POWER)
			    heat = MAX_POWER;
			if (heat > pnode->limit)
			    heat = pnode->limit;
			if (panel != N_PANEL-1)
			    delement->heat = (POWER)heat;
			else
			    delement->heat = 0;

			/* do errors checks
			 */
			if (!pelement->ck_down && !pzone->he_ck_down) {
			    if (ABS (delement->p_error) > pnode->deviation) {
				CERRHER;
			    }
			    if (scatter                 > pnode->tdeviation) {
				CERRHSC;
			    }
			}
		    }
		    calc_load += delement->heat;
		    low_load += delement->heat;
		    ielement->left = delement->heat;
		}
		dfase->calc_load = calc_load / MAX_POWER;
		ifase->low_load = low_load / MAX_POWER;
		ifase->nhigh = low_load % MAX_POWER;
	    }
	}
}

/* hcalc2 - recalculate heater temperatures and heater powers
 */
hcalc2 (db, panel)
database	*db;
int	panel;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	p_panel	*ppanel = pdb->panel + panel;
	i_panel	*ipanel = idb->panel + panel;
	d_panel	*dpanel = ddb->panel + panel;

	{

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT
		float		calc_load = 0;

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    b_zone *bzone = db->biparameter.zone + pelement->zone;
		    p_zone *pzone = db->parameter.zone + pelement->zone;
		    i_zone *izone = db->intermediate.zone + pelement->zone;
		    d_zone *dzone = db->data.zone + pelement->zone;
		    p_node *pnode = NULL;

		    if (pzone->n_node)
			pnode = pzone->node + dzone->c_node;

		    /* adjust hpwr request of maxload left any unsatisfied
		     */
		    delement->heat -= ielement->left;
		    calc_load += delement->heat;
		}
		dfase->calc_load = calc_load / MAX_POWER;
	    }
	}
}

/* zcalc - calculate zone temperatures and zone powers
 */
zcalc (db)
database	*db;
{
	DCL_PANEL
	DCL_ZONE

	for (zone = 0; zone < N_ZONE; ZONE__) {
	    izone->count = 0;
	    izone->tsum  = 0;
	    izone->hsum  = 0;
	}

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    b_zone *bzone = db->biparameter.zone + pelement->zone;
		    p_zone *pzone = db->parameter.zone + pelement->zone;
		    i_zone *izone = db->intermediate.zone + pelement->zone;
		    d_zone *dzone = db->data.zone + pelement->zone;

		    if (IS_INDEFT(delement->htmp))
			continue;
		    if (pelement->ignore || pzone->n_node == 0)
			continue;

		    if (izone->count++) {
			izone->tmin = MIN (izone->tmin, delement->htmp);
			izone->tmax = MAX (izone->tmax, delement->htmp);
		    } else {
			izone->tmin = delement->htmp;
			izone->tmax = delement->htmp;
		    }
		    izone->tsum += delement->htmp;
		    izone->hsum += delement->heat;
		}
	    }
	}

	bzone	= db->biparameter.zone;
	pzone	= db->parameter.zone;
	izone	= db->intermediate.zone;
	dzone	= db->data.zone;

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node + dzone->c_node;

	    if (zone == ROT_ZONE) {
		if (IS_INDEFT (dzone->ztmp))
		    dzone->zdiff  = INDEFT;
		else
		    dzone->zdiff  = dzone->etmp - dzone->ztmp;
		if (pzone->ck_down || IS_INDEFT (dzone->zdiff))
		    continue;	/* check zone scatter ? */
		if (pzone->n_node == 0 || ABS(dzone->zdiff) <= pnode->deviation)
		    continue;	/* within deviation ? */
		if (bzone->time == pnode->time && (pnode->wflag & WFLAG_D) != 0)
		    continue;	/* waiting for deviation ? */
		CERRZSC;
		continue;
	    }
	    if (izone->count) {
		dzone->zpower = izone->hsum / izone->count + 0.5;
		dzone->ztmp   = izone->tsum / izone->count;
		dzone->zdiff  = dzone->etmp - dzone->ztmp;
		dzone->zsct   = izone->tmax - izone->tmin;
	    } else {
		dzone->zdiff = INDEFT;
		dzone->zpower = 0;
		dzone->ztmp = INDEFT;
		dzone->zsct = INDEFT;
	    }
	    if (pzone->ck_down || IS_INDEFT (dzone->zsct))
		continue;	/* check zone scatter ? */
	    if (pzone->n_node == 0 || dzone->zsct <= pnode->deviation)
		continue;	/* within deviation ? */
	    if (bzone->time == pnode->time && (pnode->wflag & WFLAG_D) != 0)
		continue;	/* waiting for deviation ? */
	    CERRZSC;
	}
}

/* radc - read ADC values
 */
radc (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	int	panel;
	int	p0, pn;
	int	p;
	int	rot;

	p0 = getmycompnumber();
	p0 = p0 < 0 ? 0 : p0;
	p0 *= N_PANEL/N_COMP;
	pn = p0+N_PANEL/N_COMP;
	for (panel = p0; panel < pn; panel++) {

	    int	fase;

	    p = panel-p0;
	    if (!(panel%N_COMP)) {
		RDADC (25, voltage)
		for (rot = 0; rot < N_ROT; rot++) {
		    RDADC (32+rot, r[rot])
		    if (rot < O_LVDT || rot >= O_LVDT+N_LVDT)
			continue;
		    if (IS_INDEFA (ddb->adc[panel/N_COMP].r[rot]))
			continue;
		    ddb->adc[panel/N_COMP].r[rot] += pdb->misc.lvdt[rot-O_LVDT];
		}
	    }

	    RDADC (3+4*p, panel[panel%N_COMP].current)

	    for (fase = 0; fase < N_FASE; fase++) {

		RDADC (fase+4*p, panel[panel%N_COMP].fase[fase].current)
		 
		RDADC (16+fase+3*p, panel[panel%N_COMP].fase[fase].voltage)

	    }
	}
}

/* ckadc - check ADC values
 */
ckadc (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	int	panel;

	if (!pdb->misc.xadc)
	    return;

	for (panel = 0; panel < N_PANEL-1; panel++) {

	    int	fase;

	    if (!(panel%N_COMP)) {
		CKADC (voltage)
	    }

	    CKADC (panel[panel%N_COMP].current)

	    for (fase = 0; fase < N_FASE; fase++) {

		CKADC (panel[panel%N_COMP].fase[fase].current)

		CKADC (panel[panel%N_COMP].fase[fase].voltage)

	    }
	}
}

/* adcbroadcast - broadcast all my lastest ADC values
 */
adcbroadcast (db)
database	*db;
{
	forever {
	    p_database	*pdb = &db->parameter;
	    i_database	*idb = &db->intermediate;
	    d_database	*ddb = &db->data;
	    int	comp = getmycompnumber ();
	    int	adc = comp;

	    adcwait ();

/*	    collect adc values here */
	    ddb->adc[adc].timestamp = ddb->misc.uclock;
	    radc (db);
	    rdrdata (db);
	    ddb->adc[adc].rztmp = ddb->zone[ROT_ZONE].ztmp;

	    for (adc = 0; adc < N_COMP; adc++) {
		if (adc != comp)
		    continue;
		uportbroadcast (PORTADC0+adc, (char *)&ddb->adc[adc],
		  sizeof(ddb->adc[adc]));
		taskDelay (2);
	    }
	}
}
