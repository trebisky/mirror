#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"

rnido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	s += prspace (s, 8);
	s += prsped (s, Dzone->etmp);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	return (t);
}

rnnli (s)
char	*s;
{
	int	t = 0;

	t = scnodel (s, &Pnoden->nlabel);
	return (t);
}

rnnlo (s)
char	*s;
{
	int	t = 0;

	s += prnodel (s, Pnoden->nlabel);
	*s++ = Dzone->c_node == Gnode ? '*' : ' ';
	*s++ = 0;
	return (t);
}

rnrti (s)
char	*s;
{
	int	t = 0;
	int	n;
	TEMP	rate;
	TIME	dtime;
	TIME	ddtime = 0;
	TIME	newtime;

	t = sctemp (s, &rate);
	if (!t && Gnode) {
	    rate /= 3600.0;
	    if (ABS(rate) < 0.001/3600.0) {
	        Pnoden->temp = Pnodem->temp;
	    } else {
	        dtime = (Pnoden->temp - Pnodem->temp) / rate;
		dtime = (dtime+30)/60*60; /* normalize to integral minutes */
		if (dtime) {
	            ddtime = dtime - (Pnoden->time - Pnodem->time);
		    if (ddtime) {
		        for (n = Gnode; n < Pzone->n_node; n++) {
			    Pnodex(n)->time += ddtime;
			}

			/* update the zone clock when changing the rate
			 * on the current node.
			 */
	                if ((Pnoden->time != Pnodem->time) &&
			    (Bzone ->time >= Pnodem->time) &&
			    (Bzone ->time <= Pnoden->time) &&
	                    (Pnoden->temp != Pnodem->temp) &&
			    (Dzone ->etmp >= Pnodem->temp) &&
			    (Dzone ->etmp <= Pnoden->temp)) {
			    newtime = ((TIME)
				 ((float)(Pnoden->time - Pnodem->time)
				/ (float)(Pnoden->temp - Pnodem->temp)
			        * (float)(Dzone ->etmp - Pnodem->temp)
				+ Pnodem->time)+30)/60*60;
			    if (Bzone->time != newtime)
				if (Geditb) {
				    Bzone->time = newtime;
				} else {
				    for (n = Gnode; n < Pzone->n_node; n++)
					Pnodex(n)->time -= ddtime;
				    t = SCER_NOEDITB;
				}
			}
		    }
		}
	    }
	}
	return (t);
}

rnrto (s)
char	*s;
{
	int	t = 0;
	TEMP	dtemp;
	TIME	dtime;

	dtemp = Pnoden->temp - Pnodem->temp;
	dtime = Pnoden->time - Pnodem->time;
	s += prsped (s, dtime ? (float)dtemp/(float)dtime*3600.0 : 0.0);
	return (t);
}

rndvi (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pnoden->deviation);
	return (t);
}

rndvo (s)
char	*s;
{
	int	t = 0;

	s += prsped (s, Pnoden->deviation);
	return (t);
}

rngti (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pnoden->temp);
	return (t);
}

rngto (s)
char	*s;
{
	int	t = 0;

	s += prsped (s, Pnoden->temp);
	return (t);
}

rngci (s)
char	*s;
{
	int	t = 0;

	t = sctime (s, &Pnoden->time);
	return (t);
}

rngco (s)
char	*s;
{
	int	t = 0;

	s += prtime (s, Pnoden->time);
	return (t);
}

rndti (s)
char	*s;
{
	int	t = 0;
	TEMP	dtemp;

	t = sctemp (s, &dtemp);
	if (!t && Gnode)
	    Pnoden->temp = Pnodem->temp + dtemp;
	return (t);
}

rndto (s)
char	*s;
{
	int	t = 0;

	s += prsped (s, Pnoden->temp - Pnodem->temp);
	return (t);
}

rndci (s)
char	*s;
{
	int	t = 0;
	int	n;
	TIME	dtime;
	TIME	ddtime = 0;

	t = sctime (s, &dtime);
	if (!t && Gnode)
	    ddtime = dtime - (Pnoden->time - Pnodem->time);
	if (ddtime)
	    for (n = Gnode; n < Pzone->n_node; n++)
		Pnodex(n)->time += ddtime;
	return (t);
}

rndco (s)
char	*s;
{
	int	t = 0;

	s += prtime (s, Pnoden->time - Pnodem->time);
	return (t);
}

rnwtt ()
{
	int	t = 0;

	t = toggleb (&Pnoden->wflag, WFLAG_T);
	return (t);
}

rnwto (s)
char	*s;
{
	int	t = 0;

	prstatusb (s, Pnoden->wflag, WFLAG_T);
	return (t);
}

rnwdt ()
{
	int	t = 0;

	t = toggleb (&Pnoden->wflag, WFLAG_D);
	return (t);
}

rnwdo (s)
char	*s;
{
	int	t = 0;

	prstatusb (s, Pnoden->wflag, WFLAG_D);
	return (t);
}

rnwwt ()
{
	int	t = 0;

	t = toggleb (&Pnoden->wflag, WFLAG_W);
	return (t);
}

rnwwo (s)
char	*s;
{
	int	t = 0;

	prstatusb (s, Pnoden->wflag, WFLAG_W);
	return (t);
}

rnwzi (s)
char	*s;
{
	int	t = 0;

	t = sczonea (s, &Pnoden->wzone);
	return (t);
}

rnwzo (s)
char	*s;
{
	int	t = 0;

	if (Pnoden->wflag & WFLAG_W)
	    przone (s, Pnoden->wzone);
	return (t);
}

rnwni (s)
char	*s;
{
	int	t = 0;

	t = scnodel (s, &Pnoden->wlabel);
	return (t);
}

rnwno (s)
char	*s;
{
	int	t = 0;

	if (Pnoden->wflag & WFLAG_W)
	    prnodel (s, Pnoden->wlabel);
	return (t);
}

rnnpi (s)
char	*s;
{
	int	t = 0;

	t = sccoef (s, &Pnoden->p_coef);
	return (t);
}

rnnpo (s)
char	*s;
{
	int	t = 0;

	s += prcoef (s, Pnoden->p_coef);
	return (t);
}

rnnii (s)
char	*s;
{
	int	t = 0;

	t = sccoef (s, &Pnoden->i_coef);
	return (t);
}

rnnio (s)
char	*s;
{
	int	t = 0;

	s += prcoef (s, Pnoden->i_coef);
	return (t);
}

rnndi (s)
char	*s;
{
	int	t = 0;

	t = sccoef (s, &Pnoden->d_coef);
	return (t);
}

rnndo (s)
char	*s;
{
	int	t = 0;

	s += prcoef (s, Pnoden->d_coef);
	return (t);
}

rnnhi (s)
char	*s;
{
	int	t = 0;

	t = scidepth (s, &Pnoden->i_depth);
	return (t);
}

rnnho (s)
char	*s;
{
	int	t = 0;

	s += pridepth (s, Pnoden->i_depth);
	return (t);
}

rnnqi (s)
char	*s;
{
	int	t = 0;

	t = scpower (s, &Pnoden->limit);
	return (t);
}

rnnqo (s)
char	*s;
{
	int	t = 0;

	s += prpower (s, Pnoden->limit);
	return (t);
}
