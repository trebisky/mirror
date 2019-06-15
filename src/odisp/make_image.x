include <error.h>
include <imhdr.h>
include "odisp.h"
include "tc_he.h"
include "tc_he_bounds.h"
include "../imagemask/mask.h"
include "../xytran/tran.h"

# MAKE_IMAGE.X - form image from data values


procedure make_image (imname, infono, aspectno, date, timen, interpolate,
		      showtc, data, ldata, x, y, z, imsp)

char	imname[SZ_PATHNAME]	# output image name
int	infono, aspectno	# identifies information and aspect
long	date			# date:  YYMMDD
int	timen			# time location (1 .. 1440)
bool	interpolate		# smooth the image?
bool	showtc			# show the thermocouple locations?
real	data[ldata]		# data array for this information
int	ldata			# length of data array
double	x[ARB], y[ARB], z[ARB]	# working arrays for image information
double	imsp[ARB]		# space to form image

int	i, k, iloc, ier, temp, off, ix, iy
int	n			# number of x, y, z data values
int	npixx, npixy		# image (imsp) size in pixels
double	xmin, xmax, ymin, ymax	# x, y image limit values
double	zmin, zmax		# min and max input values
int	xoffset, yoffset	# image offset in image space
int	xsize, ysize		# image size
double	tcval, bgval		# thermocouple and background values for image
real	xsz, ysz, midx, midy
pointer	im

int	imaccess()
pointer immap(), imps2d(), impl2d()

errchk	impl2d, imps2d

include "tc_he.com"
include "imdat.com"
include "../imagemask/mask.com"
include "debugd.com"

begin
	ndebug = 0
	n = 0
	if (debug) {
	    call eprintf("get info\n")
	}

	do i = 1, num[aspectno, infono] {
	    iloc = loc[i, aspectno, infono]
	    if (iloc >= 1 && iloc <= ldata)
	        if (data[iloc] != INDEFR) {
		    n = n + 1
		    x[n] = xpos[i, aspectno, infono]
		    y[n] = ypos[i, aspectno, infono]
		    z[n] = data[iloc]
	        } else
		    ndebug = ndebug + 1
	}
	if (debug) {
	    call eprintf ("%d good points, %d INDEFs\n")
		call pargi (n)
		call pargi (ndebug)
	}

	# Find minimum and maximum input values.
	call alimd (z, n, zmin, zmax)
	if (zmin <= 0.D0)
	    bgval = -1.D0
	else
	    bgval = 0.D0

	xoffset = imoffx[aspectno]
	yoffset = imoffy[aspectno]
	xsize = imszx[aspectno]
	ysize = imszy[aspectno]
	npixx = imsmx[aspectno]
	npixy = imsmy[aspectno]

	if (interpolate) {

	    # There must be at least so many points for the smooth routine.
	    if (n < MINSMOOTH) {
	        call eprintf ("Number of data points = %d ( < %d )\n")
		    call pargi (n)
		    call pargi (MINSMOOTH)
	        call error (15, "Too few data points to smooth")
	    }
    
	    xmin = imxmn[aspectno]
	    xmax = imxmx[aspectno]
	    ymin = imymn[aspectno]
	    ymax = imymx[aspectno]
	    call smooth (x, y, z, n, npixx, npixy, imsp, xmin, xmax, ymin, ymax, ier)
	    if (ier != 0) {
	        call eprintf ("Smooth routine error return = %d\n")
		    call pargi (ier)
	        call error (18, "Smooth routine failed")
	    }
	
	    # Mask off non-physical part of image.
	    do i = 1, ysize {
		off = yoffset * npixx + (i - 1) * npixx + xoffset + 1
		do k = 0, xsize - 1
		    if (OMASK(k+1, i, aspectno) == -1)
			imsp[off + k] = bgval
	    }

	} else {

	    do i = 1, ysize {
		off = yoffset * npixx + (i - 1) * npixx + xoffset + 1
		do k = 0, xsize - 1 {
		    iloc = OMASK(k+1, i, aspectno)
		    if (iloc <= 0)
			imsp[off + k] = bgval
		    else
			if (data[iloc] != INDEFR)
			    imsp[off + k] = double (data[iloc])
			else
			    imsp[off + k] = double (2000.0)
		}
	    }
	}


	if (showtc) {
	    if (debug)
		call eprintf ("show thermocouples at\n")
	    if (zmax <= 0.D0)
		tcval = 5.D0
	    else
		tcval = max (1.1D0 * zmax, 5.0D0 + zmin)
	    xsz = xsize
	    ysz = ysize
	    midx = (xsize + 1.) / 2.
	    midy = (ysize + 1.) / 2.

	    do i = 1, num[aspectno, TVAL] {
		if (aspectno == WVAL) {
		    ix = midx + ((xpos[i, aspectno, TVAL] - CENTERWALLT) /
				WALLSIZET) * xsz + 0.5
		    iy = midy + ((ypos[i, aspectno, TVAL] - CENTERWALLZ) /
				WALLSIZEZ) * ysz + 0.5
		} else {
		    ix = midx + (xpos[i, aspectno, TVAL] / DIAMLBM) * xsz + 0.5
		    iy = midy + (ypos[i, aspectno, TVAL] / DIAMLBM) * ysz + 0.5
		}
		off = yoffset * npixx + (iy - 1) * npixx + xoffset
		imsp[off + ix] = tcval
		if (debug) {
		    call eprintf ("%.2f %.2f at %d %d\n")
			call pargr (xpos[i, aspectno, TVAL])
			call pargr (ypos[i, aspectno, TVAL])
			call pargi (ix)
			call pargi (iy)
		}
	    }
	}

	if (imaccess (imname, READ_ONLY) == YES)
	    call imdelete (imname)
	
	im = immap (imname, NEW_IMAGE, 0)

	IM_NDIM(im) = 2
	IM_LEN(im, 1) = xsize
	IM_LEN(im, 2) = ysize
	IM_PIXTYPE(im) = TY_REAL
	temp = (timen - 1) / 60
	temp = temp * 100 + ((timen - 1) - temp * 60)
	call sprintf (IM_TITLE(im), SZ_IMTITLE, "%s %s %06d %04d")
	    call pargstr (INFOLABEL(infono))
	    call pargstr (ASPECTLABEL(aspectno))
	    call pargl (date)
	    call pargi (temp)
	if (debug) {
	    call eprintf ("  title = '%s'\n")
		call pargstr (IM_TITLE (im))
	}

	# Set keywords for minimum and maximum input values.
	call imaddr (im, INMIN_KW, real (zmin))
	call imaddr (im, INMAX_KW, real (zmax))
	if (!silent || debug) {
	    call printf ("data min/max = %.2f, %.2f\n")
	        call pargr (real (zmin))
	        call pargr (real (zmax))
	}
	if (debug) {
	    call eprintf ("background value = %.2f")
		call pargr (real (bgval))
	    if (showtc) {
		call eprintf (", and thermocouple value = %.2f")
		    call pargr (real (tcval))
	    }
	    call eprintf ("\n")
	}

	# Write out image.
	if (xoffset == 0 && yoffset == 0)		# easy
	    call amovd (imsp, Memd[imps2d (im, 1, xsize, 1, ysize)],
		    xsize * ysize)
	else
	    do i = 1, ysize
		call amovd (imsp[yoffset*npixx + (i-1)*npixx + xoffset + 1],
			Memd[impl2d (im, i)], xsize)
	
	call imunmap (im)
end
