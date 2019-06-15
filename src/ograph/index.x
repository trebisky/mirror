include "ograph.h"

# INDEX.X - routines for finding information in tables; the index into the table
#		is returned.


# GET_INDEX - find the index of any kind of information; return 0 if error.

int procedure get_index (type, line, info)

int	type		# type of data to find index
int	line		# which line 1 .. N_LINES
char	info[ARB]	# information specification (for GPAN)

char	zone[LWORD]
int	radius, theta, z
int	addr, jd, jt, jn, jx
int	index

int	clgeti(), clgwrd(), list_index(), space_index()

include "ograph.com"
include "debugg.com"

begin
	switch (type) {
	case TTMP, HPWR, HTMP:
	    radius = clgeti (radius_line[1, line])
	    theta = clgeti (theta_line[1, line])
	    z = clgeti (z_line[1, line])
	    if (debug) {
		call eprintf ("  r = %d, th = %d, z = %d\n")
		    call pargi (radius)
		    call pargi (theta)
		    call pargi (z)
	    }
	    if (type == TTMP)
		index = space_index (tc_len, tc_index, tc_radius, tc_theta,
			tc_z, radius, theta, z)
	    else
		index = space_index (he_len, he_index, he_radius, he_theta,
			he_z, radius, theta, z)
	case ETMP, ZTMP, ZSCT, XTMP:
	    index = clgwrd (zone_line[1, line], zone, LWORD, ZONE_LIST)
# ????	    if (index > 13)
# ????		index = index - 2
	    if (index <= 0) {
		call eprintf ("bad zone specification: '%s'\n")
		    call pargstr (zone)
	    }
	case JTMP:
	    addr = clgeti (addr_line[1, line])
	    jd = addr/1000
	    if (jd >= NTD) {
		call eprintf ("bad addr specification: '%d'\n")
		    call pargi (addr)
		index = 0
		return (index)
	    }
	    addr = addr - jd*1000
	    jn = addr/100
	    if (jn >= NTN) {
		call eprintf ("bad addr specification: '%d'\n")
		    call pargi (addr)
		index = 0
		return (index)
	    }
	    addr = addr - jn*100
	    jt = addr/10
	    if (jt >= NTT) {
		call eprintf ("bad addr specification: '%d'\n")
		    call pargi (addr)
		index = 0
		return (index)
	    }
	    addr = addr - jt*10
	    jx = addr/1
	    if (jx >= NTX) {
		call eprintf ("bad addr specification: '%d'\n")
		    call pargi (addr)
		index = 0
		return (index)
	    }
	    addr = addr - jx*1
	    index = (jd*NTN+jn)*NTT+jt+1
	case ADCV, ROTV:
	    index = list_index (info)
	default:
	    index = 0
	}

	return (index)
end


# LIST_INDEX - find the index of a word in the gpan list.

int procedure list_index (word)

char	word[ARB]		# word to find in the list

int	i

bool	streq()

include "ograph.com"
include "debugg.com"

begin
	if (debug) {
	    call eprintf ("  word = '%s'\n")
		call pargstr (word)
	}

	do i = 1, gp_len {
	    if (streq (word, gp_name[1, i])) {
		return (gp_index[i])
	    }
	}
	
	call eprintf ("GPAN specification unknown: '%s'\n")
	    call pargstr (word)
	return (0)
end


# SPACE_INDEX - find the index of a location in space (nearest).

int procedure space_index (len, ai, ar, at, az, radius, theta, z)

int	len			# length of arrays
int	ai[len], ar[len]	# arrays of index, radius, theta, and z
int	at[len], az[len]
int	radius, theta, z	# radius, theta, and z to find in the arrays

int	i, index
long	d, mind

include "debugg.com"

begin
	if (IS_INDEFI (radius) || IS_INDEFI (theta) || IS_INDEFI (z)) {
	    call eprintf ("bad values for radius = %d, theta = %d, and/or z = %d\n")
		call pargi (radius)
		call pargi (theta)
		call pargi (z)
	    return (0)
	}

	index = 1
	d = abs (at[1] - theta)
	d = min (d, 360 - d)
	mind = d**2 + (ar[1] - radius)**2 + (az[1] - z)**2
	if (debug) {
	    call eprintf ("  d**2 = %d at index %d\n")
		call pargl (mind)
		call pargi (ai[index])
	}

	do i = 2, len {
	    d = abs (at[i] - theta)
	    d = min (d, 360 - d)
	    d = d**2 + (ar[i] - radius)**2 + (az[i] - z)**2
	    if (d < mind) {
		mind = d
		index = i
		if (debug) {
		    call eprintf ("  d**2 = %d at index %d\n")
			call pargl (mind)
			call pargi (ai[index])
		}
		if (mind == 0)
		    break
	    }
	}

	return (ai[index])
end
