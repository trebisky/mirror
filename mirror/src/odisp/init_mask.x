include "odisp.h"

# INIT_MASK.X - initialize the mask for the image


procedure init_mask (aspect)

int	aspect		# aspect number of image

int	xsize, ysize

include "debugd.com"

begin
	switch (aspect) {
	case BVAL:
	    xsize = XSZBASE
	    ysize = YSZBASE
	case LVAL:
	    xsize = XSZLID
	    ysize = YSZLID
	case WVAL:
	    xsize = XSZWALL
	    ysize = YSZWALL
	case MVAL:
	    xsize = XSZMOLD
	    ysize = YSZMOLD
	case AVAL:
	    xsize = XSZALUM
	    ysize = YSZALUM
	}

	call make_mask (aspect, xsize, ysize)
end

# INIT_MASKS.X - initialize all the masks for the image


procedure init_masks ()

int	aspect		# aspect number of image

include "debugd.com"

begin
	do aspect=1,NASPECT
	    call init_mask (aspect)
end
