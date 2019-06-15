include <math.h>

define	ZERO	0.D0

# CKDATAN2.X - check the arguments of DATAN2.

double procedure ckdatan2 (y, x)

double	y, x

double	dval

begin
	if (x == ZERO) {
	    if (y == ZERO) 
		dval = ZERO
	    else if (y > ZERO)
		dval = HALFPI
	    else
		dval = -HALFPI
	} else
	    dval = datan2 (y, x)

	return (dval)
end
