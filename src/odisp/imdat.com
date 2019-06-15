# IMDAT.COM - common area for image descriptions.

int 	imszx[NASPECT], imszy[NASPECT]	# size of images in pixels
int 	imsmx[NASPECT], imsmy[NASPECT]	# size of images in pixels for
					#   smoothing routine
int	imoffx[NASPECT], imoffy[NASPECT] # offset of image in smoothed array
real	imxmn[NASPECT], imxmx[NASPECT]	# minimum and maximum x and y positions
real	imymn[NASPECT], imymx[NASPECT]	#   for smoothed image
char	label[LWORD*(NINFO+NASPECT)]	# text labels

bool	silent				# don't report image min/max values

common /imdat/ imszx, imszy, imsmx, imsmy, imoffx, imoffy, imxmn, imxmx,
	       imymn, imymx, silent, label
