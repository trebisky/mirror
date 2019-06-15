# TC_HE.H - header file for thermocouple and heater locations.

define DLEN	720			# maximum number of any type of
					#    thermocouple or heater data

define IS_BASE	($1 >= bmin && $1 <= bmax)
define IS_LID	($1 >= lmin && $1 <= lmax)
define IS_WALL 	($1 >= wmin && $1 <= wmax && $2 > bmax && $2 < lmin)
define IS_MOLD  ($1 >= mmin && $1 <= mmax)
define IS_ALUM  ($1 >= umin && $1 <= umax)
