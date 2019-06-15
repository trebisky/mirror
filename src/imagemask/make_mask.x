include <math.h>
include "../odisp/odisp.h"
include "../odisp/tc_he.h"
include "../odisp/tc_he_bounds.h"
include "../xytran/tran.h"
include	"mask.h"

# MAKE_MASK.X - compute an image map and mask.

# The image has, as pixel values, the index for the heater which lies at the
# location of that pixel.  Pixels beyond the outer diameter of the base, lid,
# and mold are set to -1 to indicate that these pixels are not part of the
# oven component.  Pixels inside the inner radius of the mold are also set
# to -1.  All other pixel values (middles of lid and base, and various regions
# of the wall) are set to 0 to indicate the absence of heaters in that area.


procedure make_mask (aspectno, xsize, ysize)

int	aspectno		# aspect number
int	xsize, ysize		# image size in pixels

real	zmin, zmax
int	i, j, n
real	midx, midy, xsz, ysz, rmin
real	rsq, x, y, y2, z, theta
pointer	sp, line

double	ckdatan2()

include "map.com"
include "mask.com"
include	"../odisp/debugd.com"

errchk	malloc

begin
	call smark (sp)
	call salloc (line, xsize, TY_INT)

	switch (aspectno) {
	case BVAL:
	    zmin = HBMIN
	    zmax = HBMAX
	case LVAL:
	    zmin = HLMIN
	    zmax = HLMAX
	case WVAL:
	    zmin = HWMIN
	    zmax = HWMAX
	case MVAL:
	    zmin = HMMIN
	    zmax = HMMAX
	case AVAL:
	    zmin = HAMIN
	    zmax = HAMAX
	}

	# Set up scaling parameters.
	xsz = xsize
	ysz = ysize
	midx = (xsize + 1.) / 2.
	midy = (ysize + 1.) / 2.

	rmin = r1[1]
	if (aspectno != WVAL && aspectno != MVAL) {
	    do i = 2, maplen
		if (r1[i] < rmin)
		    rmin = r1[i]
	}

	call malloc (maskp[aspectno], xsize*ysize, TY_INT)
	mxsize[aspectno] = xsize
	mysize[aspectno] = ysize

	if (aspectno == WVAL) {
	    do j = 1, ysize {
	        if (debug)
		    call eprintf ("w")
		z = ((real(j) - midy) / ysz) * WALLSIZEZ + CENTERWALLZ
	        do i = 0, xsize - 1 {
		    theta = DEGTORAD (((real(i+1) - midx) / xsz) *
				CIRCLE + CENTERWALLT)
		    Memi[line + i] = 0
		    do n = 1, maplen
			if (r1[n] >= HWMIN*HWMIN && r2[n] <= HWMAX*HWMAX)
			    if (z > z1[n] && z <= z2[n])
				if (th1[n] < th2[n]) {
				    # normal order
				    if (theta > th1[n] && theta <= th2[n]) {
					Memi[line + i] = he_id[n]
					break
				    }
				} else {
				    # theta limits cross 360 degrees
				    if (theta > th1[n] || theta <= th2[n]) {
					Memi[line + i] = he_id[n]
					break
				    }
				}
#		    if (debug) {
#			call eprintf (" %d")
#			call pargi (Memi[line + i])
#		    }
		}
#		if (debug)
#		    call eprintf ("\n")
	        
	        call amovi (Memi[line], OMASK(1, j, aspectno), xsize)
	    }
	} else {
	    do j = 1, ysize {
	        if (debug)
		    call eprintf ("o")
		y = ((real(j) - midy) / ysz) * DIAMLBM
		y2 = y * y
	        do i = 0, xsize - 1 {
		    x = ((real(i+1) - midx) / xsz) * DIAMLBM
		    rsq = x * x + y2
		    if (rsq == 0.)
			theta = 0.
		    else {
			theta = ckdatan2 (double (x), double (y))
			if (theta < 0.)
			    theta = theta + TWOPI
		    }
		    Memi[line + i] = -1
		    if (rsq <= rmin && aspectno != MVAL)
			Memi[line + i] = 0
		    else
		        do n = 1, maplen
			    if (z1[n] >= zmin && z2[n] <= zmax)
				if (rsq > r1[n] && rsq <= r2[n])
				    if (th1[n] < th2[n]) {
					# normal order
					if (theta > th1[n] && theta <= th2[n]) {
					    Memi[line + i] = he_id[n]
					    break
					}
				    } else {
					# theta limits cross 360 degrees
					if (theta > th1[n] || theta <= th2[n]) {
					    Memi[line + i] = he_id[n]
					    break
					}
				    }
#		    if (debug) {
#			call eprintf (" %d")
#			call pargi (Memi[line + i])
#		    }
		}
#		if (debug)
#		    call eprintf ("\n")
	        
	        call amovi (Memi[line], OMASK(1, j, aspectno), xsize)
	    }
	}
	if (debug)
	    call eprintf ("\n")

	call sfree (sp)
end
