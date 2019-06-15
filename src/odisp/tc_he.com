# TC_HE.COM - common area for thermocouple and heater locations.

int	num[NASPECT,NINFO]	  # number of values for each aspect and information
int	loc[DLEN,NASPECT,NINFO]	  # locations in input data array
real	xpos[DLEN,NASPECT,NINFO]  # projected x and y positions for each
real	ypos[DLEN,NASPECT,NINFO]  #    aspect and information

common /loccom/ num, loc, xpos, ypos
