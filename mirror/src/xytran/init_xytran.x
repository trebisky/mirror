include "../odisp/odisp.h"
include "../odisp/tc_he.h"
include    "tran.h"

# INIT_XYTRAN.X - initialize common arrays for task XYTRAN.


procedure init_xytran ()

int    	i

include "tran.com"
include "debugx.com"

begin
	# Read in the thermocouple location data 

	i = TOFFSET
	call xtc_loc (name[1, i], LWORD, lr[i], lt[i], lz[i], lx[i], ly[i], last_therm)
	last_therm= last_therm + TOFFSET-1

	# Read in the heater location data 

	i = HOFFSET
	call xhe_loc (name[1, i], LWORD, lr[i], lt[i], lz[i], lx[i], ly[i], last_heater)
	last_heater= last_heater + HOFFSET-1
end
