include <imhdr.h>
include <time.h>
define	NX		1440		# 24*60

# LGDATA -- Log data to iraf "images"

procedure lgdata (prefix, pix, ny)
char	prefix
real	pix[ny]
int	ny

int	x, y
long	clktime()
int	tm[LEN_TMSTRUCT]
int	imaccess()
pointer	sp
pointer	name
pointer	im
pointer	immap()
pointer	impl2r(), imps2r()

int	transpose
data	transpose/YES/

errchk	immap, imunmap
errchk	impl2r, imps2r

begin
	call smark (sp)
	call salloc (name, SZ_FNAME, TY_CHAR)

	call brktime (clktime (0), tm)
	call sprintf (Memc[name], SZ_FNAME, "%s%02d%02d%02d")
	    call pargstr (prefix)
	    call pargi (mod (TM_YEAR(tm), 100))
	    call pargi (TM_MONTH(tm))
	    call pargi (TM_MDAY(tm))
	x = TM_HOUR(tm)*60 + TM_MIN(tm)+1

	if (imaccess (Memc[name], READ_WRITE) == NO) {
	    im = immap (Memc[name], NEW_IMAGE, 0)
	    IM_PIXTYPE(im) = TY_REAL
	    IM_NDIM(im) = 2
	    if (transpose == YES) {
		IM_LEN(im, 1) = ny
		IM_LEN(im, 2) = NX
	    } else {
		IM_LEN(im, 1) = NX
		IM_LEN(im, 2) = ny
	    }
	    call strcpy (Memc[name], IM_TITLE(im), SZ_IMTITLE)
	    for (y = 1; y <= IM_LEN(im, 2); Y = y+1)
		call amovkr (INDEFR, Memr[impl2r (im, y)], IM_LEN(im, 1))
	    call imaddi (im, "FIRSTCOL", x)
	} else {
	    im = immap (Memc[name], READ_WRITE, 0)
	}

	call imaddi (im, "LASTCOL", x)
	if (IM_LEN(im, 2) == ny)
	    call amovr (pix, Memr[imps2r (im, x, x, 1, ny)], ny)
	else
	    call amovr (pix, Memr[imps2r (im, 1, ny, x, x)], ny)
	call imunmap (im)
	call sfree (sp)
end
