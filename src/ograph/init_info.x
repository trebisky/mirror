include <ctype.h>
include <error.h>
include "ograph.h"

# INIT_INFO.X - initialize data arrays with information.

procedure init_info()

int	n, index, i
long	filetime

int	status
int	oveno()
long	clktime()
long	ptimestamp()

long	timestamp

data	timestamp /-1/

include "ograph.com"
include "debugg.com"

errchk	init_loc

begin
	status = oveno (0)
	if (status != 0) {
	    call eprintf ("shared memory parameter database initialization error %d\n")
		call pargi (status)
	    call erract (EA_FATAL)
	}

	if (timestamp == ptimestamp(0))
	    return;
	timestamp = ptimestamp(0);

	filetime = clktime (0)

	call tc_loc (tc_index, tc_radius, tc_theta, tc_z, L_TTMP)
	tc_len = L_TTMP
	call he_loc (he_index, he_radius, he_theta, he_z, L_HPWR)
	he_len = L_HPWR
	call gpan_loc (gp_index, gp_name, L_GPAN, LWORD)
	gp_len = L_GPAN
	call datab_xtmp (db_n, db_time, db_val, N_XTMP, L_XTMP);

	do index = 1, N_XTMP {
	    do i = 1, db_n[index] {
		db_time[i, index] = (db_time[i, index] + filetime + 30) / 60
	    }
	}
    
	do n = 1, N_LINES {
	    call sprintf (info_line[1, n], LWORD, "%s%d")
		call pargstr (INFO_TEXT)
		call pargi (n)
	    call sprintf (radius_line[1, n], LWORD, "%s%d")
		call pargstr (RADIUS_TEXT)
		call pargi (n)
	    call sprintf (theta_line[1, n], LWORD, "%s%d")
		call pargstr (THETA_TEXT)
		call pargi (n)
	    call sprintf (z_line[1, n], LWORD, "%s%d")
		call pargstr (Z_TEXT)
		call pargi (n)
	    call sprintf (zone_line[1, n], LWORD, "%s%d")
		call pargstr (ZONE_TEXT)
		call pargi (n)
	    call sprintf (addr_line[1, n], LWORD, "%s%d")
		call pargstr (ADDR_TEXT)
		call pargi (n)
	    call sprintf (factor_line[1, n], LWORD, "%s%d")
		call pargstr (FACTOR_TEXT)
		call pargi (n)
	}

	# initialize database for odisp also

	call init_loc
	call read_map
	call init_masks
	call init_xytran
end

# GTC_LOC.X - get tc loc info for odisp

bool	procedure gtc_loc (n, index, radius, theta, z)

int	n
int	index
real	radius
real	theta
real	z

include "ograph.com"
include "debugg.com"

begin
	if (n > tc_len)
	    return (false)

	index = tc_index[n]
	radius = tc_radius[n]
	theta = tc_theta[n]
	z = tc_z[n]
	return (true)
end

# GHE_LOC.X - get he loc info for odisp

bool	procedure ghe_loc (n, index, radius, theta, z)

int	n
int	index
real	radius
real	theta
real	z

include "ograph.com"
include "debugg.com"

begin
	if (n > he_len)
	    return (false)

	index = he_index[n]
	radius = he_radius[n]
	theta = he_theta[n]
	z = he_z[n]
	return (true)
end
