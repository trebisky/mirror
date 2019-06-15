#include <stdio.h>
#include <ctype.h>
#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"

#define	SZLINE	160

#define	SEC(x)	if ((s = firsttok (line)), (strcmp (s, (x)) != 0)) return (1)
#define	EC(x)	if (nnn != (x)) return (1)
#define	SCEC(x)	if ((s = nexttok (s)), ((x) != SCER_OK)) return (1)

static	FILE	*dafp;
static	int	dalineno;

/* firsttok - read in next non-comment line
 */
static
char	*
firsttok (line)
char	*line;
{
	char	*s;
	char	*strchr();

	do {
	    if (fgets (line, SZLINE-1, dafp) == NULL)
		line[0] = 0;
	    else
		dalineno++;
	} while (line[0] == '#');
	if (s = strchr (line, '\n'))
	    *s = 0;
	s = line + strlen (line) + 1;
	*s = 0;
	s = line;
	while (*s && !isspace(*s))
	    s++;
	*s = 0;
	return (line);
}

/* nexttok - get next white space delimited token
 */
static
char	*
nexttok (s)
char	*s;
{
	char	*ip = s;
	char	*op;

	if (*ip) {
	    while (*ip)
		ip++;
	    ip++;
	    while (*ip && isspace(*ip))
		ip++;
	    op = ip;
	    while (*op && !isspace(*op))
		op++;
	    *op = 0;
	}
	return (ip);
}

/* databascw - do data to ascii
 */
databascw ()
{
	if (dafp = fopen ("databasc", "w")) {
	    dataasc (globalp->db);
	    fclose (dafp);
	}
}

/* databascr - do ascii to data
 */
databascr ()
{
	if (dafp = fopen ("databasc", "r")) {
	    dalineno = 0;
	    if (dataascr (globalp->db))
		fprintf (stderr, " error on line %d\n", dalineno);
	    fclose (dafp);
	}
}

/* dataasc - do data to ascii on this database
 */
dataasc (db)
database	*db;
{
	miscasc (db);
	dcuasc (db);
	bussasc (db);
	ticasc (db);
	tcasc (db);
	tctypeasc (db);
	adcasc (db);
	panelasc (db);
	faseasc (db);
	he0asc (db);
	he1asc (db);
	zoneasc (db);
}

/* dataascr - do ascii to data on this database
 */
dataascr (db)
database	*db;
{
	if (miscascr (db)) return (1);
	if (dcuascr (db)) return (1);
	if (bussascr (db)) return (1);
	if (ticascr (db)) return (1);
	if (tcascr (db)) return (1);
	if (tctypeascr (db)) return (1);
	if (adcascr (db)) return (1);
	if (panelascr (db)) return (1);
	if (faseascr (db)) return (1);
	if (he0ascr (db)) return (1);
	if (he1ascr (db)) return (1);
	if (zoneascr (db)) return (1);
	return (0);
}

/* tcasc - do tc data
 */
tcasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_DCU

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "dntx");
	s += prspace (s, 1); s += prstring (s, "d");
	s += prspace (s, 1); s += prstring (s, "i");
	s += prspace (s, 1); s += prstring (s, "t");
	s += prspace (s, 1); s += prstring (s, "serial");
	s += prspace (s, 1); s += prstring (s, "offsetT");
	s += prspace (s, 1); s += prstring (s, "RRRTTTZZZ");
	fprintf (dafp, "%s\n", line);

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

		    for (tc = 0; tc < N_TC; ptc++, itc++, dtc++, tc++) {

			DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC+tc;

			if (tc >= N_TTMP)
			    continue;
			s = line;
			s += prstring (s, "tc    ");
			s += prspace (s, 1); s += prdntx (s, dntx);
			s += prspace (s, 1); s += prstat (s, ptc->down);
			s += prspace (s, 1); s += prenab (s, ptc->ignore);
			s += prspace (s, 1); s += prtctype (s, ptc->tctype);
			s += prspace (s, 1); s += prserial (s, ptc->serial);
			s += prspace (s, 1); s += prtemp (s, ptc->offset);
			s += prspace (s, 1); s += praloc (s, &ptc->loc);
			fprintf (dafp, "%s\n", line);
		    }
		}
	    }
	}
}

/* ticasc - do tic data
 */
ticasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_DCU

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "dntx");
	s += prspace (s, 1); s += prstring (s, "d");
	s += prspace (s, 1); s += prstring (s, "t");
	s += prspace (s, 1); s += prstring (s, "dsb");
	s += prspace (s, 1); s += prstring (s, "ijblck");
	s += prspace (s, 1); s += prstring (s, "serial");
	s += prspace (s, 1); s += prstring (s, "refvlt");
	fprintf (dafp, "%s\n", line);

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC)*N_TC;
		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC;

		    s = line;
		    s += prstring (s, "tic   ");
		    s += prspace (s, 1); s += prdntx (s, dntx);
		    s += prspace (s, 1); s += prstat (s, ptic->down);
		    s += prspace (s, 1); s += prtctype (s, ptic->tctype);
		    s += prspace (s, 1); s += prdsb (s, ptic->select);
		    s += prspace (s, 1); s += prijb (s, ptic->ijblock);
		    s += prspace (s, 1); s += prserial (s, ptic->serial);
		    s += prspace (s, 1); s += prrefv (s, ptic->reference);
		    fprintf (dafp, "%s\n", line);
		}
	    }
	}
}

/* bussasc - do buss data
 */
bussasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_DCU

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "dsb");
	s += prspace (s, 1); s += prstring (s, "d");
	fprintf (dafp, "%s\n", line);

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_SELECT

	    for (select = 0; select < N_SELECT; SELECT__) {

		DSB	dsb = (dcu*N_SELECT+select)*N_BUSS;
		DCL_BUSS

		for (buss = 0; buss < N_BUSS; BUSS__) {

		    DSB	dsb = (dcu*N_SELECT+select)*N_BUSS+buss;

		    s = line;
		    s += prstring (s, "dsb   ");
		    s += prspace (s, 1); s += prdsb (s, dsb);
		    s += prspace (s, 1); s += prstat (s, pbuss->down);
		    fprintf (dafp, "%s\n", line);
		}
	    }
	}
}

/* dcuasc - do dcu data
 */
dcuasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_DCU

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "dntx");
	s += prspace (s, 1); s += prstring (s, "d");
	s += prspace (s, 1); s += prstring (s, "cmp");
	s += prspace (s, 1); s += prstring (s, "portad");
	fprintf (dafp, "%s\n", line);

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;

	    s = line;
	    s += prstring (s, "dcu   ");
	    s += prspace (s, 1); s += prdntx (s, dntx);
	    s += prspace (s, 1); s += prstat (s, pdcu->down);
	    s += prspace (s, 1); s += prvnumb (s, pdcu->connect);
	    s += prspace (s, 1); s += prvport (s, pdcu->port);
	    fprintf (dafp, "%s\n", line);
	}
}

/* he0asc - do he0 data
 */
he0asc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_PANEL

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "pfe");
	s += prspace (s, 1); s += prstring (s, "d");
	s += prspace (s, 1); s += prstring (s, "i");
	s += prspace (s, 1); s += prstring (s, "c");
	s += prspace (s, 1); s += prstring (s, "r");
	s += prspace (s, 1); s += prstring (s, "v");
	s += prspace (s, 1); s += prstring (s, "k");
	s += prspace (s, 1); s += prstring (s, "zon");
	s += prspace (s, 1); s += prstring (s, "beamno");
	s += prspace (s, 1); s += prstring (s, "twistn");
	s += prspace (s, 1); s += prstring (s, "RRRTTTZZZ");
	s += prspace (s, 1); s += prstring (s, "RR1RR2");
	s += prspace (s, 1); s += prstring (s, "TT1TT2");
	s += prspace (s, 1); s += prstring (s, "ZZ1ZZ2");
	fprintf (dafp, "%s\n", line);

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    s = line;
		    s += prstring (s, "he0   ");
		    s += prspace (s, 1); s += prpfe (s, pfe);
		    s += prspace (s, 1); s += prstat (s, pelement->he_down);
		    s += prspace (s, 1); s += prenab (s, pelement->ignore);
		    s += prspace (s, 1); s += prstat (s, pelement->cs_down);
		    s += prspace (s, 1); s += prstat (s, pelement->rs_down);
		    s += prspace (s, 1); s += prstat (s, pelement->vs_down);
		    s += prspace (s, 1); s += prstat (s, pelement->ck_down);
		    s += prspace (s, 1); s += przone (s, pelement->zone);
		    s += prspace (s, 1); s += prbeam (s, pelement->beam);
		    s += prspace (s, 1); s += prtwist (s, pelement->twist);
		    s += prspace (s, 1); s += praloc (s, &pelement->loc);
		    s += prspace (s, 1); s += pralocr (s, &pelement->loc);
		    s += prspace (s, 1); s += praloct (s, &pelement->loc);
		    s += prspace (s, 1); s += pralocz (s, &pelement->loc);
		    fprintf (dafp, "%s\n", line);
		}
	    }
	}
}

/* he1asc - do he1 data
 */
he1asc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_PANEL

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "pfe");
	s += prspace (s, 1); s += prstring (s, "#neigh");
	s += prspace (s, 1); s += prstring (s, "dntx");
	s += prspace (s, 1); s += prstring (s, "dntx");
	s += prspace (s, 1); s += prstring (s, "dntx");
	s += prspace (s, 1); s += prstring (s, "... ");
	fprintf (dafp, "%s\n", line);

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    int		n;

		    s = line;
		    s += prstring (s, "he1   ");
		    s += prspace (s, 1); s += prpfe (s, pfe);
		    s += prspace (s, 1); s += prnnb (s, pelement->n_nneighbor);
		    for (n = 0; n < N_NNEIGHBOR; n++) {
			s += prspace (s, 1);
			if (n < pelement->n_nneighbor)
			    s += prdntx (s, pelement->nneighbor[n]);
			else
			    s += prdntx (s, 0);
		    }
		    fprintf (dafp, "%s\n", line);

		    s = line;
		    s += prstring (s, "he2   ");
		    s += prspace (s, 1); s += prpfe (s, pfe);
		    s += prspace (s, 1); s += prnnb (s, pelement->n_fneighbor);
		    for (n = 0; n < N_FNEIGHBOR; n++) {
			s += prspace (s, 1);
			if (n < pelement->n_fneighbor)
			    s += prdntx (s, pelement->fneighbor[n]);
			else
			    s += prdntx (s, 0);
		    }
		    fprintf (dafp, "%s\n", line);
		}
	    }
	}
}

/* faseasc - do fase data
 */
faseasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_PANEL

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "pfe");
	s += prspace (s, 1); s += prstring (s, "maxlod");
	s += prspace (s, 1); s += prstring (s, "epsiln");
	fprintf (dafp, "%s\n", line);

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		s = line;
		s += prstring (s, "fase  ");
		s += prspace (s, 1); s += prpfe (s, pfe);
		s += prspace (s, 1); s += prmaxload (s, pfase->max_load);
		s += prspace (s, 1); s += prepsilon (s, pfase->epsilon);
		fprintf (dafp, "%s\n", line);
	    }
	}
}

/* panelasc - do panel data
 */
panelasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_PANEL

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "pfe");
	s += prspace (s, 1); s += prstring (s, "d");
	s += prspace (s, 1); s += prstring (s, "cmp");
	s += prspace (s, 1); s += prstring (s, "portad");
	fprintf (dafp, "%s\n", line);

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;

	    s = line;
	    s += prstring (s, "panel ");
	    s += prspace (s, 1); s += prpfe (s, pfe);
	    s += prspace (s, 1); s += prstat (s, ppanel->ss_down);
	    s += prspace (s, 1); s += prvnumb (s, ppanel->connect);
	    s += prspace (s, 1); s += prvport (s, ppanel->port);
	    fprintf (dafp, "%s\n", line);
	}
}

/* tctypeasc - do tctype data
 */
tctypeasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	p_tctype	*ptctype = db->parameter.tctype;
	int		tctype;

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "t");
	s += prspace (s, 1); s += prstring (s, "coef...");
	fprintf (dafp, "%s\n", line);

	for (tctype = 0; tctype < N_TCTYPE; ptctype++, tctype++) {

	    int	n;

	    s = line;
	    s += prstring (s, "polymt");
	    s += prspace (s, 1); s += prtctype (s, tctype);
	    s += prspace (s, 1);
	    for (n = 0; n < N_POLYCOEF; n++) {
		s += prspace (s, 1); s += prpoly (s, ptctype->mt_polycoef[n]);
	    }
	    fprintf (dafp, "%s\n", line);

	    s = line;
	    s += prstring (s, "polycv");
	    s += prspace (s, 1); s += prtctype (s, tctype);
	    s += prspace (s, 1);
	    for (n = 0; n < N_POLYCOEF; n++) {
		s += prspace (s, 1); s += prpoly (s, ptctype->cv_polycoef[n]);
	    }
	    fprintf (dafp, "%s\n", line);

	    s = line;
	    s += prstring (s, "polyjt");
	    s += prspace (s, 1); s += prtctype (s, tctype);
	    s += prspace (s, 1);
	    for (n = 0; n < N_POLYCOEF; n++) {
		s += prspace (s, 1); s += prpoly (s, ptctype->jt_polycoef[n]);
	    }
	    fprintf (dafp, "%s\n", line);
	}
}

/* zoneasc - do zone data
 */
zoneasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_ZONE

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "zon");
	s += prspace (s, 1); s += prstring (s, "k");
	s += prspace (s, 1); s += prstring (s, "h");
	s += prspace (s, 1); s += prstring (s, "K");
	s += prspace (s, 1); s += prstring (s, "zonetime");
	s += prspace (s, 1); s += prstring (s, "#nodes");
	fprintf (dafp, "%s\n", line);

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "zon");
	s += prspace (s, 1); s += prstring (s, "node_#");
	s += prspace (s, 1); s += prstring (s, "nodetime");
	s += prspace (s, 1); s += prstring (s, "nodetmp");
	s += prspace (s, 1); s += prstring (s, "deviate");
	s += prspace (s, 1); s += prstring (s, "tdev   ");
	s += prspace (s, 1); s += prstring (s, "L");
	s += prspace (s, 1); s += prstring (s, "T");
	s += prspace (s, 1); s += prstring (s, "D");
	s += prspace (s, 1); s += prstring (s, "W");
	s += prspace (s, 1); s += prstring (s, "W_z");
	s += prspace (s, 1); s += prstring (s, "l");
	fprintf (dafp, "%s\n", line);

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "zon");
	s += prspace (s, 1); s += prstring (s, "node_#");
	s += prspace (s, 1); s += prstring (s, "p_coefficient");
	s += prspace (s, 1); s += prstring (s, "i_coefficient");
	s += prspace (s, 1); s += prstring (s, "d_coefficient");
	s += prspace (s, 1); s += prstring (s, "idp");
	s += prspace (s, 1); s += prstring (s, "plm");
	fprintf (dafp, "%s\n", line);

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node;
	    int		node;

	    s = line;
	    s += prstring (s, "zone  ");
	    s += prspace (s, 1); s += przone (s, zone);
	    s += prspace (s, 1); s += prstat (s, pzone->ck_down);
	    s += prspace (s, 1); s += prstat (s, pzone->he_ck_down);
	    s += prspace (s, 1); s += prclstat (s, bzone->clock);
	    s += prspace (s, 1); s += prtime (s, bzone->time);
	    s += prspace (s, 1); s += prnoden (s, pzone->n_node);
	    fprintf (dafp, "%s\n", line);

	    for (node = 0; node < pzone->n_node; pnode++, node++) {

		s = line;
		s += prstring (s, "node0 ");
		s += prspace (s, 1); s += przone (s, zone);
		s += prspace (s, 1); s += prnoden (s, node);
		s += prspace (s, 1); s += prtime (s, pnode->time);
		s += prspace (s, 1); s += prtemp (s, pnode->temp);
		s += prspace (s, 1); s += prtemp (s, pnode->deviation);
		s += prspace (s, 1); s += prtemp (s, pnode->tdeviation);
		s += prspace (s, 1); s += pranodel (s, pnode->nlabel);
		s += prspace (s, 1); s += prenab (s, pnode->wflag & WFLAG_T);
		s += prspace (s, 1); s += prenab (s, pnode->wflag & WFLAG_D);
		s += prspace (s, 1); s += prenab (s, pnode->wflag & WFLAG_W);
		s += prspace (s, 1); s += przone (s, pnode->wzone);
		s += prspace (s, 1); s += pranodel (s, pnode->wlabel);
		fprintf (dafp, "%s\n", line);

		s = line;
		s += prstring (s, "node1 ");
		s += prspace (s, 1); s += przone (s, zone);
		s += prspace (s, 1); s += prnoden (s, node);
		s += prspace (s, 1); s += prcoef (s, pnode->p_coef);
		s += prspace (s, 1); s += prcoef (s, pnode->i_coef);
		s += prspace (s, 1); s += prcoef (s, pnode->d_coef);
		s += prspace (s, 1); s += pridepth (s, pnode->i_depth);
		s += prspace (s, 1); s += prpower (s, pnode->limit);
		fprintf (dafp, "%s\n", line);
	    }
	}
}

/* adcasc - do adc data
 */
adcasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	DCL_PANEL

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "pfe");
	s += prspace (s, 1); s += prstring (s, "lower_L");
	s += prspace (s, 1); s += prstring (s, "upper_L");
	fprintf (dafp, "%s\n", line);

	for (panel = 0; panel < N_PANEL-1; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    p_adc	*adc = db->parameter.adc  + panel/N_COMP;
	    pa_panel	*apanel = adc->panel + panel%N_COMP;
	    pa_fase	*afase = apanel->fase;
	    int		rot;

	    for (fase = 0; fase < N_FASE; FASE__, afase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		s = line;
		s += prstring (s, "adcC  ");
		s += prspace (s, 1); s += prpf0 (s, pfe);
		s += prspace (s, 1); s += pramps (s, afase->current.lower);
		s += prspace (s, 1); s += pramps (s, afase->current.upper);
		fprintf (dafp, "%s\n", line);

		s = line;
		s += prstring (s, "adcV  ");
		s += prspace (s, 1); s += prpf0 (s, pfe);
		s += prspace (s, 1); s += prvolts (s, afase->voltage.lower);
		s += prspace (s, 1); s += prvolts (s, afase->voltage.upper);
		fprintf (dafp, "%s\n", line);
	    }

	    s = line;
	    s += prstring (s, "adcN  ");
	    s += prspace (s, 1); s += prp00 (s, pfe);
	    s += prspace (s, 1); s += pramps (s, apanel->current.lower);
	    s += prspace (s, 1); s += pramps (s, apanel->current.upper);
	    fprintf (dafp, "%s\n", line);

	    if (panel%N_COMP == N_PANEL/N_COMP-1) {

		PFE	pfe = (panel/N_COMP*N_FASE)*N_ELEMENT;

		s = line;
		s += prstring (s, "adcH  ");
		s += prspace (s, 1); s += prp00 (s, pfe);
		s += prspace (s, 1); s += prvolts (s, adc->voltage.lower);
		s += prspace (s, 1); s += prvolts (s, adc->voltage.upper);
		fprintf (dafp, "%s\n", line);
	    }
	}
}

/* miscasc - do misc data
 */
miscasc (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	p_misc	*pmisc = &db->parameter.misc;
	int	n;

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "0");
	s += prspace (s, 1); s += prstring (s, "1");
	s += prspace (s, 1); s += prstring (s, "2");
	s += prspace (s, 1); s += prstring (s, "R");
	s += prspace (s, 1); s += prstring (s, "A");
	s += prspace (s, 1); s += prstring (s, "J");
	s += prspace (s, 1); s += prstring (s, "jtmpdif");
	s += prspace (s, 1); s += prstring (s, "rport0");
	s += prspace (s, 1); s += prstring (s, "rport1");
	s += prspace (s, 1); s += prstring (s, "rport2");
	fprintf (dafp, "%s\n", line);

	s = line;
	s += prstring (s, "misc  ");
	s += prspace (s, 1); s += prenab (s, pmisc->vup[0]);
	s += prspace (s, 1); s += prenab (s, pmisc->vup[1]);
	s += prspace (s, 1); s += prenab (s, pmisc->vup[2]);
	s += prspace (s, 1); s += prenab (s, pmisc->logrotv);
	s += prspace (s, 1); s += prenab (s, pmisc->xadc);
	s += prspace (s, 1); s += prenab (s, pmisc->xjtmp);
	s += prspace (s, 1); s += prtemp (s, pmisc->djtmp);
	s += prspace (s, 1); s += prvport (s, pmisc->rport[0]);
	s += prspace (s, 1); s += prvport (s, pmisc->rport[1]);
	s += prspace (s, 1); s += prvport (s, pmisc->rport[2]);
	fprintf (dafp, "%s\n", line);

	s = line;
	s += prstring (s, "#     ");
	s += prspace (s, 1); s += prstring (s, "     n");
	s += prspace (s, 1); s += prstring (s, " offset");
	fprintf (dafp, "%s\n", line);

	for (n = 0; n < N_LVDT; n++) {
	    s = line;
	    s += prstring (s, "lvdt  ");
	    s += prspace (s, 1); s += prserial (s, n);
	    s += prspace (s, 1); s += prtemp (s, pmisc->lvdt[n]);
	    fprintf (dafp, "%s\n", line);
	}
}

/* tcascr - do tc data
 */
tcascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
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

		    for (tc = 0; tc < N_TC; ptc++, itc++, dtc++, tc++) {

			DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC+tc;

			if (tc >= N_TTMP)
			    continue;
			SEC("tc");
			SCEC(scdntx (s, &nnn)); EC(dntx);
			SCEC(scstat (s, &ptc->down));
			SCEC(scenab (s, &ptc->ignore));
			SCEC(sctctype (s, &ptc->tctype));
			SCEC(scserial (s, &ptc->serial));
			SCEC(sctemp (s, &ptc->offset));
			SCEC(scaloc (s, &ptc->loc));
		    }
		}
	    }
	}
	return (0);
}

/* ticascr - do tic data
 */
ticascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC)*N_TC;
		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC;

		    SEC("tic");
		    SCEC(scdntx (s, &nnn)); EC(dntx);
		    SCEC(scstat (s, &ptic->down));
		    SCEC(sctctype (s, &ptic->tctype));
		    SCEC(scdsb (s, &ptic->select));
		    SCEC(scijb (s, &ptic->ijblock));
		    SCEC(scserial (s, &ptic->serial));
		    SCEC(screfv (s, &ptic->reference));
		}
	    }
	}
	return (0);
}

/* bussascr - do buss data
 */
bussascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_SELECT

	    for (select = 0; select < N_SELECT; SELECT__) {

		DSB	dsb = (dcu*N_SELECT+select)*N_BUSS;
		DCL_BUSS

		for (buss = 0; buss < N_BUSS; BUSS__) {

		    DSB	dsb = (dcu*N_SELECT+select)*N_BUSS+buss;

		    SEC("dsb");
		    SCEC(scdsb (s, &nnn)); EC(dsb);
		    SCEC(scstat (s, &pbuss->down));
		}
	    }
	}
	return (0);
}

/* dcuascr - do dcu data
 */
dcuascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;

	    SEC("dcu");
	    SCEC(scdntx (s, &nnn)); EC(dntx);
	    SCEC(scstat (s, &pdcu->down));
	    SCEC(scvnumb (s, &pdcu->connect));
	    SCEC(scvportd (s, &pdcu->port));
	}
	return (0);
}

/* he0ascr - do he0 data
 */
he0ascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    SEC("he0");
		    SCEC(scpfe (s, &nnn)); EC(pfe);
		    SCEC(scstat (s, &pelement->he_down));
		    SCEC(scenab (s, &pelement->ignore));
		    SCEC(scstat (s, &pelement->cs_down));
		    SCEC(scstat (s, &pelement->rs_down));
		    SCEC(scstat (s, &pelement->vs_down));
		    SCEC(scstat (s, &pelement->ck_down));
		    SCEC(sczone (s, &pelement->zone));
		    SCEC(scbeam (s, &pelement->beam));
		    SCEC(sctwist (s, &pelement->twist));
		    SCEC(scaloc (s, &pelement->loc));
		    SCEC(scalocr (s, &pelement->loc));
		    SCEC(scaloct (s, &pelement->loc));
		    SCEC(scalocz (s, &pelement->loc));
		}
	    }
	}
	return (0);
}

/* he1ascr - do he1 data
 */
he1ascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;
		    int		n;

		    SEC("he1");
		    SCEC(scpfe (s, &nnn)); EC(pfe);
		    SCEC(scnnnb (s, &pelement->n_nneighbor));
		    for (n = 0; n < N_NNEIGHBOR; n++) {
			SCEC(scdntx (s, &pelement->nneighbor[n]));
		    }

		    SEC("he2");
		    SCEC(scpfe (s, &nnn)); EC(pfe);
		    SCEC(scnfnb (s, &pelement->n_fneighbor));
		    for (n = 0; n < N_FNEIGHBOR; n++) {
			SCEC(scdntx (s, &pelement->fneighbor[n]));
		    }
		}
	    }
	}
	return (0);
}

/* faseascr - do fase data
 */
faseascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		SEC("fase");
		SCEC(scpfe (s, &nnn)); EC(pfe);
		SCEC(scmaxload (s, &pfase->max_load));
		SCEC(scepsilon (s, &pfase->epsilon));
	    }
	}
	return (0);
}

/* panelascr - do panel data
 */
panelascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;

	    SEC("panel");
	    SCEC(scpfe (s, &nnn)); EC(pfe);
	    SCEC(scstat (s, &ppanel->ss_down));
	    SCEC(scvnumb (s, &ppanel->connect));
	    SCEC(scvportp (s, &ppanel->port));
	}
	return (0);
}

/* tctypeascr - do tctype data
 */
tctypeascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	p_tctype	*ptctype = db->parameter.tctype;
	int		tctype;

	for (tctype = 0; tctype < N_TCTYPE; ptctype++, tctype++) {

	    int	n;

	    SEC("polymt");
	    SCEC(sctctype (s, &nnn)); EC(tctype);
	    for (n = 0; n < N_POLYCOEF; n++) {
		SCEC(scpoly (s, &ptctype->mt_polycoef[n]));
	    }

	    SEC("polycv");
	    SCEC(sctctype (s, &nnn)); EC(tctype);
	    for (n = 0; n < N_POLYCOEF; n++) {
		SCEC(scpoly (s, &ptctype->cv_polycoef[n]));
	    }

	    SEC("polyjt");
	    SCEC(sctctype (s, &nnn)); EC(tctype);
	    for (n = 0; n < N_POLYCOEF; n++) {
		SCEC(scpoly (s, &ptctype->jt_polycoef[n]));
	    }
	}
	return (0);
}

/* zoneascr - do zone data
 */
zoneascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_ZONE

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node;
	    int		node;

	    SEC("zone");
	    SCEC(sczone (s, &nnn)); EC(zone);
	    SCEC(scstat (s, &pzone->ck_down));
	    SCEC(scstat (s, &pzone->he_ck_down));
	    SCEC(scclstat (s, &bzone->clock));
	    SCEC(sctime (s, &bzone->time));
	    SCEC(scnoden (s, &pzone->n_node));

	    for (node = 0; node < pzone->n_node; pnode++, node++) {

		SEC("node0");
		SCEC(sczone (s, &nnn)); EC(zone);
		SCEC(scnoden (s, &nnn)); EC(node);
		SCEC(sctime (s, &pnode->time));
		SCEC(sctemp (s, &pnode->temp));
		SCEC(sctemp (s, &pnode->deviation));
		SCEC(sctemp (s, &pnode->tdeviation));
		SCEC(scanodel (s, &pnode->nlabel));
		pnode->wflag = 0;
		SCEC(scenab (s, &nnn)); if (nnn) pnode->wflag |= WFLAG_T;
		SCEC(scenab (s, &nnn)); if (nnn) pnode->wflag |= WFLAG_D;
		SCEC(scenab (s, &nnn)); if (nnn) pnode->wflag |= WFLAG_W;
		SCEC(sczonea (s, &pnode->wzone));
		SCEC(scanodel (s, &pnode->wlabel));

		SEC("node1");
		SCEC(sczone (s, &nnn)); EC(zone);
		SCEC(scnoden (s, &nnn)); EC(node);
		SCEC(sccoef (s, &pnode->p_coef));
		SCEC(sccoef (s, &pnode->i_coef));
		SCEC(sccoef (s, &pnode->d_coef));
		SCEC(scidepth (s, &pnode->i_depth));
		SCEC(scpower (s, &pnode->limit));
	    }
	}
	return (0);
}

/* adcascr - do adc data
 */
adcascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL-1; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    p_adc	*adc = db->parameter.adc  + panel/N_COMP;
	    pa_panel	*apanel = adc->panel + panel%N_COMP;
	    pa_fase	*afase = apanel->fase;
	    int		rot;

	    for (fase = 0; fase < N_FASE; FASE__, afase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		SEC("adcC");
		SCEC(scpfe (s, &nnn)); EC(pfe);
		SCEC(scamps (s, &afase->current.lower));
		SCEC(scamps (s, &afase->current.upper));

		SEC("adcV");
		SCEC(scpfe (s, &nnn)); EC(pfe);
		SCEC(scvolts (s, &afase->voltage.lower));
		SCEC(scvolts (s, &afase->voltage.upper));
	    }

	    SEC("adcN");
	    SCEC(scpfe (s, &nnn)); EC(pfe);
	    SCEC(scamps (s, &apanel->current.lower));
	    SCEC(scamps (s, &apanel->current.upper));

	    if (panel%N_COMP == N_PANEL/N_COMP-1) {

		PFE	pfe = (panel/N_COMP*N_FASE)*N_ELEMENT;

		SEC("adcH");
		SCEC(scpfe (s, &nnn)); EC(pfe);
		SCEC(scvolts (s, &adc->voltage.lower));
		SCEC(scvolts (s, &adc->voltage.upper));
	    }
	}
	return (0);
}

/* miscascr - do misc data
 */
miscascr (db)
database	*db;
{
	char	line[SZLINE];
	char	*s;
	int	nnn;
	int	n;
	p_misc	*pmisc = &db->parameter.misc;

	SEC("misc");
	SCEC(scenab (s, &pmisc->vup[0]));
	SCEC(scenab (s, &pmisc->vup[1]));
	SCEC(scenab (s, &pmisc->vup[2]));
	SCEC(scenab (s, &pmisc->logrotv));
	SCEC(scenab (s, &pmisc->xadc));
	SCEC(scenab (s, &pmisc->xjtmp));
	SCEC(sctemp (s, &pmisc->djtmp));
	SCEC(scvportp (s, &pmisc->rport[0]));
	SCEC(scvportp (s, &pmisc->rport[1]));
	SCEC(scvportp (s, &pmisc->rport[2]));

	for (n = 0; n < N_LVDT; n++) {
	    SEC("lvdt");
	    SCEC(scserial (s, &nnn)); EC(n);
	    SCEC(sctemp (s, &pmisc->lvdt[n]));
	}
	return (0);
}
