# ODISP.H - header file for task odisp

# various constant values

define LWORD		10

define FIRSTCOL_KW	"FIRSTCOL"	# keywords for first column
define LASTCOL_KW	"LASTCOL"	#    and last column
define INMIN_KW		"INPUTMIN"	# keywords for minimum and maximum
define INMAX_KW		"INPUTMAX"	#    input data values

define NINFO		3		# number of kinds of information:
					#    heater power, heater temperatures,
					#    thermocouple temperatures
define NASPECT		5		# number of aspects:
					#    base, lid, wall, mold
define INFOLIST		"|hpwr|htmp|ttmp|"
define ASPECTLIST	"|base|lid|wall|mold|alum|"

define NIMAGE	       30            	# this must be equal to (2 * NINFO * NASPECT)
					# and must be a numeric constant because
					#    it is used in a DATA statement

define HVAL		1		# values to specify 1 of NIMAGE images:
define HTVAL            2		#   (p,h,t)(b,l,w,m)(now,then)
define TVAL		3
define BVAL		1
define LVAL		2
define WVAL		3
define MVAL             4
define AVAL             5

# wall information

define WALL_OVERLAP  90.	# amount of overlap on each end of wall (degrees)
define CIRCLE       360.

# image information

define WALL_HT_MAG     1	# extra magnification for wall height
define XSZBASE	      64
define YSZBASE	      64
define XSZLID	      64
define YSZLID	      64
define XSZWALL	     128
define YSZWALL	      32	# this must be equal to (32 / WALL_HT_MAG)
				# and must be a numeric constant because
				#   it is used in a DATA statement
define XSZMOLD	      64
define YSZMOLD	      64
define XSZALUM	      64
define YSZALUM	      64

define Xsp	Memd[imsp]
define Ysp	Memd[imsp + DLEN]
define Zsp	Memd[imsp + 2 * DLEN]
define Isp	Memd[imsp + 3 * DLEN]

define MIN_INTENSITY_DIFF	5.	# minimum intensity difference for auto
					#    intensity scaling
define XSZWINDOW	      512	# window size in pixels
define YSZWINDOW	      512

# smoothing routine limits

define MINSMOOTH         4		# minimum number of points to smooth
define MAXSMAXISLEN	1024		# minimum axis length
define MAXSMAREA 	(256*256)	# maximum image area

# labels

define HPWR		"hpwr"
define HTMP		"htmp"
define TTMP		"ttmp"
define BASE		"base"
define LID		"lid"
define WALL		"wall"
define MOLD		"mold"
define ALUM		"alum"

define INFOLABEL	label[($1-1)*LWORD + 1]
define ASPECTLABEL	label[NINFO*LWORD + ($1-1)*LWORD + 1]
