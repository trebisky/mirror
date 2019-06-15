#include "oven.h"

#define	M_DCU	1
#define	M_PANEL	1
#define	M_ZONE	1
#define	M_NODE	2
#define	M_FNEIGHBOR	5

extern	database	*dbs;

/* toinit - do test oven initialization
 */
toinit ()
{
	dbs->parameter.misc.vup[0] = YES;
	ttinit (dbs);
	tpinit (dbs);
	tcinit (dbs);
}

/* ttinit - do temperature initialization
 */
ttinit (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	DCL_DCU
	p_tctype	*ptctype;

	for (dcu = 0; dcu < M_DCU; DCU__) {

	    DCL_SELECT
	    DCL_COUNTER

	    pdcu->connect = db->parameter.misc.vnumber+1;

	    for (select = 0; select < N_SELECT; SELECT__) {

		DCL_BUSS

		for (buss = 0; buss < N_BUSS; BUSS__) {
		    pbuss->down = YES;
		}
	    }
	    pdcu->select[0].buss[0].down = NO;
	    pdcu->select[0].buss[1].down = NO;

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {
		    ptic->reference = 50.000;
		    ptic->tctype = TCTYPEZ;
		    ptic->select = ((dcu*N_COUNTER+counter)*N_TIC+tic)/N_POS;
		}
	    }
	}
	ptctype = pdb->tctype;
	ptctype->mt_polycoef[0] = 1.25625;
	ptctype->mt_polycoef[1] = 37.3586;
	ptctype->mt_polycoef[2] = -0.774667;
	ptctype->mt_polycoef[3] = 0.025625;
	ptctype->mt_polycoef[4] = -4.326627e-04;
	ptctype->mt_polycoef[5] = 3.019461e-06;
	ptctype->cv_polycoef[0] = 0.0;
	ptctype->cv_polycoef[1] = 2.589943e-02;
	ptctype->cv_polycoef[2] = 1.645333e-05;
	ptctype->cv_polycoef[3] = 3.617621e-08;
	ptctype->cv_polycoef[4] = -1.822097e-10;
	ptctype->cv_polycoef[5] = 0.0;
	ptctype = pdb->tctype + TCTYPEZ;
	ptctype->jt_polycoef[0] = 24.8629;
	ptctype->jt_polycoef[1] = -0.80145;
	ptctype->jt_polycoef[2] = 3.498299e-03;
	ptctype->jt_polycoef[3] = -7.310954e-05;
	ptctype->jt_polycoef[4] = -3.320249e-06;
	ptctype->jt_polycoef[5] = -1.244616e-07;
}

/* tcinit - do control initialization
 */
tcinit (db)
database	*db;
{
	DCL_ZONE

	for (zone = 0; zone < M_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node;
	    int		node;

	    pzone->n_node = M_NODE;
	    bzone->time = 0;
	    bzone->clock = YES;

	    for (node = 0; node < pzone->n_node; pnode++, node++) {
		pnode->time = node*50*60;
		pnode->temp = node*1000;
		pnode->deviation = 1000;
		pnode->p_coef = 4;
		pnode->i_coef = 10;
		pnode->d_coef = 3;
		pnode->i_depth = 12;
		pnode->limit = 240;
	    }
	}
}

/* tpinit - do power initialization
 */
tpinit (db)
database	*db;
{
	DCL_PANEL

	for (panel = 0; panel < M_PANEL; PANEL__) {

	    DCL_FASE

	    ppanel->connect = db->parameter.misc.vnumber+1;
	    ppanel->ss_down = YES;

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		pfase->max_load = N_ELEMENT;
		pfase->epsilon = 2;

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    int		nb;

		    pelement->n_fneighbor = M_FNEIGHBOR;

		    for (nb = 0; nb < pelement->n_fneighbor; nb++) {
			pelement->fneighbor[nb] = nb;
		    }
		}
	    }
	}
}

/* toprint - do test oven print
 */
toprint ()
{
	printf ("msclock = %d\n", dbs->intermediate.misc.msclock);
	printf ("vnumber = %d\n", dbs->parameter.misc.vnumber);
	printf ("vup0 = %d\n", dbs->parameter.misc.vup[0]);
	ttprint (dbs);
	tpprint (dbs);
	tcprint (dbs);
}

/* ttprint - do temperature print
 */
ttprint (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	DCL_DCU
	p_tctype	*ptctype;

	for (dcu = 0; dcu < M_DCU; DCU__) {

	    DCL_COUNTER

	    printf ("dcuconnect0 = %d\n", pdcu->connect);

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {
		    printf ("tic %d ref %f\n", tic, ptic->reference);
		}
	    }
	}
}

/* tcprint - do control print
 */
tcprint (db)
database	*db;
{
	DCL_ZONE

	for (zone = 0; zone < M_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node;
	    int		node;

	    printf ("zone %d time %d\n", zone, bzone->time/60);
	    printf ("zone %d clock %d\n", zone, bzone->clock);

	    for (node = 0; node < pzone->n_node; pnode++, node++) {
	    }
	}
}

/* tpprint - do power print
 */
tpprint (db)
database	*db;
{
	DCL_PANEL

	for (panel = 0; panel < M_PANEL; PANEL__) {

	    DCL_FASE

	    printf ("panelconnect0 = %d\n", ppanel->connect);

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		printf ("fase %d eps %d\n", fase, pfase->epsilon);

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    int		nb;

		    printf ("element %d heat %d\n", pfe, delement->heat);
		    for (nb = 0; nb < pelement->n_fneighbor; nb++) {
		    }
		}
	    }
	}
}

/* tp1 - do test oven print
 */
tp1 ()
{
	printf ("m=%d c=%d t=%d z=%d e0=%6.1f h3=%6.1f h4=%6.1f p3=%6.1f p4=%6.1f\n",
	  dbs->intermediate.misc.msclock,
	  dbs->intermediate.misc.cclock,
	  dbs->intermediate.misc.tclock,
	  dbs->biparameter.zone[0].time/60,
	  dbs->data.zone[0].etmp,
	  dbs->data.panel[0].fase[0].element[3].htmp,
	  dbs->data.panel[0].fase[0].element[4].htmp,
	  dbs->data.panel[0].fase[0].element[3].p_error,
	  dbs->data.panel[0].fase[0].element[4].p_error);
}

h4check (x)
int	x;
{
	  static float p4;

	  if (p4 == dbs->data.panel[0].fase[0].element[4].p_error)
	      return;
	  p4 = dbs->data.panel[0].fase[0].element[4].p_error;
	  printf ("x=%d", x);
	  tp1 ();
}
