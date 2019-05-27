include <imhdr.h>
include <time.h>
include "odisp.h"
include "tc_he.h"

# T_ODISP.X - Code for the oven task odisp.


procedure t_odisp()

bool	initialized, display, auto, interpolate, showtc, have_name
char	info[LWORD], aspect[LWORD], when[LWORD], infoc
char	dfname[SZ_FNAME], cmd[SZ_LINE], filename[SZ_PATHNAME]
char	dir[SZ_PATHNAME]
char	image[SZ_PATHNAME], datadir[SZ_PATHNAME]
int	now, infono, aspectno, frame, order, showtcval, interval
int	time, timen, tm[LEN_TMSTRUCT], imageno, imagetim[NIMAGE]
int	imagestc[NIMAGE], imageinter[NIMAGE]
int	firstcol, lastcol, match, ytop, i, j, list, nlist
long	date, temp, imagedat[NIMAGE]
real	z1, z2, xmag, ymag, imz1, imz2, imzdiff
pointer im, column, sp, imsp
int	quick

bool	clgetb()
int	clgeti(), clgwrd(), imgeti(), imaccess(), imaccf()
int	clpopnu(), clplen(), clgfil()
long	clgetl(), clktime()
real	clgetr(), imgetr()
pointer immap(), imgs2r()

data	initialized /false/, imagetim /NIMAGE*-1/, imagestc /NIMAGE*-1/
data	imagedat /NIMAGE*-1/, imageinter /NIMAGE*-1/

include	"imdat.com"
include "debugd.com"

errchk	init_info, init_imdat, immap, imgs2r, imunmap, salloc, clcmdw

begin
	debug = clgetb ("debug")
	# Initialize image data first time only.
	if (!initialized) {
	    if (debug)
		call eprintf ("Initializing image data\n")
	    call init_imdat()
	    initialized = true
	}
	call init_info()

	# Get parameters from the cl.

	infono = clgwrd ("information", info, LWORD, INFOLIST)
	if (debug) {
	    call eprintf ("  info = '%s' = %d\n")
		call pargstr (info)
		call pargi (infono)
	}
	aspectno = clgwrd ("aspect", aspect, LWORD, ASPECTLIST)
	if (debug) {
	    call eprintf ("aspect = '%s' = %d\n")
		call pargstr (aspect)
		call pargi (aspectno)
	}
	if (infono == HVAL && aspectno == MVAL) {
	    call eprintf ("'hpwr' in 'mold' is not allowed\n")
	    return
	}

	date = clgetl ("date")
	if (IS_INDEFL (date)) {
	    date = clktime (0)
	    call brktime (date, tm)
	    date = (mod (tm[TM_YEAR], 100) * 100 + tm[TM_MONTH]) * 100 +
					tm[TM_MDAY]
	    timen = tm[TM_HOUR]*60 + tm[TM_MIN] + 1
	    now = YES
	} else
	    now = NO
	if (debug) {
	    call eprintf ("  date = %06d\n")
		call pargl (date)
	}

	time = clgeti ("time")
	if (debug) {
	    call eprintf ("  time = %d (hhmm)\n")
		call pargi (time)
	}
	if (!IS_INDEFI (time)) {
	    temp = time / 100
	    timen = temp * 60 + (time - temp * 100) + 1
	    now = NO
	    if (debug) {
		call eprintf ("  time location = column %d\n")
		    call pargi (timen)
	    }
	}

	display = clgetb ("display")
	if (display)
	    frame = clgeti ("frame")
	auto = clgetb ("auto")
	z1 = clgetr ("z1")
	z2 = clgetr ("z2")

	showtc = clgetb ("showtc")
	if (showtc)
	    showtcval = 1
	else
	    showtcval = 0

	if (infono == TVAL)
	    interpolate = true
	else
	    interpolate = clgetb ("interpolate")

	if (interpolate)
	    interval = 1
	else
	    interval = 0

	order = clgeti ("order")

	list = clpopnu ("searchdir")
	if (debug) {
	    nlist = clplen (list)
	    call eprintf ("search dir length = %d\n")
		call pargi (nlist)
	    do i = 1, nlist {
		j = clgfil (list, dir, SZ_PATHNAME)
		call eprintf ("  %s\n")
		    call pargstr (dir)
	    }
	}

	call clgstr ("datadir", datadir, SZ_PATHNAME)
	silent = clgetb ("silent")
	if (debug) {
	    call eprintf ("  display  auto  interpolate  showtc  silent\n")
	    call eprintf ("    %3b    %3b       %3b       %3b     %3b\n")
		call pargb (display)
		call pargb (auto)
		call pargb (interpolate)
		call pargb (showtc)
		call pargb (silent)
	    if (display) {
		call eprintf ("  display frame = %d, order = %d\n")
		    call pargi (frame)
		    call pargi (order)
	    }
	    call eprintf ("  z range:  %.1f - %.1f\n")
		call pargr (z1)
		call pargr (z2)
	}

	call sprintf (dfname, SZ_FNAME, "%s%06d")
	    call pargstr (INFOLABEL(infono))
	    call pargl (date)
	if (debug) {
	    call eprintf ("data file name: '%s'\n")
		call pargstr (dfname)
	}

	# find which image to use, the 2 is for "now" and "then"
	imageno = ((infono - 1) * NASPECT + (aspectno - 1)) * 2
	if (now == YES) {
	    imageno = imageno + 1
	    call strcpy ("now", when, LWORD)
	} else {
	    imageno = imageno + 2
	    call strcpy ("then", when, LWORD)
	}
	if (debug) {
	    call eprintf ("image # = %d, date = %d, time location = column %d\n")
		call pargi (imageno)
		call pargl (imagedat[imageno])
		call pargi (imagetim[imageno])
	}

	switch (infono) {
	case HVAL:
	    infoc = 'p'
	case HTVAL:
	    infoc = 'h'
	case TVAL:
	    infoc = 't'
	}

	call sprintf (image, SZ_PATHNAME, "%s%c%c%s")
	    call pargstr (datadir)
	    call pargc (infoc)
	    call pargc (ASPECTLABEL(aspectno))
	    call pargstr (when)
	if (debug) {
	    call eprintf ("datadir = '%s'\n")
		call pargstr (datadir)
	    call eprintf ("image name = '%s'\n")
		call pargstr (image)
	}

	quick = now
	if (quick == NO) {
	    have_name = false
	    call clprew (list)
	    nlist = clplen (list)
	    do i = 0, nlist {
		if (i == 0)
		    call strcpy (dfname, filename, SZ_PATHNAME)
		else {
		    j = clgfil (list, dir, SZ_PATHNAME)
		    if (j == EOF) {
			call eprintf ("cannot happen:  search directory list is short\n")
			break
		    } else {
			call sprintf (filename, SZ_PATHNAME, "%s/%s")
			    call pargstr (dir)
			    call pargstr (dfname)
		    }
		}
		if (debug) {
		    call eprintf ("   try filename '%s'\n")
			call pargstr (filename)
		}
		if (imaccess (filename, READ_ONLY) == YES) {
		    have_name = true
		    break
		}
	    }
	    call clpcls (list)
	    if (!have_name) {
		call eprintf ("Cannot find database file '%s'\n")
		    call pargstr (dfname)
		return
	    }

	    im = immap (filename, READ_ONLY, 0)
	    if (IM_LEN (im, 1) > DLEN)
		ytop = IM_LEN (im, 2)
	    else
		ytop = IM_LEN (im, 1)
	    if (debug) {
		call eprintf ("  column length is 1 .. %d\n")
		    call pargi (ytop)
	    }

	    if (imaccf (im, FIRSTCOL_KW) == NO || imaccf (im, LASTCOL_KW) == NO) {
		call eprintf ("Keyword missing in '%s'\n")
		    call pargstr (dfname)
		call imunmap (im)
		return
	    }
	    firstcol = imgeti (im, FIRSTCOL_KW)
	    lastcol = imgeti (im, LASTCOL_KW)
	    if (debug) {
		call eprintf ("  good columns are %d .. %d\n")
		    call pargi (firstcol)
		    call pargi (lastcol)
	    }

	    if (IS_INDEFI (time)) {
		timen = lastcol
		if (debug) {
		    call eprintf ("  time location = column %d\n")
			call pargi (timen)
		}
	    }

	    if (timen < firstcol || timen > lastcol) {
		call eprintf ("Data not available\n")
		call imunmap (im)
		return
	    }
	}

	if (timen == imagetim[imageno] && date == imagedat[imageno] &&
	    showtcval == imagestc[imageno] && interval == imageinter[imageno]) {
	    if (imaccess (image, READ_ONLY) == YES)
	        match = YES
	    else
		match = NO
	} else
	    match = NO
	if (debug) {
	    if (match == NO)
		call eprintf ("no ")
	    call eprintf ("match with image #\n")
	}

	if (match == YES) {
	    if (quick == NO) {
		call imunmap (im)
	    }
	} else {
	    call smark (sp)
	    call salloc (column, DLEN, TY_REAL)
	    if (quick == NO) {
		if (IM_LEN (im, 1) > DLEN)
		    call amovr (Memr[imgs2r (im, timen, timen, 1, ytop)],
			Memr[column], ytop)
		else
		    call amovr (Memr[imgs2r (im, 1, ytop, timen, timen)],
			Memr[column], ytop)
		call imunmap (im)
	    } else {
		switch (infono) {
		case HVAL:
		    call ghpnow (Memr[column], ytop)
		case HTVAL:
		    call ghtnow (Memr[column], ytop)
		case TVAL:
		    call gttnow (Memr[column], ytop)
		}
	    }

	    if (debug) {
		call eprintf ("column as read:")
		ndebug = 0
		do i = 0, ytop - 1 {
		    if (mod (i, 10) == 0)
			call eprintf ("\n")
		    call eprintf ("%7.1f")
			call pargr (Memr[column + i])
		    if (IS_INDEFR (Memr[column + i]))
			ndebug = ndebug + 1
		}
		call eprintf ("\nthere are %d INDEFs\n")
		    call pargi (ndebug)
	    }

	    # Check that some values exist
	    temp = 0
	    do i = 0, ytop - 1
		if (Memr[column + i] != INDEFR) {
		    temp = temp + 1
		    break
		}
	    if (temp == 0) {
	        call sfree (sp)
		call eprintf ("No data available\n")
		return
	    }

	    call salloc (imsp, 3 * DLEN + MAXSMAREA, TY_DOUBLE)
	    iferr {
	        call make_image (image, infono, aspectno, date, timen,
		        interpolate, showtc, Memr[column], ytop,
			Xsp, Ysp, Zsp, Isp)
	        call sfree (sp)
	    } then
		return
	    imagetim[imageno] = timen
	    imagedat[imageno] = date
	    imagestc[imageno] = showtcval
	    imageinter[imageno] = interval
	}

	if (display) {
	    if (auto || z1 == INDEFR || z2 == INDEFR) {
		# Get minimum and maximum values from image header.
		im = immap (image, READ_ONLY, 0)
		imz1 = imgetr (im, INMIN_KW)
		imz2 = imgetr (im, INMAX_KW)
		imzdiff = max (abs (imz2 - imz1), MIN_INTENSITY_DIFF)
		if (auto || z1 == INDEFR)
		    z1 = imz1 - 0.1 * imzdiff
		if (auto || z2 == INDEFR)
		    z2 = imz2 + 0.1 * imzdiff
		call imunmap (im)
		if (debug) {
		    call eprintf ("z min/max = %8.1f, %8.1f\n")
			call pargr (z1)
			call pargr (z2)
		}
		if (auto && ((z2 - z1) < MIN_INTENSITY_DIFF)) {
		    z1 = z1 - (MIN_INTENSITY_DIFF - (z2 - z1))/2.
		    z2 = z1 + MIN_INTENSITY_DIFF
		}
	    }
	    if (debug) {
		call eprintf ("z min/max = %8.1f, %8.1f\n")
		    call pargr (z1)
		    call pargr (z2)
	    }
	    xmag = XSZWINDOW / imszx(aspectno)
	    if (aspectno == WVAL)
		ymag = WALL_HT_MAG * xmag
	    else
	    	ymag = YSZWINDOW / imszy(aspectno)
	    call sprintf (cmd, SZ_LINE,
		"display image=%s frame=%d order=%d zscale- zrange- xmag=%.2f ymag=%.2f z1=%.2f z2=%.2f")
		call pargstr (image)
		call pargi (frame)
		call pargi (order)
		call pargr (xmag)
		call pargr (ymag)
		call pargr (z1)
		call pargr (z2)
	    if (silent && !debug)
		call strcat (" > dev$null", cmd, SZ_LINE)
	    if (debug) {
		call eprintf ("cmd: %s\n")
		    call pargstr (cmd)
	    }

	    call clcmdw (cmd)
	}
end
