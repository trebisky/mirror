include <time.h>
include "ograph.h"

define	MIN_PER_DAY	1440

# T_OGRAPH.X - Code for the oven task ograph, plot time dependencies of data.


procedure t_ograph()

char	info[LWORD], word[LWORD], fname[LWORD, N_LINES], cmd[LEN_CMD],
	datadir[SZ_PATHNAME], origin[LWORD], tustr[LWORD]
bool	append
int	i, j, wcs, key, n, list
int	tm[LEN_TMSTRUCT]
int	index[N_LINES], info_type[N_LINES]
int	yhorg, ylorg, tlorg, trorg, timeunit
long	date, datem, time, ndays, timezpt
long	datel, dater, tloff, troff, timel, timer
real	timenmin, timenmax, wx, wy
real	ymin, ymax, yhoff, yloff

bool	clgetb(), streq()
int	clgeti(), clgwrd(), get_index(), strdic(), strlen(), get_scale(),
	clgcur(), clpopnu(), clgfil(), clplen(), access()
long	clgetl(), clktime(), date_to_m(), time_to_m()
real	clgetr()

include	"ograph.com"
include "debugg.com"

errchk	init_info, get_index, get_scale, clcmdw

begin
	debug = clgetb ("debug")
	call init_info()

	# Get parameters from the cl.

	date = clktime (0)
	call brktime (date, tm)
	date = (mod (tm[TM_YEAR], 100) * 100 + tm[TM_MONTH]) * 100 +
		    tm[TM_MDAY]
	time = (tm[TM_HOUR] * 100) + tm[TM_MIN]

	datel = clgetl ("dateleft")
	dater = clgetl ("dateright")

	if (IS_INDEFL (datel))
	    datel = date
	if (IS_INDEFL (dater))
	    dater = date
	
	if (debug) {
	    call eprintf ("date:  left = %06d  right = %06d\n")
		call pargl (datel)
		call pargl (dater)
	}

	call clgstr ("timelorigin", word, LWORD)
	if (strlen (word) == 0)
	    tlorg = TIMEDATE
	else {
	    tlorg = strdic (word, origin, LWORD, TORIGIN_LIST)
	    if (tlorg <= 0) {
		call eprintf ("improper value for parameter timelorigin = '%s'\n")
		    call pargstr (word)
		return
	    }
	}

	call clgstr ("timerorigin", word, LWORD)
	if (strlen (word) == 0)
	    trorg = TIMEDATE
	else {
	    trorg = strdic (word, origin, LWORD, TORIGIN_LIST)
	    if (trorg <= 0) {
		call eprintf ("improper value for parameter timerorigin = '%s'\n")
		    call pargstr (word)
		return
	    }
	}

	if (debug) {
	    call eprintf ("time origin:  left = %d  right = %d\n")
		call pargi (tlorg)
		call pargi (trorg)
	}

	tloff = clgetl ("timeloffset")
	if (!IS_INDEFL (tloff))
	    if (tlorg == TIMEDATE && (tloff < 0 || tloff > 2359)) {
		call eprintf ("timeloffset = %d, must be in range [0 .. 2359]\n")
		    call pargl (tloff)
		return
	    }

	troff = clgetl ("timeroffset")
	if (!IS_INDEFL (troff))
	    if (trorg == TIMEDATE && (troff < 0 || troff > 2359)) {
		call eprintf ("timeroffset = %d, must be in range [0 .. 2359]\n")
		    call pargl (troff)
		return
	    }
	
	if (debug) {
	    call eprintf ("time offset:  left = %d  right = %d\n")
		call pargl (tloff)
		call pargl (troff)
	}

	timel = tloff			# this is for origin = ""
	timer = troff

	datem = date_to_m (date) + time_to_m (time)

	if (tlorg == TIMENOW) {
	    if (IS_INDEFL (tloff))
		datel = date
	    else {
		time = time_to_m (tloff)
		time = datem + time
		call minutes_to_dt (time, datel, timel)
	    }
	}

	if (trorg == TIMENOW) {
	    if (IS_INDEFL (troff))
		dater = date
	    else {
		time = time_to_m (troff)
		time = datem + time
		call minutes_to_dt (time, dater, timer)
	    }
	}

	ndays = (date_to_m (dater) - date_to_m (datel)) / MIN_PER_DAY
	ndays = abs (ndays) + 1

	if (debug) {
	    call eprintf ("time interval:  %d %d  to  %d %d:  %d days\n")
		call pargl (datel)
		call pargl (timel)
		call pargl (dater)
		call pargl (timer)
		call pargl (ndays)
	}

	call clgstr ("yhiorigin", word, LWORD)
	if (strlen (word) == 0)
	    yhorg = ZERODATA
	else {
	    yhorg = strdic (word, origin, LWORD, YORIGIN_LIST)
	    if (yhorg <= 0) {
		call eprintf ("improper value for parameter yhiorigin = '%s'\n")
		    call pargstr (word)
		return
	    }
	}

	call clgstr ("yloorigin", word, LWORD)
	if (strlen (word) == 0)
	    ylorg = ZERODATA
	else {
	    ylorg = strdic (word, origin, LWORD, YORIGIN_LIST)
	    if (ylorg <= 0) {
		call eprintf ("improper value for parameter yloorigin = '%s'\n")
		    call pargstr (word)
		return
	    }
	}

	yhoff = clgetr ("yhioffset")
	yloff = clgetr ("ylooffset")

	if (debug) {
	    call eprintf ("high y limit:  origin = %d  offset = %f\n")
		call pargi (yhorg)
		call pargr (yhoff)
	    call eprintf (" low y limit:  origin = %d  offset = %f\n")
		call pargi (ylorg)
		call pargr (yloff)
	}

	subsample = clgeti ("subsample")
	if (debug) {
	    call eprintf ("subsample input values by %d\n")
		call pargi (subsample)
	}

	append = clgetb ("append")
	if (debug) {
	    call eprintf ("append = %b\n")
		call pargb (append)
	}

	timeunit = clgwrd ("timeunit", tustr, LWORD, TUNIT_LIST)
	timezpt = clgetl ("timezeropoint")
	if (debug) {
	    call eprintf ("time units = %d:  '%s'    time zero point = %d\n")
		call pargi (timeunit)
		call pargstr (tustr)
		call pargl (timezpt)
	}

	if (!IS_INDEFL (timezpt) && timezpt != 0) {
	    if (timeunit == TUHOURS && (timezpt < 0 || timezpt > 2359)) {
		call eprintf ("timezeropoint = %d, must be in range [0..2359] with timeunit = 'hours'\n")
		    call pargl (timezpt)
		return
	    }
	    if (timeunit == TUDAYS && (timezpt < 0 || timezpt > 991231)) {
		call eprintf ("timezeropoint = %d, must be in range [0..991231] with timeunit = 'days'\n")
		    call pargl (timezpt)
		return
	    }
	}

	list = clpopnu ("searchdir")
	if (debug) {
	    n = clplen (list)
	    call eprintf ("search dir length = %d\n")
		call pargi (n)
	    do i = 1, n {
		j = clgfil (list, datadir, SZ_PATHNAME)
		call eprintf ("  %s\n")
		    call pargstr (datadir)
	    }
	}

	call clgstr ("datadir", datadir, SZ_PATHNAME)
	do i = 1, N_LINES {
	    call sprintf (tmp_file[1, i], SZ_PATHNAME, "%sog%1d")
		call pargstr (datadir)
		call pargi (i)
	}

	call clgstr ("device", device, SZ_LINE)
	if (debug) {
	    call eprintf ("device = '%s'\n")
		call pargstr (device)
	}

	# Get the line information.
	do i = 1, N_LINES {
	    call clgstr (info_line[1, i], word, LWORD)
	    if (strlen (word) > 0) {
		if (streq (word, "=imcur") || streq (word, "imcur")) {
		    call strcpy ("", cmd, LEN_CMD)
		    n = clgcur ("coord", wx, wy, wcs, key, cmd, LEN_CMD)
		    if (debug) {
			call eprintf ("line %d, imcur = %d %.2f %.2f %d %d '%s'\n")
			    call pargi (i)
			    call pargi (n)
			    call pargr (wx)
			    call pargr (wy)
			    call pargi (wcs)
			    call pargi (key)
			    call pargstr (cmd)
		    }
		    call txyimcur (wx, wy, wcs, key, index[i], word, LWORD)
		    info_type[i] = strdic (word, info, LWORD, INFO_LIST)
		    call strcpy (info, fname[1, i], LWORD)
		} else {
		    info_type[i] = strdic (word, info, LWORD, INFO_LIST)
		    if (info_type[i] <= 0) {
			if (word[1] == 'r') {
			    info_type[i] = ROTV
			    call strcpy (ROTV_TEXT, fname[1, i], LWORD)
			} else {
			    info_type[i] = ADCV
			    call strcpy (ADCV_TEXT, fname[1, i], LWORD)
			}
			call strcpy (word, info, LWORD)
#		    } else if (info_type[i] == XTMP) {
#			call strcpy (DATABASE_FILE, fname[1, i], LWORD)
		    } else
			call strcpy (info, fname[1, i], LWORD)
		    index[i] = get_index (info_type[i], i, info)
		}
	        if (index[i] <= 0) {
		    call eprintf ("Bad index for line %d\n")
			call pargi (i)
		    call clpcls (list)
		    return
		}
		factor[i] = clgetr (factor_line[1, i])
		if (debug) {
		    call eprintf ("line %d: type = %d, index = %d in file")
			call pargi (i)
			call pargi (info_type[i])
			call pargi (index[i])
		    if (info_type[i] == XTMP) {
		        call eprintf (" '%s',")
			    call pargstr (fname[1, i])
		    } else {
		        call eprintf ("(s) '%s%06d' to '%s%06d',")
			    call pargstr (fname[1, i])
			    call pargl (datel)
			    call pargstr (fname[1, i])
			    call pargl (dater)
		    }
		    call eprintf (" factor = %f\n")
			call pargr (factor[i])
		}
	    } else {
		# Empty string signifies no line to plot.
		info_type[i] = NONE
		index[i] = 0
		if (debug) {
		    call eprintf ("line %d: type = %d, index = %d\n")
			call pargi (i)
			call pargi (info_type[i])
			call pargi (index[i])
		}
	    }
	}

	if (get_scale (info_type, index, fname, list, ndays, datel,
			timel, timer, timeunit, yhorg, yhoff, ylorg, yloff,
			timenmin, timenmax, ymin, ymax) == YES) {
	    call make_cmd (cmd, LEN_CMD, datel, info_type, index, timeunit,
			    timezpt,timenmin, timenmax, ymin, ymax, append)
	    if (debug) {
	        call eprintf ("cmd: %s\n")
		    call pargstr (cmd)
	        call eprintf ("  length = %d, maximum = %d\n")
		    call pargi (strlen (cmd))
		    call pargi (LEN_CMD)
	    }
	    if (strlen (cmd) >= LEN_CMD) {
		call eprintf ("ograph command length is too large, >= %d\n")
		    call pargi (LEN_CMD)
	    } else {
		call clcmdw (cmd)
	    }
	}
	call clpcls (list)

	if (!debug) {
	    do i = 1, N_LINES {
		if (access (tmp_file[1, i], 0, 0) == YES)
		    call delete (tmp_file[1, i])
	    }
	}
end
