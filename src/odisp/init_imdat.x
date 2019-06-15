include "odisp.h"
include "tc_he.h"
include "tc_he_bounds.h"

# INIT_IMDAT.X - initialize image and data parameters


procedure init_imdat()

int	i, szx[NASPECT], szy[NASPECT]
real	rmax

data	szx / XSZBASE, XSZLID, XSZWALL, XSZMOLD, XSZALUM /
data	szy / YSZBASE, YSZLID, YSZWALL, YSZMOLD, YSZALUM /

include "imdat.com"
include "debugd.com"

begin
	# Set image sizes.
	do i = 1, NASPECT {
	    imszx[i] = szx[i]
	    imszy[i] = szy[i]
	}

	# Set array sizes for smoothing routine.
	do i = 1, NASPECT {
	    imsmx[i] = szx[i]
	    imsmy[i] = szy[i]
	}

	# Set offsets for image in smoothed array.
	do i = 1, NASPECT {
	    imoffx[i] = 0
	    imoffy[i] = 0
	}

	# Do wall x-direction specially:  overlaps on both ends.
	imsmx[WVAL] = XSZWALL * (1. + 2.*WALL_OVERLAP/CIRCLE) + 0.5
	imoffx[WVAL] = XSZWALL * (WALL_OVERLAP/CIRCLE) + 0.5

	# Check limits for smoothing routine.
	do i = 1, NASPECT
	    if (max (imsmx[i], imsmy[i]) > MAXSMAXISLEN) {
		call eprintf ("axis length = %d, > %d\n")
		    call pargi (max (imsmx[i], imsmy[i]))
		    call pargi (MAXSMAXISLEN)
		call fatal (12, "axis length too large for smoothing routine")
	    } else if (imsmx[i]*imsmy[i] > MAXSMAREA)
		call fatal (13, "image area too large for smoothing routine")

	# Set image dimensions.
	rmax = max (HWMAX, TWMAX)
	imxmn[BVAL] = -rmax
	imxmx[BVAL] = rmax
	imymn[BVAL] = -rmax
	imymx[BVAL] = rmax
	imxmn[LVAL] = -rmax
	imxmx[LVAL] = rmax
	imymn[LVAL] = -rmax
	imymx[LVAL] = rmax
	imxmn[WVAL] = -(WALL_OVERLAP)
	imxmx[WVAL] = CIRCLE + WALL_OVERLAP
	imymn[WVAL] = min (HBMAX, TBMAX)
	imymx[WVAL] = max (HLMIN, TLMIN)
	imxmn[MVAL] = -rmax
	imxmx[MVAL] = rmax
	imymn[MVAL] = -rmax
	imymx[MVAL] = rmax
	imxmn[AVAL] = -rmax
	imxmx[AVAL] = rmax
	imymn[AVAL] = -rmax
	imymx[AVAL] = rmax

	if (debug) {
	    call eprintf ("for each aspect:  base, lid, wall, mold, alum\n")
	    call eprintf ("   image size x:  %5d %5d %5d %5d pixels\n")
		do i = 1, NASPECT
		    call pargi (imszx[i])
	    call eprintf ("   image size y:  %5d %5d %5d %5d pixels\n")
		do i = 1, NASPECT
		    call pargi (imszy[i])
	    call eprintf ("  smooth size x:  %5d %5d %5d %5d pixels\n")
		do i = 1, NASPECT
		    call pargi (imsmx[i])
	    call eprintf ("  smooth size y:  %5d %5d %5d %5d pixels\n")
		do i = 1, NASPECT
		    call pargi (imsmy[i])
	    call eprintf ("    offset in x:  %5d %5d %5d %5d pixels\n")
		do i = 1, NASPECT
		    call pargi (imoffx[i])
	    call eprintf ("    offset in y:  %5d %5d %5d %5d pixels\n")
		do i = 1, NASPECT
		    call pargr (imoffy[i])
	    call eprintf ("   image left x:  %5d %5d %5d %5d inches\n")
		do i = 1, NASPECT
		    call pargr (imxmn[i])
	    call eprintf ("  image right x:  %5d %5d %5d %5d inches\n")
		do i = 1, NASPECT
		    call pargr (imxmx[i])
	    call eprintf ("    image low y:  %5d %5d %5d %5d inches\n")
		do i = 1, NASPECT
		    call pargr (imymn[i])
	    call eprintf ("   image high y:  %5d %5d %5d %5d inches\n")
		do i = 1, NASPECT
		    call pargr (imymx[i])
	}

	# Set labels
	call strcpy (HPWR, INFOLABEL(HVAL), LWORD)
	call strcpy (HTMP, INFOLABEL(HTVAL), LWORD)
	call strcpy (TTMP, INFOLABEL(TVAL), LWORD)
	call strcpy (BASE, ASPECTLABEL(BVAL), LWORD)
	call strcpy (LID, ASPECTLABEL(LVAL), LWORD)
	call strcpy (WALL, ASPECTLABEL(WVAL), LWORD)
	call strcpy (MOLD, ASPECTLABEL(MVAL), LWORD)
	call strcpy (ALUM, ASPECTLABEL(AVAL), LWORD)
end
