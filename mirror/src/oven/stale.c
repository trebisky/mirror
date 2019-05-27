#include "oven.h"

/* stalejtmp - stale junction temperatures
 */
stalejtmp (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	DCL_DCU
	long	now;
	BOOL	allstale;

	time (&now);
	allstale = (now - ddb->misc.uclock) > 120;

	time (&ddb->misc.uclock);
	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    if (allstale == NO && IS_STALE (ddcu->timestamp) == NO)
		continue;

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {
		    p_tc	*ptc = ptic->tc;
		    i_tc	*itc = itic->tc;
		    d_tc	*dtc = dtic->tc;
		    int		tc;

		    dtic->jtmp = INDEFT;
		    for (tc = 0; tc < N_TC; ptc++, itc++, dtc++, tc++) {
			if (tc >= N_TTMP)
			    continue;
			dtc->ttmp = INDEFT;
		    }
		}
	    }
	}
	if (allstale)
	    staleall (db);
}

/* staleadc - stale ADC values
 */
staleadc (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	int	panel;
	int	nrot;

	time (&ddb->misc.uclock);
	for (panel = 0; panel < N_PANEL-1; panel++) {

	    int	fase;

	    if (!(panel%N_COMP)) {
		STALEADC (voltage)
		for (nrot = 0; nrot < N_ROT; nrot++) {
		    STALEADC (r[nrot])
		}
		STALEADC (rztmp)
	    }

	    STALEADC (panel[panel%N_COMP].current)

	    for (fase = 0; fase < N_FASE; fase++) {
		STALEADC (panel[panel%N_COMP].fase[fase].current)
		STALEADC (panel[panel%N_COMP].fase[fase].voltage)
	    }
	}
}

/* staleall - stale panels and zones
 */
staleall (db)
database	*db;
{
	DCL_PANEL
	DCL_ZONE

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

		    delement->htmp = INDEFT;
		    delement->heat = 0;
		}
	    }
	}

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node + dzone->c_node;

	    if (zone == ROT_ZONE)
		continue;
	    dzone->zdiff = INDEFT;
	    dzone->zpower = 0;
	    dzone->ztmp = INDEFT;
	    dzone->zsct = INDEFT;
	}
}
