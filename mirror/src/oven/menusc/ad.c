#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

adaao (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp, panel, fase;
	PFE	pfe;
	d_fase	*dfase;
	da_fase	*dafase;
	da_panel *dapanel;
	float	amps = 0.0, load = 0.0, kwattsm = 0.0, kwattsc = 0.0;

	if ((ncomp = (n - ((N_FASE+2)*(N_PANEL-1)+1))) >= 0) {
	    s += prah (s, ncomp, Ddb->adc[ncomp].Voltage);
	} else if (n == (N_FASE+2)*(N_PANEL-1)) {
	    for (panel = 0; panel < N_PANEL-1; panel++) { /* no phantom */
	        for (fase = 0; fase < N_FASE; fase++) {
		    pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
		    dfase = dfase_pfe (Ddb, pfe);
		    dafase = dafase_pfe (Ddb, pfe);
		    ADS (amps, dafase->Current);
		    ADS (load, dfase->calc_load);
		    ADS (kwattsm, KW (dafase->Voltage, dafase->Current));
		    ADS (kwattsc, KW (dafase->Voltage, H2A (dfase->calc_load)));
	        }
	    }
	    s += prat (s, amps, load, kwattsm, kwattsc);
	} else {
	    panel = n/(N_FASE+2);
	    fase  = n%(N_FASE+2);
	    pfe = panel*(N_FASE*N_ELEMENT);
	    if (fase == N_FASE+1) {
	        for (fase = 0; fase < N_FASE; fase++) {
		    PFE	pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;

		    dfase = dfase_pfe (Ddb, pfe);
		    dafase = dafase_pfe (Ddb, pfe);
		    ADS (amps, dafase->Current);
		    ADS (load, dfase->calc_load);
		    ADS (kwattsm, KW (dafase->Voltage, dafase->Current));
		    ADS (kwattsc, KW (dafase->Voltage, H2A (dfase->calc_load)));
	        }
	        s += prapt (s, pfe, amps, load, kwattsm, kwattsc);
	    } else if (fase == N_FASE) {
		dapanel = dapanel_pfe (Ddb, pfe);
	        s += prapn (s, pfe, dapanel->Current);
	    } else {
		pfe = panel*(N_FASE*N_ELEMENT) + fase*N_ELEMENT;
		dfase = dfase_pfe (Ddb, pfe);
		dafase = dafase_pfe (Ddb, pfe);
	        s += prapf (s, pfe, dafase->Voltage, dafase->Current,
		    dfase->calc_load);
	    }
	}
	return (t);
}

adna ()
{
	int	n = 0;

	n = N_FASE;
	n += 2;			/* + neutral + total		*/
	n *= (N_PANEL-1);
	n += 1;			/* + grand total		*/
	n += N_COMP;		/* + housekeeping		*/
	return (n);
}
