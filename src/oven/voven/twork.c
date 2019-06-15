#include "oven.h"
#ifndef	NOTEST
int	tworktest;
#endif

/* twork - temperature work for each tick
 */
twork (db)
database	*db;
{
	int	old;
	int	new;

	pioinit (db);
	forever {
	    twait ();
	    old = (db->intermediate.misc.tclock+TCN_INIT)%TCN_MAXN;
	    new = (old+1) % TCN_MAXN;
	    if (old == TCN_INIT) {
		tcalc (db, old);
		break;
	    }
	}
	forever {
	    twait ();
	    old = (db->intermediate.misc.tclock+TCN_INIT)%TCN_MAXN;
	    new = (old+1) % TCN_MAXN;
	    tdata (db, old, new);
	    tcalc (db, old);
	    if (old == TCN_INIT)
		dcufire ();
	}
}

/* pioinit - do PIO board initialization
 */
pioinit (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	DCL_DCU
	int	s;

	for (dcu = 0; dcu < N_DCU; DCU__) {

#ifdef	NO_INIT_ALL_DCUS
	    if (!(CONNECT (pdcu->connect)))
		continue;
#endif
#ifndef	NOTEST
	    if (tworktest)
		s = tworktest_pio_init (pdcu->port);
	    else
#endif
		s = pio_init (pdcu->port);
	    if (s) {
		;
	    } else {
		;
	    }

	}
}

/* tdata - get the temperature data
 */
tdata (db, old, new)
database *db;
int	old;
int	new;
{
	rdtcntl (db, old);
/*	wrtaddr (db, new);						*/
	rdtdata (db, old);
	wrtaddr (db, new);
	rdtaddr (db, new);
	wrtcntl (db, new);
}

/* tinit - do temperature database initialization
 */
tinit (db)
database	*db;
{
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    i_tc *itc = itic->tc;
		    int	tc;

		    for (tc = 0; tc < N_TTMP; itc++, tc++) {

			int	m;

			itc->good = INDEFT;
			for (m = 0; m < N_MTMP; m++) {
			    itc->mtmp[m] = INDEFT;
			}
		    }
		}
	    }
	}
}

/* tcalc - do temperature calculations
 */
tcalc (db, tcn)
database	*db;
int		tcn;
{
	int	tc = tc_tcn[tcn];
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	DCL_DCU
	int	s;

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    if (!(CONNECT (pdcu->connect))) {
		continue;
	    } else if (tcn == TCN_INIT) {
		ddcu->timestamp = db->data.misc.uclock;
#ifndef	NOTEST
		if (tworktest)
		    s = tworktest_thw_init (pdcu->port);
		else
#endif
		    s = thw_init (pdcu->port);
		if (s) {
		    TERRDST;
		    idcu->error = YES;
		} else {
		    idcu->error = NO;
		}
	    }

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC+tc;
		    p_tc *ptc = ptic->tc+tc;
		    i_tc *itc = itic->tc+tc;
		    d_tc *dtc = dtic->tc+tc;
		    float *jtp = db->parameter.tctype[ptic->tctype].jt_polycoef;
		    float *cvp = db->parameter.tctype[ptc->tctype].cv_polycoef;
		    float *mtp = db->parameter.tctype[ptc->tctype].mt_polycoef;
		    p_buss *pbuss = pbuss_dsb (pdb, ptic->select);
		    i_buss *ibuss = ibuss_dsb (idb, ptic->select);
		    BOOL down = pdcu->down || ptic->down || pbuss->down;
		    BOOL error = idcu->error || itic->error || ibuss->error;
		    int	np = N_POLYCOEF;
		    TEMP mtmp;		/* new measurement temperature	*/

		    switch (tcn) {
		    case TCN_INIT:
			itic->error = NO;
			ibuss->error = NO;
			break;
		    case TCN_IDLE:
			break;
		    case TCN_ZERO:
			break;
		    case TCN_REFV:
			if (down || error) {
			    ;
			} else {
			    if (trefv (COUNTS, ptic->reference, &itic->scale)) {
				TERRFPE;
				itic->error = YES;
			    } else if (itic->scale < 1e-18) {
				itic->error = YES;
			    }
			}
			break;
		    case TCN_JTMP:
			if (down || error) {
			    dtic->jtmp = INDEFT;
			} else {

			    float jv;		/* junction voltage	*/

			    if (tscale (COUNTS, itic->scale, 0.0, &jv)) {
				TERRFPE;
				itic->error = YES;
				dtic->jtmp = INDEFT;
			    } else if (tpoly (jtp, np, jv, &dtic->jtmp)) {
				TERRFPE;
				itic->error = YES;
				dtic->jtmp = INDEFT;
			    } else if (dtic->jtmp < JTMPLLIM ||
			      dtic->jtmp > JTMPULIM) {
				TERRJTL;
				itic->error = YES;
				dtic->jtmp = INDEFT;
			    }
			}
			break;
		    default:
			mtmp = INDEFT;
			if (down || error || ptc->down) {
			    ;
			} else {

			    float cv;		/* compensation voltage	*/
			    float tv;		/* thermocouple voltage	*/
			    TEMP  mt;		/* measurement temperature */

			    if (tpoly (cvp, np, dtic->jtmp, &cv)) {
				TERRFPE;
			    } else if (tscale (COUNTS, itic->scale, cv, &tv)) {
				TERRFPE;
			    } else if (tpoly (mtp, np, tv, &mt)) {
				TERRFPE;
			    } else if (mt += ptc->offset, ptc->down) {
				mt = INDEFT;
			    } else if (mt < TLIMBACK) {
				TERRBACK;
			    } else if (mt < TLIMSHRT) {
				TERRSHRT;
			    } else if (mt > TLIMMELT) {
				TERRMELT;
			    } else {
				if (IS_INDEFT (itc->good)) {
				    mtmp = mt;
				} else {
				    if (ABS (itc->good - mt) > TLIMOPEN) {
					TERROPEN;
				    } else {
					mtmp = mt;
				    }
				}
				itc->good = mt;
			    }
			}
			itc->mtmp[itc->last++] = mtmp;
			itc->last %= N_MTMP;

			/* do an insertion sort to find the median
			 */
			{
			    register TEMP *m;
			    register TEMP w;
			    register int  l = 0;
			    register int  n;
			    TEMP t[N_MTMP];

			    for (m = itc->mtmp; m < itc->mtmp+N_MTMP; m++) {
				w = *m;
				if (IS_INDEFT (w) == NO) {
				    for (n = l++; n != 0; n--) {
					if (t[n-1] > w) {
					    t[n] = t[n-1];
					} else {
					    break;
					}
				    }
				    t[n] = w;
				}
			    }
			    if (l == 0) {
				mtmp = INDEFT;
				itc->good = INDEFT;
			    } else {
				mtmp = t[(l-1)/2];
			    }
			}
			dtc->ttmp = mtmp;
			break;
		    }
		}
	    }
	}
}

/* dcubroadcast - broadcast all my lastest temperatures
 */
dcubroadcast (db)
database	*db;
{
	forever {
	    p_database	*pdb = &db->parameter;
	    i_database	*idb = &db->intermediate;
	    d_database	*ddb = &db->data;
	    DCL_DCU

	    dcuwait ();

	    for (dcu = 0; dcu < N_DCU; DCU__) {
		if (CONNECT (pdcu->connect)) {
		    uportbroadcast (PORTDCU0+dcu, (char *)ddcu,
		      sizeof(*ddcu));
		}
		taskDelay (2);
	    }
	}
}

/* ckjtmp - check junction temperatures
 */
ckjtmp (db)
database	*db;
{
	p_database	*pdb = &db->parameter;
	i_database	*idb = &db->intermediate;
	d_database	*ddb = &db->data;
	DCL_DCU
	TEMP		jtmp;

	if (!pdb->misc.xjtmp)
	    return;

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    if (IS_STALE (ddcu->timestamp))
		continue;

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC;

		    if (IS_INDEFT (dtic->jtmp))
			continue;
		    dntx = adjtic (pdb, dntx); 
		    if (dntx >= N_DCU*N_COUNTER*N_TIC*N_TC)
			continue;
		    if (IS_STALE (ddcu_dntx (ddb, dntx)->timestamp))
			continue;
		    jtmp = dtic_dntx (ddb, dntx)->jtmp;
		    if (IS_INDEFT (jtmp))
			continue;
		    if (ABS (dtic->jtmp - jtmp) > pdb->misc.djtmp)
			TERRIJT;
		}
	    }
	}
}

/* rdtcntl - read the temperature control/status bits
 */
rdtcntl (db, tcn)
database	*db;
int		tcn;
{
	int	tc = tc_tcn[tcn];
	DCL_DCU
	int	s;

	if (tcn == TCN_INIT || tcn == TCN_IDLE)
	    return;

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx	= dcu*N_COUNTER*N_TIC*N_TC;
	    int		timeout, invalid;

	    if (!(CONNECT (pdcu->connect)))
		continue;
	    if (pdcu->down || idcu->error)
		continue;

#ifndef	NOTEST
	    if (tworktest)
		s = tworktest_tmstat (pdcu->port, &timeout, &invalid);
	    else
#endif
		s = tmstat (pdcu->port, &timeout, &invalid);
	    if (s) {
		TERRDST;
		idcu->error = YES;
	    } else {
		if (timeout) {
		    TERRTON;
		    idcu->error = YES;
		} else if (invalid) {
		    TERRDST;
		    idcu->error = YES;
		}
	    }
	}
}

/* wrtcntl - write the temperature control/status bits
 */
wrtcntl (db, tcn)
database	*db;
int		tcn;
{
	int	tc = tc_tcn[tcn];
	DCL_DCU
	int	s;

	if (tcn == TCN_INIT || tcn == TCN_IDLE)
	    return;

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx	= dcu*N_COUNTER*N_TIC*N_TC;
	    int		timeout, invalid;

	    if (!(CONNECT (pdcu->connect)))
		continue;
	    if (pdcu->down || idcu->error)
		continue;

#ifndef	NOTEST
	    if (tworktest)
		s = tworktest_tmstart (pdcu->port, &timeout, &invalid);
	    else
#endif
		s = tmstart (pdcu->port, &timeout, &invalid);
	    if (s) {
		TERRDST;
		idcu->error = YES;
	    } else {
		if (timeout) {
		    TERRTON;
		    idcu->error = YES;
		} else if (invalid) {
		    TERRDST;
		    idcu->error = YES;
		}
	    }
	}
}

/* wrtaddr - write the temperature address bits
 */
wrtaddr (db, tcn)
database	*db;
int		tcn;
{
	int	tc = tc_tcn[tcn];
	DCL_DCU
	int	s;

	if (tcn == TCN_INIT || tcn == TCN_IDLE)
	    return;

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_SELECT

	    if (!(CONNECT (pdcu->connect)))
		continue;
	    if (pdcu->down || idcu->error)
		continue;

	    for (select = 0; select < N_SELECT; SELECT__) {

		DCL_BUSS

		for (buss = 0; buss < N_BUSS; BUSS__) {

		    DSB	dsb = (dcu*N_SELECT+select)*N_BUSS+buss;
		    int	muxi;

		    if (pbuss->down)
			continue;

#ifndef	NOTEST
		    if (tworktest)
			s = tworktest_putmux(pdcu->port,select,buss,mux_tc[tc]);
		    else
#endif
			s = putmux (pdcu->port, select, buss, mux_tc[tc]);
		    if (s) {
			TERRDST;
			idcu->error = YES;
		    }
		}
	    }
	}
}

/* rdtaddr - read the temperature address bits
 */
rdtaddr (db, tcn)
database	*db;
int		tcn;
{
	int	tc = tc_tcn[tcn];
	DCL_DCU
	int	s;

	if (tcn == TCN_INIT || tcn == TCN_IDLE)
	    return;

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_SELECT

	    if (!(CONNECT (pdcu->connect)))
		continue;
	    if (pdcu->down || idcu->error)
		continue;

	    for (select = 0; select < N_SELECT; SELECT__) {

		DCL_BUSS

		for (buss = 0; buss < N_BUSS; BUSS__) {

		    DSB	dsb = (dcu*N_SELECT+select)*N_BUSS+buss;
		    int	muxi;

		    if (pbuss->down)
			continue;

#ifndef	NOTEST
		    if (tworktest)
			s = tworktest_getmux (pdcu->port, select, buss, &muxi);
		    else
#endif
			s = getmux (pdcu->port, select, buss, &muxi);
		    if (s) {
			TERRDST;
			idcu->error = YES;
		    } else {
			if (muxi%N_TC != mux_tc[tc]) {
			    TERRMER;
			    ibuss->error = YES;
			}
		    }
		}
	    }
	}
}

#ifndef	NOTEST
static	int	rdtdata_tc;
#endif

/* rdtdata - read the temperature data
 */
rdtdata (db, tcn)
database	*db;
int		tcn;
{
	int	tc = tc_tcn[tcn];
	DCL_DCU
	int	s;

	if (tcn == TCN_INIT || tcn == TCN_IDLE)
	    return;

#ifndef	NOTEST
	rdtdata_tc = tc;
#endif

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    if (!(CONNECT (pdcu->connect)))
		continue;
	    if (pdcu->down || idcu->error)
		continue;

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    int	count;

#ifndef	NOTEST
		    if (tworktest)
			s = tworktest_getcounts (pdcu->port,counter,tic,&count);
		    else
#endif
			s = getcounts (pdcu->port, counter, tic, &count);
		    if (s) {
			TERRDST;
			idcu->error = YES;
		    }
		    RCOUNTS = count%65536 - 32768;
		}
	    }
	}
}
#ifndef	NOTEST

tworktestx (x) { tworktest = x; }

/* tmstat - read back DCU status - timer should be stopped
 */
tworktest_tmstat (dcu, timeout, invalid)
int	dcu;
int	*timeout;
int	*invalid;
{
	*timeout = NO;
	*invalid = NO;
	return (0);
}

/* tmstart - start DCU timer and check status
 */
tworktest_tmstart (dcu, timeout, invalid)
int	dcu;
int	*timeout;
int	*invalid;
{
	*timeout = NO;
	*invalid = NO;
	return (0);
}

/* save the mux address here for testing
 */
static	int	static_mux;

/* getmux - read the mux address from the DCU select buss
 */
tworktest_getmux (dcu, select, buss, mux)
int	dcu;
int	select;
int	buss;
int	*mux;
{
	*mux = static_mux;
	return (0);
}

/* putmux - write the mux address to the DCU select buss
 */
tworktest_putmux (dcu, select, buss, mux)
int	dcu;
int	select;
int	buss;
int	mux;
{
	static_mux = mux;
	return (0);
}

/* getcounts - read the count data from the DCU counter card for one TIC
 */
tworktest_getcounts (dcu, counter, tic, count)
int	dcu;
int	counter;
int	tic;
int	*count;
{
	static	int	counts[] = { -9421, 0, 1000, 1500, 2000,
				-9433, 12967, -6761 };

	*count = counts[rdtdata_tc] + 32768;
	return (0);
}

/* thw_init - initialize DCU hardware
 */
tworktest_thw_init (dcu)
int	dcu;
{
	return (0);
}

/* pio_init - initialize PIO hardware
 */
tworktest_pio_init (dcu)
int	dcu;
{
	return (0);
}
#endif
