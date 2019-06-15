#define	import_spp
#include <iraf.h>
#include "../oven/oven.h"
#include "../oven/menus.h"
#include "../oven/context.h"
#include "../oven/global.h"
#include "../oven/oxnames.h"

/* ptimestamp - compare parameter timestamps
 */
XLONG
PTIMESTAMP ()
{
	database	*db = globalp->db;

	return (db->parameter.misc.timestamp);
}

/* gpan_loc - ADC and Rotation value names and indices
 */
GPAN_LOC (gp_index, gp_name, l_gpan, lword)
XINT	*gp_index;
XCHAR	*gp_name;
XINT	*l_gpan;
XINT	*lword;
{
	database	*db = globalp->db;
	DCL_PANEL
	int	comp;
	int	rot;
	int	n = 1;
	char	name[SZ_FNAME];
	static	char	*rname[] = {
	    "fva", "fvb", "fvc", "p12", "m12", "p24", "m24", "fld",
	    "amp", "err", "ref", "fdbk", "ch12", "ch13", "ch14", "ch15",
	    "lvdt0", "lvdt1", "lvdt2", "lvdt3",
	    "lvdt4", "lvdt5", "lvdt6", "lvdt7",
	    "lvdt8", "lvdt9", "lvdt10", "lvdt11",
	    "lvdt12", "lvdt13", "lvdt14", "lvdt15"
	    };

	for (panel = 0; panel < N_PANEL-1; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {
		*gp_index++ = n++;
		sprintf (name, "fc%d%d0", panel, fase);
		c_strupk (name, gp_name, *lword);
		gp_name += *lword+1;
		*gp_index++ = n++;
		sprintf (name, "fv%d%d0", panel, fase);
		c_strupk (name, gp_name, *lword);
		gp_name += *lword+1;
	    }
	    *gp_index++ = n++;
	    sprintf (name, "pc%d00", panel);
	    c_strupk (name, gp_name, *lword);
	    gp_name += *lword+1;

	    if (panel%N_COMP == N_PANEL/N_COMP-1) {
		*gp_index++ = n++;
		sprintf (name, "hv%d00", panel/N_COMP);
		c_strupk (name, gp_name, *lword);
		gp_name += *lword+1;
	    }
	}

	n = 1;
	for (comp = 0; comp < N_COMP; comp++) {
	    for (rot = 0; rot < N_ROT; rot++) {
		*gp_index++ = n++;
		sprintf (name, "r%d%s", comp, rname[rot]);
		c_strupk (name, gp_name, *lword);
		gp_name += *lword+1;
	    }
	    *gp_index++ = n++;
	    sprintf (name, "r%d%s", comp, "rpm");
	    c_strupk (name, gp_name, *lword);
	    gp_name += *lword+1;
	}
}

/* tc_loc - tc locations
 */
TC_LOC (tc_index, tc_radius, tc_theta, tc_z, l_ttmp)
XINT	*tc_index;
XINT	*tc_radius;
XINT	*tc_theta;
XINT	*tc_z;
XINT	*l_ttmp;
{
	database	*db = globalp->db;
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    p_tc	*ptc = ptic->tc;
		    int		tc;

		    for (tc = 0; tc < N_TTMP; ptc++, tc++) {

		      DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TTMP+tc;

			*tc_index++ = dntx+1;
			*tc_radius++ = ptc->loc.r;
			*tc_theta++ = ptc->loc.t;
			*tc_z++ = ptc->loc.z;
		    }
		}
	    }
	}
}

/* he_loc - heater element locations
 */
HE_LOC (he_index, he_radius, he_theta, he_z, l_hpwr)
XINT	*he_index;
XINT	*he_radius;
XINT	*he_theta;
XINT	*he_z;
XINT	*l_hpwr;
{
	database	*db = globalp->db;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    *he_index++ = pfe+1;
		    *he_radius++ = pelement->loc.r;
		    *he_theta++ = pelement->loc.t;
		    *he_z++ = pelement->loc.z;
		}
	    }
	}
}

/* datab_xtmp - Temperature schedule
 */
DATAB_XTMP (db_n, db_time, db_val, n_xtmp, l_xtmp)
XINT	*db_n;
XLONG	*db_time;
XREAL	*db_val;
XINT	*n_xtmp;
XINT	*l_xtmp;
{
	database	*db = globalp->db;
	DCL_ZONE

	for (zone = 0; zone < N_ZONE; ZONE__) {

	    p_node	*pnode = pzone->node;
	    int		node;
	    int		n_node = pzone->n_node;

	    *db_n++ = n_node;

	    for (node = 0; node < n_node; pnode++, node++) {
		*(db_time + node) = pnode->time - bzone->time;
		*(db_val  + node) = pnode->temp;
	    }
	    db_time += *l_xtmp;
	    db_val  += *l_xtmp;
	}
}

/* xtc_loc - tc locations
 */
XTC_LOC (tc_name, lword, tc_radius, tc_theta, tc_z, tc_x, tc_y, l_ttmp)
XCHAR	*tc_name;
XINT	*lword;
XINT	*tc_radius;
XINT	*tc_theta;
XINT	*tc_z;
XREAL	*tc_x;
XREAL	*tc_y;
XINT	*l_ttmp;
{
	database	*db = globalp->db;
	char	name[SZ_FNAME];
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    p_tc	*ptc = ptic->tc;
		    int		tc;

		    for (tc = 0; tc < N_TTMP; ptc++, tc++) {

		      DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TTMP+tc;

			prdntx (name, dntx);
			c_strupk (name, tc_name, *lword);
			tc_name += *lword+1;
			*tc_radius++ = ptc->loc.r;
			*tc_theta++ = ptc->loc.t;
			*tc_z++ = ptc->loc.z;
			*tc_x++ = ptc->loc.x;
			*tc_y++ = ptc->loc.y;
		    }
		}
	    }
	}
	*l_ttmp = N_DCU*N_COUNTER*N_TIC*N_TTMP;
}

/* xhe_loc - heater element locations
 */
XHE_LOC (he_name, lword, he_radius, he_theta, he_z, he_x, he_y, l_hpwr)
XCHAR	*he_name;
XINT	*lword;
XINT	*he_radius;
XINT	*he_theta;
XINT	*he_z;
XREAL	*he_x;
XREAL	*he_y;
XINT	*l_hpwr;
{
	database	*db = globalp->db;
	char	name[SZ_FNAME];
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    prpfe (name, pfe);
		    c_strupk (name, he_name, *lword);
		    he_name += *lword+1;
		    *he_radius++ = pelement->loc.r;
		    *he_theta++ = pelement->loc.t;
		    *he_z++ = pelement->loc.z;
		    *he_x++ = pelement->loc.x;
		    *he_y++ = pelement->loc.y;
		}
	    }
	}
	*l_hpwr = N_PANEL*N_FASE*N_ELEMENT;
}

/* he_map - heater element boundarys
 */
HE_MAP (id, r1, r2, t1, t2, z1, z2, maplen)
XINT	*id;
XREAL	*r1;
XREAL	*r2;
XREAL	*t1;
XREAL	*t2;
XREAL	*z1;
XREAL	*z2;
XINT	*maplen;
{
	database	*db = globalp->db;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    *id++ = pfe+1;
		    *r1++ = pelement->loc.r1;
		    *r2++ = pelement->loc.r2;
		    *t1++ = pelement->loc.t1;
		    *t2++ = pelement->loc.t2;
		    *z1++ = pelement->loc.z1;
		    *z2++ = pelement->loc.z2;
		}
	    }
	}
	*maplen = N_PANEL*N_FASE*N_ELEMENT;
}

/* ghtnow - get htmp's now
 */
GHTNOW (c, maplen)
XREAL	*c;
XINT	*maplen;
{
	database	*db = globalp->db;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    *c++ = delement->htmp;
		}
	    }
	}
	*maplen = N_PANEL*N_FASE*N_ELEMENT;
}

/* ghpnow - get hpwr's now
 */
GHPNOW (c, maplen)
XREAL	*c;
XINT	*maplen;
{
	database	*db = globalp->db;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    *c++ = delement->heat;
		}
	    }
	}
	*maplen = N_PANEL*N_FASE*N_ELEMENT;
}

/* gttnow - get ttmp's now
 */
GTTNOW (c, maplen)
XREAL	*c;
XINT	*maplen;
{
	database	*db = globalp->db;
	char	name[SZ_FNAME];
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    d_tc	*dtc = dtic->tc;
		    int		tc;

		    for (tc = 0; tc < N_TTMP; dtc++, tc++) {

		      DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TTMP+tc;

			*c++ = dtc->ttmp;
		    }
		}
	    }
	}
	*maplen = N_DCU*N_COUNTER*N_TIC*N_TTMP;
}
