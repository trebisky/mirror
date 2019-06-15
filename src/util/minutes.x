include <time.h>


# MINUTES.X - Routines to handle time in minutes.


# DATE_TO_M - Convert a date (yymmdd) to minutes since 00:00 01 Jan 1980.
#
# The date is in the format 'yymmdd' for year, month, and day of the month.
# The date value is the long integer which corresponds to this format.

# Corrected for Y2K by J. M. Hill 10FEB00

define	MIN_PER_DAY	1440
define	FEBRUARY	2
define	BEGIN_YEAR	long (1980)

long procedure date_to_m (date)

long	date		# date in format yymmdd (year, month, day)

int	i
long	y, m, min, n
int	days_per_month[12]
data	days_per_month /31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31/

begin
	y = date / 10000
	n = date - y * 10000
	m = n / 100
	min = n - m * 100

	y = y + 1900

# Correction for Y2K start
	if ( y < BEGIN_YEAR )
	    y = y + 100
# Correction for Y2K end

	if (mod (y, 4) == 0)
	    if (mod (y, 100) == 0)
		if (mod (y, 400) == 0)
		    days_per_month[FEBRUARY] = 29
		else
		    days_per_month[FEBRUARY] = 28
	    else
		days_per_month[FEBRUARY] = 29
	else
	    days_per_month[FEBRUARY] = 28

	n = 0
	if (m >= 1 && m <= 12)
	    do i = 1, m-1
		n = n + days_per_month[i]

	min = min + n

	n = (y - (BEGIN_YEAR - 3)) / 4		# number of leap years before
						#   this year

	min = min + 365 * (y - BEGIN_YEAR) + n
	min = (min - 1) * MIN_PER_DAY		# -1 is to start on 01 Jan
	return (min)
end


# MINUTES_TO_DT - Convert minutes since 00:00 01 Jan 1980 to a date and a time.
#
# The date returned is in the format 'yymmdd' for year, month, and day of month.
# The time returned is in the format 'hhmm' for hours and minutes.

define SEC_PER_MIN	60

procedure minutes_to_dt (minutes, date, time)

long	minutes			# minutes since 00:00 01 Jan 1980
long	date			# returned date in format yymmdd
long	time			# returned time in format hhmm

int	tm[LEN_TMSTRUCT]
long	sec

begin
	sec = minutes * SEC_PER_MIN
	call brktime (sec, tm)
	date = (TM_YEAR(tm) - 1900) * 10000 + TM_MONTH(tm) * 100 + TM_MDAY(tm)

# Correction for Y2K start
	if ( date > 999999 )
		date = date - 1000000
# Correction for Y2K end

	time = TM_HOUR(tm) * 100 + TM_MIN(tm)
end

# TIME_TO_M - Convert a signed formatted time to minutes.
#
# The time is a signed quantity in the format 'dhhmm' for days, hours, and
# minutes.  The time value is the long integer which corresponds to this
# format.

define	MIN_PER_DAY	1440
define	MIN_PER_HOUR	60

long procedure time_to_m (time)

long	time		# time in format dhhmm (days, hours, minutes)

long	atime, d, h, m, min

begin
	if (time < 0)
	    atime = -time
	else
	    atime = time

	d = atime / 10000
	atime = atime - d * 10000
	h = atime / 100
	m = atime - h * 100
	min = d * MIN_PER_DAY + h * MIN_PER_HOUR + m

	if (time < 0)
	    min = -min

	return (min)
end
