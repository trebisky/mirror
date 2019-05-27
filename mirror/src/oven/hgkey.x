include <fset.h>
include <chars.h>

# HGKEY -- Hoven get ukey routine

define	SZ_KEYSTR	80

procedure hgkey (key, period, offset)
int	key
int	period
int	offset

#char	keystr[SZ_KEYSTR]
#int	clgkey()

int	ch, delay, getci()
long	clktime()
long	now
long	wakeuptime
int	notraw

begin
#	if (clgkey ("ukey", key, keystr, SZ_KEYSTR) == EOF)
#	    key = INTCHAR
	call flush (STDOUT)
	call flush (STDERR)
	key = 'q'
	notraw = -1
	call thgkey (notraw)
	if (notraw == YES) {
	    call thgkey (2)
	}
	now = clktime(0)
	wakeuptime = (period - mod (now, period) + offset) + now
	delay = 0
	repeat {
	    ch = getci (STDIN, ch)
	    switch (ch) {
	    case '\033', 'O', '[':
		next
	    case EOF:
#		call zwmsec (100)
		call decisleep
		delay = delay-1
		if (delay <= 0) {
		    delay = (wakeuptime - clktime(0))*10
		    if (delay <= 0)
			break
		    if (delay > 100)
			delay = 100
		}
	    default:
		key = ch
		break
	    }
	}
#	if (notfirst == NO) {
#	    call fseti (STDIN, F_IOMODE, IO_NORMAL)
#	}
end


procedure thgkey (sw)
int	sw

int	notraw
data	notraw/YES/

begin
	if (sw == -1) {
	    sw = notraw
	    return
	}

	call flush (STDOUT)
	call flush (STDERR)
	if ((sw & 1) != 0) {
	    call fseti (STDIN, F_IOMODE, IO_NORMAL)
	    notraw = YES
	}
	call flush (STDOUT)
	call flush (STDERR)
	if ((sw & 2) != 0) {
	    call fseti (STDIN, F_IOMODE, IO_RAW+IO_NDELAY)
	    notraw = NO
	}
	call flush (STDOUT)
	call flush (STDERR)
end


# HPAGEH - Hoven page cursor mode help file

procedure hpageh ()

begin
	call thgkey(1)
	call pagefiles ("oven$cm.hlp")
	call thgkey(2)
end

# HCLGST - Hoven CL get string

procedure hclgst (param, outstr, maxch)
char	param[ARB]
char	outstr[maxch]
int	maxch

errchk	clgstr

begin
	call clgstr (param, outstr, maxch)
end

# FLGKEY -- Flush before getting the next key.

procedure flgkey (key)

int	key

begin
	# Flush any buffered text output.
	call flush (STDERR)
	call flush (STDOUT)
end
