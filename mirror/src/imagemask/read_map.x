include <math.h>
include "../odisp/tc_he.h"

# READ_MAP.X - read the map information.


procedure read_map ()

int	n

real	th

include "map.com"
include "../odisp/debugd.com"

begin
	call he_map (he_id, r1, r2, th1, th2, z1, z2, maplen)

	do n = 1, maplen {
	    r1[n] = r1[n]*r1[n]
	    r2[n] = r2[n]*r2[n]
	    if ((th2[n] - th1[n]) > 180.0) {
		th     = th2[n]
		th2[n] = th1[n]
		th1[n] = th
	    }
	    th1[n] = DEGTORAD (th1[n])
	    th2[n] = DEGTORAD (th2[n])
	}
end
