# TRAN.H - Header file for task xytran

define  FORMAT     "%s%18txy= %6.2f %6.2f   rtz= %3d %3d %3d\n"
define	LINE_SIZE		80
define	LID_TYPE		'l'
define	BASE_TYPE		'b'
define	MOLD_TYPE		'm'
define	WALL_TYPE		'w'
define	ALUM_TYPE		'a'
define	REQ_TTMP		't'
define	REQ_HPWR		'h'
define	LIDTMP			11
define	BASETMP			21
define	MOLDTMP			31
define	WALLTMP			41
define	ALUMTMP			51
define	LIDHPWR			12
define	BASEHPWR		22
define	MOLDHPWR		32
define	WALLHPWR		42
define	ALUMHPWR		52
define	TLID			((TLMIN + TLMAX)/2.)
define	TBASE			((TBMIN + TBMAX)/2.)
define	TMOLD			((TMMIN + TMMAX)/2.)
define	TALUM			((TAMIN + TAMAX)/2.)
define	HLID			((HLMIN + HLMAX)/2.)
define	HBASE			((HBMIN + HBMAX)/2.)
define	HALUM			((HAMIN + HAMAX)/2.)

define	DIAMLBM			(TWMAX * 2.)
define	LBMSIZEX		real (XSZBASE)
define	LBMMIDX			((LBMSIZEX + 1.) / 2.)
define	LBMSIZEY		real (YSZBASE)
define	LBMMIDY			((LBMSIZEY + 1.) / 2.)
define	WALLSIZET		CIRCLE
define	CENTERWALLT		(WALLSIZET / 2.)
define	WALLSIZEX		real (XSZWALL)
define	WALLMIDX		((WALLSIZEX + 1.) / 2.)
define  WZMAX			max (HLMIN, TLMIN)
define  WZMIN			min (HBMAX, TBMAX)
define	WALLSIZEZ		real (WZMAX - WZMIN)
define	CENTERWALLZ		(real (WZMAX + WZMIN) / 2.)
define	WALLSIZEY		real (YSZWALL)
define	WALLMIDY		((WALLSIZEY + 1.) / 2.)
define	RADWALL			((TWMIN + TWMAX) / 2.)

define	MAXLOC			DLEN
define	HOFFSET			(DLEN + 1)
define	TOFFSET			1
define	L_ARRAY			2 * DLEN
define	TMAXLID			int (TLMAX)
define	TMINLID			int (TLMIN)
define	TMAXBASE		int (TBMAX)
define	TMINBASE		int (TBMIN)
define	TMAXMOLD		int (TMMAX)
define	TMINMOLD		int (TMMIN)
define	TMAXWALL		int (TWMAX)
define	TMINWALL		int (TWMIN)
define	TMAXALUM		int (TAMAX)
define	TMINALUM		int (TAMIN)
define	HMAXLID			int (HLMAX)
define	HMINLID			int (HLMIN)
define	HMAXBASE		int (HBMAX)
define	HMINBASE		int (HBMIN)
define	HMAXWALL		int (HWMAX)
define	HMINWALL		int (HWMIN)
define	HMAXALUM		int (HAMAX)
define	HMINALUM		int (HAMIN)
