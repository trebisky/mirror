include "../odisp/odisp.h"
include "../odisp/tc_he.h"
include "../odisp/tc_he_bounds.h"
include	"tran.h"

define	DISPCOR	( (($1)-($2)) / ($3) * ($4) + (($4)+1.)/2.0 )
define	DCXB	( DISPCOR ($1,         0.0,   DIAMLBM, XSZBASE) )
define	DCYB	( DISPCOR ($1,         0.0,   DIAMLBM, YSZBASE) )
define	DCXL	( DISPCOR ($1,         0.0,   DIAMLBM, XSZLID ) )
define	DCYL	( DISPCOR ($1,         0.0,   DIAMLBM, YSZLID ) )
define	DCXM	( DISPCOR ($1,         0.0,   DIAMLBM, XSZMOLD) )
define	DCYM	( DISPCOR ($1,         0.0,   DIAMLBM, YSZMOLD) )
define	DCXW	( DISPCOR ($1, CENTERWALLT, WALLSIZET, XSZWALL) )
define	DCYW	( DISPCOR ($1, CENTERWALLZ, WALLSIZEZ, YSZWALL) )
define	DCXA	( DISPCOR ($1,         0.0,   DIAMLBM, XSZALUM) )
define	DCYA	( DISPCOR ($1,         0.0,   DIAMLBM, YSZALUM) )

# TCMARK - print the TC locations

procedure t_tcmark ()

char	where[LINE_SIZE]
int	i
real	bmin, bmax, lmin, lmax, wmin, wmax, mmin, mmax, umin, umax

include "tran.com"

errchk	init_info

begin
	bmin = TBMIN
	bmax = TBMAX
	lmin = TLMIN
	lmax = TLMAX
	wmin = TWMIN
	wmax = TWMAX
	mmin = TMMIN
	mmax = TMMAX
	umin = TAMIN
	umax = TAMAX

	call init_info ()

	call clgstr ("aspect", where, LINE_SIZE)

	do i = TOFFSET, last_therm {
	    switch (where[1]) {
	    case BASE_TYPE:
		if (IS_BASE (lz[i])) {
		    call printf ("%f %f %03d_%03d_%03d %s\n")
			call pargr (DCXB (lx[i]))
			call pargr (DCYB (ly[i]))
			call pargi (lr[i])
			call pargi (lt[i])
			call pargi (lz[i])
			call pargstr (name[1,i])
		}
	    case LID_TYPE:
		if (IS_LID (lz[i])) {
		    call printf ("%f %f %03d_%03d_%03d %s\n")
			call pargr (DCXL (lx[i]))
			call pargr (DCYL (ly[i]))
			call pargi (lr[i])
			call pargi (lt[i])
			call pargi (lz[i])
			call pargstr (name[1,i])
		}
	    case MOLD_TYPE:
		if (IS_MOLD (lz[i])) {
		    call printf ("%f %f %03d_%03d_%03d %s\n")
			call pargr (DCXM (lx[i]))
			call pargr (DCYM (ly[i]))
			call pargi (lr[i])
			call pargi (lt[i])
			call pargi (lz[i])
			call pargstr (name[1,i])
		}
	    case WALL_TYPE:
		if (IS_WALL (lr[i], lz[i])) {
		    call printf ("%f %f %03d_%03d_%03d %s\n")
			call pargr (DCXW (real(lt[i])))
			call pargr (DCYW (real(lz[i])))
			call pargi (lr[i])
			call pargi (lt[i])
			call pargi (lz[i])
			call pargstr (name[1,i])
		}
	    case ALUM_TYPE:
		if (IS_ALUM (lz[i])) {
		    call printf ("%f %f %03d_%03d_%03d %s\n")
			call pargr (DCXA (lx[i]))
			call pargr (DCYA (ly[i]))
			call pargi (lr[i])
			call pargi (lt[i])
			call pargi (lz[i])
			call pargstr (name[1,i])
		}
	    }
	}
end
