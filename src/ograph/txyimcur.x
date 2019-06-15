include "ograph.h"
include "../odisp/tc_he_bounds.h"

define	LID	1
define	WALL	2
define	BASE	3
define 	MOLD	4

# TXYIMCUR.X - translate the X,Y coordinates from the image cursor into RTZ
#		coordinates in the oven.


procedure txyimcur (xc, yc, wcs, key, index, info, l_info)

real	xc, yc		# the X,Y coordinates from the image cursor
int	wcs		# world coordinate system frame number
int	key		# key stroke
int	index		# index of information (returned)
char	info[l_info]	# information specification (returned)
int	l_info		# length of character array 'info'

int	asp, inf, ir, it, iz
real	r, t, x, y, z

int	space_index()

include	"ograph.com"
include "debugg.com"

begin
	# setup a default aspect based on the wcs frame number

	wcs = wcs / 100
	switch (wcs) {
	case 1:
	    asp = LID
	case 2:
	    asp = WALL
	case 3:
	    asp = BASE
	default:
	    asp = MOLD
	}

	# Based on the key stroke, find the info and aspect

	switch (key) {
	case 'l':
	    inf = TTMP
	    asp = LID
	case 'w':
	    inf = TTMP
	    asp = WALL
	case 'b':
	    inf = TTMP
	    asp = BASE
	case 'm':
	    inf = TTMP
	    asp = MOLD
	case 'L':
	    inf = HPWR
	    asp = LID
	case 'W':
	    inf = HPWR
	    asp = WALL
	case 'B':
	    inf = HPWR
	    asp = BASE
	case 'M':
	    inf = HPWR
	    asp = MOLD
	case '\014':
	    inf = HTMP
	    asp = LID
	case '\027':
	    inf = HTMP
	    asp = WALL
	case '\002':
	    inf = HTMP
	    asp = BASE
	case '\015':
	    inf = HTMP
	    asp = MOLD
	case 't':
	    inf = TTMP
	case 'h':
	    inf = HTMP
	case 'p':
	    inf = HPWR
	default:
	    call strcpy ("", info, l_info)
	    index = 0
	    return
	}

	# Calculate the RTZ from the above information

	if (asp == WALL)
	    call gr_tran_w (xc, yc, r, t, x, y, z)
	else {
	    call gr_tran_lbm (xc, yc, r, t, x, y)
	    if (asp == LID)
		if (inf == HPWR)
		    z = (HLMIN + HLMAX) / 2.
		else
		    z = (TLMIN + TLMAX) / 2.
	    else if (asp == BASE)
		if (inf == HPWR)
		    z = (HBMIN + HBMAX) / 2.
		else
		    z = (TBMIN + TBMAX) / 2.
	    else
		z = TMMIN + 1.
	}

	ir = r + 0.5
	it = t + 0.5
	iz = z + 0.5

	if (debug) {
	    call eprintf ("  r = %d, th = %d, z = %d\n")
		call pargi (ir)
		call pargi (it)
		call pargi (iz)
	}

	if (inf == TTMP) {
	    index = space_index (tc_len, tc_index, tc_radius, tc_theta,
		    tc_z, ir, it, iz)
	    call strcpy ("ttmp", info, l_info)
	} else {
	    index = space_index (he_len, he_index, he_radius, he_theta,
		    he_z, ir, it, iz)
	    if (inf == HPWR)
		call strcpy ("hpwr", info, l_info)
	    else
		call strcpy ("htmp", info, l_info)
	}
end

# TIMCUR.X - translate the image cursor into RTZ
#		coordinates in the oven.


int procedure timcur (info, l_info)

char	info[l_info]	# information specification (returned)
int	l_info		# length of character array 'info'

real	xc, yc		# the X,Y coordinates from the image cursor
int	wcs		# world coordinate system frame number
int	key		# key stroke
int	n
int	asp, inf, ir, it, iz
real	r, t, x, y, z
char	cmd[LEN_CMD]

bool	strne()
int	clgcur()

include "debugg.com"

begin
	if (strne (info, "=imcur") && strne (info, "imcur"))
	    return (NO)

	call strcpy ("", cmd, LEN_CMD)
	n = clgcur ("coord", xc, yc, wcs, key, cmd, LEN_CMD)

	# setup a default aspect based on the wcs frame number

	wcs = wcs / 100
	switch (wcs) {
	case 1:
	    asp = LID
	case 2:
	    asp = WALL
	case 3:
	    asp = BASE
	default:
	    asp = MOLD
	}

	# Based on the key stroke, find the info and aspect

	switch (key) {
	case 'l':
	    inf = TTMP
	    asp = LID
	case 'w':
	    inf = TTMP
	    asp = WALL
	case 'b':
	    inf = TTMP
	    asp = BASE
	case 'm':
	    inf = TTMP
	    asp = MOLD
	case 'L':
	    inf = HPWR
	    asp = LID
	case 'W':
	    inf = HPWR
	    asp = WALL
	case 'B':
	    inf = HPWR
	    asp = BASE
	case 'M':
	    inf = HPWR
	    asp = MOLD
	case '\014':
	    inf = HTMP
	    asp = LID
	case '\027':
	    inf = HTMP
	    asp = WALL
	case '\002':
	    inf = HTMP
	    asp = BASE
	case '\015':
	    inf = HTMP
	    asp = MOLD
	case 't':
	    inf = TTMP
	case 'h':
	    inf = HTMP
	case 'p':
	    inf = HPWR
	default:
	    call strcpy ("", info, l_info)
	    return (YES)
	}

	# Calculate the RTZ from the above information

	if (asp == WALL)
	    call gr_tran_w (xc, yc, r, t, x, y, z)
	else {
	    call gr_tran_lbm (xc, yc, r, t, x, y)
	    if (asp == LID)
		if (inf == HPWR)
		    z = (HLMIN + HLMAX) / 2.
		else
		    z = (TLMIN + TLMAX) / 2.
	    else if (asp == BASE)
		if (inf == HPWR)
		    z = (HBMIN + HBMAX) / 2.
		else
		    z = (TBMIN + TBMAX) / 2.
	    else
		z = TMMIN + 1.
	}

	ir = r + 0.5
	it = t + 0.5
	iz = z + 0.5

	call sprintf (info, l_info, "%d %d %d")
	    call pargi (ir)
	    call pargi (it)
	    call pargi (iz)

	return (YES)
end
