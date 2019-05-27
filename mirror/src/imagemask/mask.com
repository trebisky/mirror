# MASK.COM - common area for imagemask task

pointer	maskp[NASPECT]		# pointer to mask
int	mxsize[NASPECT]		# mask xsize
int	mysize[NASPECT]		# mask ysize

common /maskcom/ maskp, mxsize, mysize
