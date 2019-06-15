# MAP.COM - common area for imagemask task

int	maplen			# actual number of data elements in arrays
int	he_id[DLEN]		# identity of heater
real	r1[DLEN], r2[DLEN]	# radius squared limits
real	th1[DLEN], th2[DLEN]	# theta limits in radians
real	z1[DLEN], z2[DLEN]	# z limits

common /mapcom/ maplen, he_id, r1, r2, th1, th2, z1, z2
