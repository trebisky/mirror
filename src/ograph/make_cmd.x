include "ograph.h"

define	LEN_LAB		80
define	MIN_PER_HOUR	60.

# MAKE_CMD.X - make a command to graph several lines.

procedure make_cmd (cmd, len, date, type, index, timeunit, timezpt,
			tmin, tmax, ymin, ymax, append)

char	cmd[len]		# command to be formed
int	len			# maximum length of command
long	date			# date at beginning of information
int	type[N_LINES]		# type of information to graph
int	index[N_LINES]		# location of information in file
int	timeunit		# time units:  hours or days
long	timezpt			# time zero point:  INDEF, 0, hhmm, yymmdd
real	tmin, tmax		# x (time) limits
real	ymin, ymax		# y limits
bool	append			# append to existing plot?

char	work[SZ_LINE], xlabel[LEN_LAB]
int	i, firsttype, firstindex
long	date0
real	time0, time1

int	access()
long	time_to_m()
bool	streq()
int	strncmp()

include "ograph.com"
include "debugg.com"

begin
	firsttype = NONE
	do i = 1, N_LINES
	    if (type[i] != NONE) {
		firsttype = type[i]
		firstindex = index[i]
		break
	    }

	if (firsttype != NONE) {
	    if (timeunit == TUHOURS) {
		if (!IS_INDEFL (timezpt))
		    call strcpy ("xl=\"time (hours)\"", xlabel, LEN_LAB)
		else
		    call strcpy ("xl=\"time of day (hours)\"", xlabel, LEN_LAB)
	    } else {
		if (timezpt == 0)
		    call strcpy ("xl=\"time (days)\"", xlabel, LEN_LAB)
		else
		    call strcpy ("xl=\"day of month\"", xlabel, LEN_LAB)
	    }
	    if (debug) {
		call eprintf ("x-label = %s\n")
		    call pargstr (xlabel)
	    }

	    switch (firsttype) {
	    case ETMP, ZTMP, ZSCT, XTMP:
		if (firstindex == R_INDEX)
		    call strcpy (ROTATION_LABEL, work, SZ_LINE)
		else
		    call strcpy (TEMP_LABEL, work, SZ_LINE)
	    case TTMP, HTMP, JTMP:
		call strcpy (TEMP_LABEL, work, SZ_LINE)
	    case HPWR:
		call strcpy (POWER_LABEL, work, SZ_LINE)
	    case ADCV:
		call strcpy (UNIT_LABEL, work, SZ_LINE)
		do i = 1, gp_len
		    if (firstindex == gp_index[i]) {
		        if (gp_name[1, i] != 'r') {
			    if (gp_name[2, i] == 'c')
				call strcpy (AMP_LABEL, work, SZ_LINE)
			    else
				call strcpy (VOLT_LABEL, work, SZ_LINE)
			    break
			}
		    }
	    case ROTV:
		call strcpy (UNIT_LABEL, work, SZ_LINE)
		do i = 1, gp_len
		    if (firstindex == gp_index[i]) {
		        if (gp_name[1, i] == 'r') {
			    if (streq (gp_name[3, i], "amp"))
				call strcpy (AMP_LABEL, work, SZ_LINE)
			    else if (streq (gp_name[3, i], "err"))
				call strcpy (PERCENT_LABEL, work, SZ_LINE)
			    else if (streq (gp_name[3, i], "ref"))
				call strcpy (PERCENT_LABEL, work, SZ_LINE)
			    else if (streq (gp_name[3, i], "fdbk"))
				call strcpy (PERCENT_LABEL, work, SZ_LINE)
			    else if (streq (gp_name[3, i], "rpm"))
				call strcpy (ROTATION_LABEL, work, SZ_LINE)
			    else if (strncmp (gp_name[3, i], "lvdt", 4) == 0)
				call strcpy (TEMP_LABEL, work, SZ_LINE)
			    else
				call strcpy (VOLT_LABEL, work, SZ_LINE)
			    break
			}
		    }
	    default:
		call strcpy (UNIT_LABEL, work, SZ_LINE)
	    }
	    if (debug) {
	        call eprintf ("y-label = '%s'\n")
		    call pargstr (work)
	    }

	    time0 = tmin
	    time1 = tmax

	    if (!IS_INDEFL (timezpt)) {
		time0 = timezpt
		if (timezpt != 0) {
		    if (timeunit == TUHOURS)
			time0 = real (time_to_m (timezpt)) / MIN_PER_HOUR
		    else
			time0 = mod (timezpt, 100)
		}
		time1 = time0 + (tmax - tmin)
	    }

	    call sprintf (cmd, len,
		"graph ap=%b dev=\"%s\" wx1=%.6g wx2=%.6g wy1=%.6g wy2=%.6g %s yl=\"%s\"")
		call pargb (append)
		call pargstr (device)
		call pargr (time0)
		call pargr (time1)
		call pargr (ymin)
		call pargr (ymax)
	        call pargstr (xlabel)
		call pargstr (work)

	    date0 = date
	    if (!IS_INDEFL (timezpt)) {
		if (timezpt == 0)
		    date0 = 0
		else if (timeunit == TUHOURS)
		    date0 = 0
		else
		    date0 = timezpt
	    }
	    if (date0 == 0)
		call sprintf (work, SZ_LINE, " tit=\"Relative")
	    else {
		call sprintf (work, SZ_LINE, " tit=\"%06d")
		    call pargl (date0)
	    }

	    call strcat (work, cmd, len)

	    do i = 1, N_LINES
		call cat_title (cmd, len, work, type[i], index[i], factor[i])

	    call strcat ("\" in=", cmd, len)

	    do i = 1, N_LINES {
		if (access (tmp_file[1, i], READ_ONLY, TEXT_FILE) == YES)
		    call cat_input (cmd, len, type[i], tmp_file[1, i])
	    }

	    if (!IS_INDEFL (timezpt)) {
		call sprintf (work, SZ_LINE, " lin+ p1=%.6g p2=%.6g q1=%.6g q2=%.6g")
		    call pargr (tmin)
		    call pargr (tmax)
		    call pargr (time0)
		    call pargr (time1)
	    } else
		call sprintf (work, SZ_LINE, " lin-")
	    
	    call strcat (work, cmd, len)

	} else
	    call strcpy ("", cmd, len)
end


# CAT_INPUT - add input specifiers to command line.

procedure cat_input (cmd, len, type, fname)

char	cmd[len]		# command to concatenate to
int	len			# maximum length of command
int	type			# type of information
char	fname[SZ_PATHNAME]	# name of data file


int	strlen()

begin
	if (type == NONE)
	    return

	if (cmd[strlen (cmd)] != '=')
	    call strcat (",", cmd, len)

	call strcat (fname, cmd, len)
end


# CAT_TITLE - add title information to the command line.

procedure cat_title (cmd, len, work, type, index, factorl)

char	cmd[len]		# command to concatenate to
int	len			# maximum length of command
char 	work[SZ_LINE]		# work array
int	type			# type of information
int	index			# location of information in file
real	factorl			# factor for line

char	fact[1]
int	i, loc, r, th, z
int	addr, jd, jn, jt

string	initial	"tphezsxj90"

include "ograph.com"

begin
	if (factorl == 1.)
	    call strcpy ("", fact, 1)
	else
	    call strcpy ("*", fact, 1)

	switch (type) {
	case TTMP, HPWR, HTMP:
	    if (type == TTMP)
		do i = 1, tc_len {
		    if (index == tc_index[i]) {
			r = tc_radius[i]
			th = tc_theta[i]
			z = tc_z[i]
			break
		    }
		}
	    else
		do i = 1, he_len {
		    if (index == he_index[i]) {
			r = he_radius[i]
			th = he_theta[i]
			z = he_z[i]
			break
		    }
		}
	    call sprintf (work, SZ_LINE, ", %c%s %d %d %d")
		call pargc (initial[type])
		call pargstr (fact)
		call pargi (r)
		call pargi (th)
		call pargi (z)

	case JTMP:
	    addr = index - 1
	    jd = addr/(NTN*NTT)
	    addr = addr - jd*(NTN*NTT)
	    jn = addr/NTT
	    addr = addr - jn*NTT
	    jt = addr
	    call sprintf (work, SZ_LINE, ", %c%s %d%d%d0")
		call pargc (initial[type])
		call pargstr (fact)
		call pargi (jd)
		call pargi (jn)
		call pargi (jt)
	
	case ETMP, ZTMP, ZSCT, XTMP:
	    if (index != R_INDEX) {
		call sprintf (work, SZ_LINE, ", %c%s z%d")
		    call pargc (initial[type])
		    call pargstr (fact)
		    call pargi (index - 1)
	    } else {
		call sprintf (work, SZ_LINE, ", %c%s r")
		    call pargc (initial[type])
		    call pargstr (fact)
	    }

	case ADCV:
	    do i = 1, gp_len
		if (index == gp_index[i]) {
		    if (gp_name[1, i] != 'r') {
			loc = i
			break
		    }
		}
	    
	    call sprintf (work, SZ_LINE, ", %s%s")
		call pargstr (gp_name[1, loc])
		call pargstr (fact)

	case ROTV:
	    do i = 1, gp_len
		if (index == gp_index[i]) {
		    if (gp_name[1, i] == 'r') {
			loc = i
			break
		    }
		}
	    
	    call sprintf (work, SZ_LINE, ", %s%s")
		call pargstr (gp_name[1, loc])
		call pargstr (fact)

	default:
	    return
	}

	call strcat (work, cmd, len)
end
