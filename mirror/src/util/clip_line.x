# CLIP_LINE.X - Clip a line to a window; return YES if line is visible, NO if not.
#
# The Cohen-Sutherland clipping algorithm is used.  The window limits may be in
# increasing or decreasing order.

define	BIT1	8	# set if point is above window
define	BIT2	4	# set if point is below window
define	BIT3	2	# set if point is to right of window
define	BIT4	1	# set if point is to left of window

int procedure clip_line (ix1, iy1, ix2, iy2, wxl, wxr, wyb, wyt, ox1, oy1, ox2, oy2)

real	ix1, iy1, ix2, iy2	# input coordinates of line endpoints
real	wxl, wxr, wyb, wyt	# window limits:  left, right, bottom, top
real	ox1, oy1, ox2, oy2	# output coordinates of clipped line endpoints

int	code[2]			# tells if point 1 or 2 is outside window
int	i
real	nwxl, nwxr, nwyb, nwyt	# window limits in proper order:  l < r,  b > t
real	xp, yp
real	x[2], y[2]

begin
	if (wxl == wxr || wyb == wyt)
	    return (NO)

	if (wxl < wxr) {
	    nwxl = wxl
	    nwxr = wxr
	} else {
	    nwxl = wxr
	    nwxr = wxl
	}

	if (wyb < wyt) {
	    nwyb = wyb
	    nwyt = wyt
	} else {
	    nwyb = wyt
	    nwyt = wyb
	}

	x[1] = ix1
	y[1] = iy1
	x[2] = ix2
	y[2] = iy2

	do i = 1, 2 {
	    code[i] = 0
	    if (x[i] < nwxl)
		code[i] = code[i] + BIT4
	    else if (x[i] > nwxr)
		code[i] = code[i] + BIT3
	    if (y[i] < nwyb)
		code[i] = code[i] + BIT2
	    else if (y[i] > nwyt)
		code[i] = code[i] + BIT1
	}

	while (or (code[1], code[2]) != 0) {
	    # Cannot trivial accept
	    if (and (code[1], code[2]) != 0) {
		# Trivial reject:  both points are outside window same direction
		return (NO)
	    }

	    switch (xor (code[1], code[2])) {
	    case 1:
		# BIT4 is set, line crosses left edge
		xp = nwxl
		yp = y[1] + (xp - x[1]) * (y[2] - y[1]) / (x[2] - x[1])
		if (and (code[1], BIT4) != 0)
		    i = 1
		else
		    i = 2
	    case 2, 3:
		# BIT3 is set, line crosses right edge
		xp = nwxr
		yp = y[1] + (xp - x[1]) * (y[2] - y[1]) / (x[2] - x[1])
		if (and (code[1], BIT3) != 0)
		    i = 1
		else
		    i = 2
	    case 4, 5, 6, 7:
		# BIT2 is set, line crosses bottom edge
		yp = nwyb
		xp = x[1] + (yp - y[1]) * (x[2] - x[1]) / (y[2] - y[1])
		if (and (code[1], BIT2) != 0)
		    i = 1
		else
		    i = 2
	    case 8, 9, 10, 11, 12, 13, 14, 15:
		# BIT1 is set, line crosses top edge
		yp = nwyt
		xp = x[1] + (yp - y[1]) * (x[2] - x[1]) / (y[2] - y[1])
		if (and (code[1], BIT1) != 0)
		    i = 1
		else
		    i = 2
	    default:
		call eprintf ("clip_line(): cannot happen\n")
		return (NO)
	    }
	    x[i] = xp
	    y[i] = yp
	    code[i] = 0
	    if (x[i] < nwxl)
		code[i] = code[i] + BIT4
	    else if (x[i] > nwxr)
		code[i] = code[i] + BIT3
	    if (y[i] < nwyb)
		code[i] = code[i] + BIT2
	    else if (y[i] > nwyt)
		code[i] = code[i] + BIT1
	}

	# Trivial accept:  both points are inside window
	ox1 = x[1]
	oy1 = y[1]
	ox2 = x[2]
	oy2 = y[2]

	return (YES)
end
