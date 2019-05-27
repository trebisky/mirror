#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"

/* datalogger - do data logging
 */
datalogger ()
{
	datalog (globalp->db);
}

/* datalog - do data logging on this database
 */
datalog (db)
database	*db;
{
	panellog (db);
	dculog (db);
	zonelog (db);
	adclog (db);
}

/* adclog - do adc data logging
 */
adclog (db)
database	*db;
{
	float	adcv[N_COMP*(1+N_PANEL/N_COMP*(1+N_FASE*2))];
	float	rotv[N_COMP*(N_ROT+1)];
	float	*ad = adcv;
	float	*ro = rotv;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL-1; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    d_adc	*adc = db->data.adc  + panel/N_COMP;
	    da_panel	*apanel = adc->panel + panel%N_COMP;
	    da_fase	*afase = apanel->fase;
	    int		rot;

	    for (fase = 0; fase < N_FASE; FASE__, afase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		*ad++ = afase->current;
		*ad++ = afase->voltage;
	    }
	    *ad++ = apanel->current;
	    if (panel%N_COMP == N_PANEL/N_COMP-1) {
		*ad++ = adc->voltage;
		for (rot = 0; rot < N_ROT; rot++)
		    *ro++ = adc->r[rot];
		*ro++ = adc->rztmp;
	    }
	}
	logdata ("adcv", adcv, sizeof(adcv)/sizeof(adcv[0]));
	if (db->parameter.misc.logrotv)
	    logdata ("rotv", rotv, sizeof(rotv)/sizeof(rotv[0]));
}

/* dculog - do dcu data logging
 */
dculog (db)
database	*db;
{
	float	jtmp[N_DCU*N_COUNTER*N_TIC];
	float	ttmp[N_DCU*N_COUNTER*N_TIC*N_TTMP];
	float	*jt = jtmp;
	float	*tt = ttmp;
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC)*N_TC;
		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC;
		    p_tc	*ptc = ptic->tc;
		    i_tc	*itc = itic->tc;
		    d_tc	*dtc = dtic->tc;
		    int		tc;

		    *jt++ = pdcu->down || ptic->down ? INDEFT : dtic->jtmp;
		    for (tc = 0; tc < N_TC; ptc++, itc++, dtc++, tc++) {

			DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC+tc;

			if (tc >= N_TTMP)
			    continue;
			*tt++ = pdcu->down || ptic->down || ptc->down
			  ? INDEFT : dtc->ttmp;
		    }
		}
	    }
	}
	logdata ("jtmp", jtmp, sizeof(jtmp)/sizeof(jtmp[0]));
	logdata ("ttmp", ttmp, sizeof(ttmp)/sizeof(ttmp[0]));
}

/* panellog - do panel data logging
 */
panellog (db)
database	*db;
{
	float	htmp[N_PANEL*N_FASE*N_ELEMENT];
	float	hpwr[N_PANEL*N_FASE*N_ELEMENT];
	float	*ht = htmp;
	float	*hp = hpwr;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    *ht++ = pelement->he_down ? INDEFT : delement->htmp;
		    *hp++ = pelement->he_down ?      0 : delement->heat;
		}
	    }
	}
	logdata ("htmp", htmp, sizeof(htmp)/sizeof(htmp[0]));
	logdata ("hpwr", hpwr, sizeof(hpwr)/sizeof(hpwr[0]));
}

/* zonelog - do zone data logging
 */
zonelog (db)
database	*db;
{
	float	ztmp[N_ZONE];
	float	zsct[N_ZONE];
	float	etmp[N_ZONE];
	float	*zt = ztmp;
	float	*zs = zsct;
	float	*et = etmp;
	DCL_ZONE

	for (zone = 0; zone < N_ZONE; ZONE__) {
	    *zt++ = dzone->ztmp;
	    *zs++ = dzone->zsct;
	    *et++ = dzone->etmp;
	}
	logdata ("ztmp", ztmp, sizeof(ztmp)/sizeof(ztmp[0]));
	logdata ("zsct", zsct, sizeof(zsct)/sizeof(zsct[0]));
	logdata ("etmp", etmp, sizeof(etmp)/sizeof(etmp[0]));
}
