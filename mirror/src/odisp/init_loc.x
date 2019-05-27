include <math.h>
include "odisp.h"
include "tc_he.h"
include "tc_he_bounds.h"

# INIT_LOC.X - initialize locations of thermocouples and heaters.


procedure init_loc()

int	index, oi, idata, i, iloc
real	radius, theta, z
real	bmin, bmax, lmin, lmax, wmin, wmax, mmin, mmax, umin, umax
int	n
bool	gtche_loc()

include "tc_he.com"
include "debugd.com"

begin
	do idata = 1, NINFO {

	    switch (idata) {
	    case HVAL, HTVAL:
		bmin = HBMIN
		bmax = HBMAX
		lmin = HLMIN
		lmax = HLMAX
		wmin = HWMIN
		wmax = HWMAX
		mmin = HMMIN
		mmax = HMMAX
		umin = HAMIN
		umax = HAMAX
	    case TVAL:
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
	    }

	    do i = 1, NASPECT
		num[i, idata] = 0
	    oi = 0

	    n = 0
	    while (gtche_loc (idata, n+1, index, radius, theta, z)) {
		n = n+1

		if (IS_BASE (z)) {
		    iloc = num[BVAL, idata] + 1
		    if (iloc > DLEN)
			call fatal (1, "base arrays too small")
		    # theta = 0 is NORTH, top of image (large y).
		    # theta increases to EAST, in clockwise sense.
		    theta = DEGTORAD (theta)
		    loc[iloc, BVAL, idata] = index
		    xpos[iloc, BVAL, idata] = radius * sin (theta)
		    ypos[iloc, BVAL, idata] = radius * cos (theta)
		    num[BVAL, idata] = iloc
		    call check_list (xpos[1, BVAL, idata],
			    ypos [1, BVAL, idata], num[BVAL, idata])
		}
		else if (IS_LID (z)) {
		    iloc = num[LVAL, idata] + 1
		    if (iloc > DLEN)
			call fatal (2, "lid arrays too small")
		    theta = DEGTORAD (theta)
		    loc[iloc, LVAL, idata] = index
		    xpos[iloc, LVAL, idata] = radius * sin (theta)
		    ypos[iloc, LVAL, idata] = radius * cos (theta)
		    num[LVAL, idata] = iloc
		    call check_list (xpos[1, LVAL, idata],
			    ypos [1, LVAL, idata], num[LVAL, idata])
		}
		else if (IS_WALL (radius, z)) {
		    iloc = num[WVAL, idata] + 1
		    if (iloc > DLEN)
			call fatal (3, "wall arrays too small")
		    loc[iloc, WVAL, idata] = index
		    xpos[iloc, WVAL, idata] = theta
		    ypos[iloc, WVAL, idata] = z
		    num[WVAL, idata] = iloc
		    call check_list (xpos[1, WVAL, idata],
			    ypos [1, WVAL, idata], num[WVAL, idata])
		    if (theta <= WALL_OVERLAP) {
			iloc = num[WVAL, idata] + 1
			if (iloc > DLEN)
			    call fatal (3, "wall arrays too small")
			loc[iloc, WVAL, idata] = index
			xpos[iloc, WVAL, idata] = theta + CIRCLE
			ypos[iloc, WVAL, idata] = z
			num[WVAL, idata] = iloc
			call check_list (xpos[1, WVAL, idata],
				ypos [1, WVAL, idata], num[WVAL, idata])
		    }
		    else if (theta >= (CIRCLE - WALL_OVERLAP)) {
			iloc = num[WVAL, idata] + 1
			if (iloc > DLEN)
			    call fatal (3, "wall arrays too small")
			loc[iloc, WVAL, idata] = index
			xpos[iloc, WVAL, idata] = theta - CIRCLE
			ypos[iloc, WVAL, idata] = z
			num[WVAL, idata] = iloc
			call check_list (xpos[1, WVAL, idata],
				ypos [1, WVAL, idata], num[WVAL, idata])
		    }
		}
		else if (IS_MOLD (z)) {
		    iloc = num[MVAL, idata] + 1
		    if (iloc > DLEN)
			call fatal (4, "mold arrays too small")
		    theta = DEGTORAD (theta)
		    loc[iloc, MVAL, idata] = index
		    xpos[iloc, MVAL, idata] = radius * sin (theta)
		    ypos[iloc, MVAL, idata] = radius * cos (theta)
		    num[MVAL, idata] = iloc
		    call check_list (xpos[1, MVAL, idata],
			    ypos [1, MVAL, idata], num[MVAL, idata])
		}
		else if (IS_ALUM (z)) {
		    iloc = num[AVAL, idata] + 1
		    if (iloc > DLEN)
			call fatal (2, "alum arrays too small")
		    theta = DEGTORAD (theta)
		    loc[iloc, AVAL, idata] = index
		    xpos[iloc, AVAL, idata] = radius * sin (theta)
		    ypos[iloc, AVAL, idata] = radius * cos (theta)
		    num[AVAL, idata] = iloc
		    call check_list (xpos[1, AVAL, idata],
			    ypos [1, AVAL, idata], num[AVAL, idata])
		}
		else {
		    oi = oi + 1
		}
	    }
	    if (debug) {
		call eprintf ("  nb = %d\n  nl = %d\n  nw = %d\n  nm = %d\n  na = %d\n  no = %d\n")
		    call pargi (num[BVAL, idata])
		    call pargi (num[LVAL, idata])
		    call pargi (num[WVAL, idata])
		    call pargi (num[MVAL, idata])
		    call pargi (num[AVAL, idata])
		    call pargi (oi)
	    }
	}
end


# CHECK_LIST - Check that no duplicates appear in the lists.  Only the last
#              entry needs to be checked.  If there are any, remove last entry.

procedure check_list (xpos, ypos, num)

real	xpos[ARB], ypos[ARB]	# lists of values to check
int	num

int	i
real	x, y

include "debugd.com"

begin
	if (num > 1) {
	    x = xpos[num]
	    y = ypos[num]
	    do i = 1, num - 1
		if (xpos[i] == x && ypos[i] == y) {
		    if (debug) {
			call eprintf ("duplicate value at i = %d, n = %d, x = %f, y = %f\n")
			    call pargi (i)
			    call pargi (num)
			    call pargr (x)
			    call pargr (y)
		    }
		    num = num - 1
		    break
		}
	}
end

# GTCHE_LOC - get tc or he loc info

bool procedure gtche_loc (idata, n, index, radius, theta, z)
int	idata
int	n
int	index
real	radius
real	theta
real	z

bool	ghe_loc(), gtc_loc()

begin
	switch (idata) {
	case HVAL, HTVAL:
	    return (ghe_loc (n, index, radius, theta, z))
	case TVAL:
	    return (gtc_loc (n, index, radius, theta, z))
	}
end
