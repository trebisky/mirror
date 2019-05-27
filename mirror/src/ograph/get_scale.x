include <imhdr.h>
include <mach.h>
include "ograph.h"

# GET_SCALE.X - find scaling limits for ograph, make lists, return success.

define	MINUTES_PER_HOUR    60
define	MINUTES_PER_DAY	  1440

define	EPS		1.E-4
define	ONEPLUS		(1. + EPS)
define	ONEMINUS	(1. - EPS)

int procedure get_scale (type, index, fname, list, ndays, datel, timel, timer,
			 timeunit, yhorg, yhoff, ylorg, yloff,
			 tmin, tmax, ymin, ymax)

int	type[N_LINES]		# array of information types
int	index[N_LINES]		# array of locations of data
char	fname[LWORD, N_LINES]	# array of filenames which hold data
int	list			# search list of directories
long	ndays			# number of days to graph
long	datel			# date at beginning of information (yymmdd)
long	timel, timer		# time limits for x scale (hhmm)
int	timeunit		# time unit:  hours or days
int	yhorg, ylorg		# y limit origins
real	yhoff, yloff		# y limit offsets
real	tmin, tmax		# returned x (time) limits
real	ymin, ymax		# returned y limits

bool	dbopen, need_scale, any_data, have_name, need_first, try_db
char	filename[SZ_PATHNAME], dir[SZ_PATHNAME]
int	i, j, j1, j2, l, n, firstcol, lastcol, t1, t2, tl, tr, fd,
	nlist, il, stat
long	datelm, daterm, ndaysm, datem, timelm, timerm, date, time, tval,
	d, xmin, xmax, dbt0, dbt1
real	val, dmin, dmax, dayl, tscale, toff, xl, xr, yl, yr
real	cx1, cx2, cy1, cy2			# clipped coordinates
real	wx1, wx2, wy1, wy2			# window limits
pointer	sp, line, im
pointer	col

int	imgeti(), imaccf(), open(), access(), imaccess(), time_number(),
	clplen(), clgfil(), clip_line()
long	date_to_m()
pointer	immap()
#pointer	imgl2r()
pointer	imgs2r()

include "ograph.com"
include "debugg.com"

errchk	immap, imunmap, salloc
#errchk	imgl2r
errchk	imgs2r

begin
	xmin = MAX_LONG
	xmax = -MAX_LONG
	dmin = MAX_REAL
	dmax = -MAX_REAL

	tl = time_number (int (timel))
	tr = time_number (int (timer))
	if (!IS_INDEFI (tl))
	    tl = tl + 1
	if (!IS_INDEFI (tr))
	    tr = tr + 1

	if (yhorg == ZERODATA && ylorg == ZERODATA) {
	    if (IS_INDEFR (yhoff) || IS_INDEFR (yloff))
		# have to find average, min, and/or max
		need_scale = true
	    else
		need_scale = false
	} else
	    # origins refer to min and/or max
	    need_scale = true

	call smark (sp)
	call salloc (line, LINE_LENGTH, TY_REAL)

	do l = 1, N_LINES {
	    if (access (tmp_file[1, l], 0, 0) == YES)
		call delete (tmp_file[1, l])
	}

	any_data = false
	datelm = date_to_m (datel)
	ndaysm = ndays * MINUTES_PER_DAY
	daterm = datelm + ndaysm - 1
	dayl = mod (datel, 100)
	if (!IS_INDEFI (tl))
	    timelm = datelm + tl - 1
	else
	    timelm = datelm
	if (!IS_INDEFI (tr))
	    timerm = daterm - MINUTES_PER_DAY + tr - 1
	else
	    timerm = daterm

	if (timeunit == TUHOURS) {
	    tscale = 1. / real (MINUTES_PER_HOUR)
	    toff = 0.
	} else {
	    tscale = 1. / real (MINUTES_PER_DAY)
	    toff = dayl
	}

	# Consider any curves of XTMP data.

	do l = 1, N_LINES {
	    if (type[l] == XTMP) {
		if (debug) {
		    call eprintf ("  graph line %d:  read XTMP line %d\n")
			call pargi (l)
			call pargi (index[l])
		}
		i = index[l]
		n = db_n[i]
		if (n >= 2) {

		    # If the XTMP curve crosses the left time edge, find that
		    # intersection.

		    if (db_time[1, i] <= timelm && db_time[n, i] > timelm) {
			any_data = true
			xmin = min (xmin, timelm - datelm + 1)
			do j = 1, n - 1 {
			    dbt0 = db_time[j, i]
			    dbt1 = db_time[j + 1, i]
			    if (dbt0 <= timelm && dbt1 >= timelm) {
				if (dbt0 == timelm)
				    val = db_val[j, i]
				else
				    val = db_val[j, i] +
					  (db_val[j+1, i] - db_val[j, i]) *
					  real (timelm - dbt0) /
					  real (dbt1 - dbt0)
				val = val * factor[l]
				if (need_scale) {
				    dmin = min (dmin, val)
				    dmax = max (dmax, val)
				}
				break
			    }
			}
		    }

		    # If the XTMP curve crosses the right time edge, find that
		    # intersection.

		    if (db_time[1, i] < timerm && db_time[n, i] >= timerm) {
			any_data = true
			xmax = max (xmax, timerm - datelm + 1)
			do j = 1, n - 1 {
			    dbt0 = db_time[j, i]
			    dbt1 = db_time[j + 1, i]
			    if (dbt0 <= timerm && dbt1 >= timerm) {
				if (dbt1 == timerm)
				    val = db_val[j + 1, i]
				else
				    val = db_val[j, i] +
					  (db_val[j+1, i] - db_val[j, i]) *
					  real (timerm - dbt0) /
					  real (dbt1 - dbt0)
				val = val * factor[l]
				if (need_scale) {
				    dmin = min (dmin, val)
				    dmax = max (dmax, val)
				}
				break
			    }
			}
		    }
		}

		# Get limits of all the XTMP points within the time range.

		do j = 1, n {
		    dbt0 = db_time[j, i]
		    if (dbt0 >= timelm && dbt0 <= timerm) {
			any_data = true
			tval = dbt0 - datelm
			val = db_val[j, i] * factor[l]
			xmin = min (xmin, tval + 1)
			xmax = max (xmax, tval + 1)
			if (need_scale) {
			    dmin = min (dmin, val)
			    dmax = max (dmax, val)
			}
		    }
		}
	    }
	}

	# Consider data which is not XTMP; do each day.

	do d = 1, ndays {
	    if (d == 1)
		t1 = tl
	    else
		t1 = 1
	    if (d == ndays)
		t2 = tr
	    else
		t2 = MINUTES_PER_DAY
	    datem = datelm + (d - 1) * MINUTES_PER_DAY
	    call minutes_to_dt (datem, date, time)
	    time = (d - 1) * MINUTES_PER_DAY
	    if (debug) {
		call eprintf ("date = %06d  n1 = %d  n2 = %d  timeoffset = %d\n")
		    call pargl (date)
		    call pargi (t1)
		    call pargi (t2)
		    call pargl (time)
	    }

	    do i = 1, N_DATA {
		if (i == XTMP)
		    next
		dbopen = false
		try_db = true
		do l = 1, N_LINES {
		    if (type[l] == i) {
			if (!dbopen) {
			    have_name = false
			    if (try_db) {
				call clprew (list)
				nlist = clplen (list)
				do il = 0, nlist {
				    if (il == 0) {
					call sprintf (filename, SZ_PATHNAME, "%s%06d")
					    call pargstr (fname[1, l])
					    call pargl (date)
				    } else {
					stat = clgfil (list, dir, SZ_PATHNAME)
					if (stat == EOF) {
					    call eprintf ("get_scale(): cannot happen: search directory list is short\n")
					    break
					} else {
					    call sprintf (filename, SZ_PATHNAME, "%s/%s%06d")
						call pargstr (dir)
						call pargstr (fname[1, l])
						call pargl (date)
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
			    }
			    if (!have_name) {
				if (debug && try_db) {
				    call eprintf ("cannot access database '%s%06d'\n")
					call pargstr (fname[1, l])
					call pargl (date)
				}
				if (access (tmp_file[1, l], 0, 0) == YES) {
				    fd = open (tmp_file[1, l], APPEND, TEXT_FILE)
				    call fprintf (fd, "%.6g %.6g\n")
					call pargr (real (time) * tscale + toff)
					call pargr (INDEFR)
				    call close (fd)
				}
				try_db = false
				next
			    }
			    if (debug) {
				call eprintf ("Opening file '%s'\n")
				    call pargstr (filename)
			    }
			    im = immap (filename, READ_ONLY, 0)
			    dbopen = true
			    if (imaccf (im, FIRSTCOL_KW) == NO ||
				imaccf (im, LASTCOL_KW) == NO) {
				call eprintf ("Keyword missing in '%s'\n")
				    call pargstr (filename)
				call imunmap (im)
				dbopen = false
				next
			    }
			    firstcol = imgeti (im, FIRSTCOL_KW)
			    lastcol = imgeti (im, LASTCOL_KW)
			    xmin = min (xmin, long (firstcol) + time)
			    xmax = max (xmax, long (lastcol) + time)
			    if (IS_INDEFI (t1))
				j1 = firstcol - 1
			    else
				j1 = t1 - 1
			    if (IS_INDEFI (t2))
				j2 = lastcol - 1
			    else
				j2 = t2 - 1
			}
			fd = open (tmp_file[1, l], APPEND, TEXT_FILE)
			if (debug) {
			    call eprintf ("Temporary file '%s'\n")
				call pargstr (tmp_file[1, l])
			}
			ndebug = 0
			if (debug) {
			  call eprintf("  graph line %d:  read image line %d\n")
				call pargi (l)
				call pargi (index[l])
			}
			if (IM_LEN(im, 1) == LINE_LENGTH) {
			    col = imgs2r(im, 1, LINE_LENGTH, index[l], index[l])
			} else {
			    col = imgs2r(im, index[l], index[l], 1, LINE_LENGTH)
			}
			call amovr (Memr[col], Memr[line], LINE_LENGTH)
			do j = j1, j2, subsample {
			    val = Memr[line + j]
			    if (!IS_INDEFR (val)) {
				any_data = true
				val = val * factor[l]
				if (need_scale) {
				    dmin = min (dmin, val)
				    dmax = max (dmax, val)
				}
			    } else
				ndebug = ndebug + 1
			    tval = time + long (j)
			    call fprintf (fd, "%.6g %.6g\n")
				call pargr (real (tval) * tscale + toff)
				call pargr (val)
			}
			if (debug) {
			    call eprintf ("  %d INDEFs\n")
				call pargi (ndebug)
			}
			call close (fd)
		    }
		}

		if (dbopen) {
		    call imunmap (im)
		    dbopen = false
		}
	    }
	}

	call sfree (sp)

	if (!any_data) {
	    if (debug)
	        call eprintf ("no data to graph\n")
	    return (NO)
	}

	if (debug) {
	    call eprintf ("Image limits x:  %d to %d\n")
		call pargl (xmin)
		call pargl (xmax)
	    call eprintf ("             y:  %.1f to %.1f\n")
		call pargr (dmin)
		call pargr (dmax)
	}
	if (!IS_INDEFI (tl))
	    xmin = tl
	if (!IS_INDEFI (tr))
	    xmax = (ndays - 1) * MINUTES_PER_DAY + long (tr)

	tmin = real (xmin - 1) * tscale + toff
	tmax = real (xmax - 1) * tscale + toff

	if (yhorg == ZERODATA) {
	    if (IS_INDEFR (yhoff))
		ymax = dmax
	    else
		ymax = yhoff
	} else {
	    if (IS_INDEFR (yhoff))
		yhoff = 0.
	    if (yhorg == AVERAGE)
		ymax = (dmin + dmax) / 2. + yhoff
	    else if (yhorg == MINDATA)
		ymax = dmin + yhoff
	    else
		ymax = dmax + yhoff
	}

	if (ylorg == ZERODATA) {
	    if (IS_INDEFR (yloff))
		ymin = dmin
	    else
		ymin = yloff
	} else {
	    if (IS_INDEFR (yloff))
		yloff = 0.
	    if (ylorg == AVERAGE)
		ymin = (dmin + dmax) / 2. + yloff
	    else if (ylorg == MINDATA)
		ymin = dmin + yloff
	    else
		ymin = dmax + yloff
	}

	# Clip the lines for the XTMP data.  This is necessary because 'graph'
	# improperly clips lines which intersect the top or bottom window
	# boundary.  To stay away from these limits, we clip to a window
	# slightly smaller (EPS) than the actual window.

	datelm = date_to_m (datel)
	if (tmin < tmax) {
	    wx1 = tmin * ONEPLUS
	    wx2 = tmax * ONEMINUS
	} else {
	    wx1 = tmax * ONEMINUS
	    wx2 = tmin * ONEPLUS
	}
	if (ymin < ymax) {
	    wy1 = ymin * ONEPLUS
	    wy2 = ymax * ONEMINUS
	} else {
	    wy1 = ymax * ONEMINUS
	    wy2 = ymin * ONEPLUS
	}
	do l = 1, N_LINES {
	    if (type[l] == XTMP) {
		fd = open (tmp_file[1, l], APPEND, TEXT_FILE)
		if (debug) {
		    call eprintf ("temporary file '%s' for XTMP\n")
			call pargstr (tmp_file[1, l])
		}
		i = index[l]
		n = db_n[i]
		if (n >= 2) {
		    need_first = true
		    do j = 1, n-1 {
			xl = real (db_time[j, i] - datelm) * tscale + toff
			xr = real (db_time[j+1, i] - datelm) * tscale + toff
			yl = db_val[j, i] * factor[l]
			yr = db_val[j+1, i] * factor[l]
			if (clip_line (xl, yl, xr, yr, wx1, wx2, wy1, wy2,
						cx1, cy1, cx2, cy2) == YES) {
			    if (need_first) {
				call fprintf (fd, "%.6g %.6g\n")
				    call pargr (cx1)
				    call pargr (cy1)
			    }
			    call fprintf (fd, "%.6g %.6g\n")
				call pargr (cx2)
				call pargr (cy2)
			    need_first = false
			} else
			    need_first = true
		    }
		}
		call close (fd)
	    }
	}
	if (debug) {
	    call eprintf ("Adopted limits x:  %d to %d\n")
		call pargl (xmin)
		call pargl (xmax)
	    call eprintf ("               t:  %.3f to %.3f\n")
		call pargr (tmin)
		call pargr (tmax)
	    call eprintf ("               y:  %.1f to %.1f\n")
		call pargr (ymin)
		call pargr (ymax)
	}
	
	return (YES)
end


# TIME_NUMBER - convert hhmm to minutes, watch for INDEF.

int procedure time_number (t)

int	t			# time in hhmm format

int	temp

begin
	if (IS_INDEFI (t))
	    return (INDEFI)

	temp = t / 100
	temp = temp * 60 + (t - temp * 100)
	return (temp)
end
