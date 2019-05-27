# OGRAPH.H - header file for task ograph.

define INFO_TEXT	"info"		# parameter name
define RADIUS_TEXT	"r"		# parameter name
define THETA_TEXT	"th"		# parameter name
define Z_TEXT		"z"		# parameter name
define ZONE_TEXT	"zone"		# parameter name
define ADDR_TEXT	"addr"		# parameter name
define FACTOR_TEXT      "factor"        # parameter name

define INFO_LIST	"|ttmp|hpwr|htmp|etmp|ztmp|zsct|xtmp|jtmp|"
define ZONE_LIST	"|z0|z1|z2|z3|z4|z5|z6|z7|z8|z9|z10|z11|z12|z13|z14|z15|z16|z17|z18|z19|r|"
define YORIGIN_LIST     "|average|mindata|maxdata|zero|"
define TORIGIN_LIST	"|now|"
define TUNIT_LIST	"|hours|days|"
define ADCV_TEXT	"adcv"
define ROTV_TEXT	"rotv"

define R_INDEX		21

define TTMP		1		# time averaged temperatures
define HPWR		2		# heater powers
define HTMP		3		# derived heater temperatures
define ETMP		4		# scheduled temperatures
define ZTMP		5		# average zone temperatures
define ZSCT		6		# scatter of zone temperatures
define XTMP		7		# expected scheduled temperatures
define JTMP		8		# junction temperatures
define ADCV		9
define ROTV		10

define N_DATA		10		# equal to largest of above info types
define NONE	     (N_DATA+1)		# one more than last value

define AVERAGE		1		# y origin values
define MINDATA		2
define MAXDATA		3
define ZERODATA		4

define TIMENOW		1		# time origin values
define TIMEDATE		2

define TUHOURS		1		# time unit values
define TUDAYS		2

define L_TTMP		720		# length of TTMP arrays
define L_HPWR		300		# length of HPWR arrays
define L_GPAN		176		# length of GPAN arrays (165?)
define L_ZTMP		 21		# length of ZTMP arrays
define L_ZSCT		 21		# length of ZSCT arrays
define L_ETMP		 21		# length of ETMP arrays
define L_XTMP		 32		# length of XTMP arrays
define N_XTMP		 21		# number of XTMP arrays

define NHP		10		# number of panels (0 .. 9)
define NHF		 3		# number of phases (0 .. 2)
define NHE		10		# number of elements (0 .. 9)

define HE_INDEX	   ((TO_INTEG($1)*NHF+TO_INTEG($2))*NHE+TO_INTEG($3)+1)

define NTD		6		# number of dcus (0 .. 5)
define NTN		6		# number of cards (0 .. 5)
define NTT		4		# number of counters (0 .. 3)
define NTX		5		# number of multiplexors (0 .. 4)

define TC_INDEX	   (((TO_INTEG($1)*NTN+TO_INTEG($2))*NTT+TO_INTEG($3))*NTX+TO_INTEG($4)+1)

define LWORD		10		# length of word
define LEN_CMD	       512		# length of GRAPH command

define N_LINES		 4		# number of lines to graph

define FIRSTCOL_KW	"FIRSTCOL"	# keywords for first column
define LASTCOL_KW	"LASTCOL"	#    and last column

define LINE_LENGTH	1440		# length of line in data files

define TEMP_LABEL	"degrees C"	# Y-axis labels
define POWER_LABEL	"power units"
define VOLT_LABEL	"volts"
define AMP_LABEL	"amps"
define UNIT_LABEL	"units"
define ROTATION_LABEL	"rpm"
define PERCENT_LABEL	"percent"
