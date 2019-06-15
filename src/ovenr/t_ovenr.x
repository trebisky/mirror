include	<mach.h>

define	LEN_BUF	2
define	SZB_BUF	( LEN_BUF*SZ_LONG*SZB_CHAR )

# T_OVENR - Read the rotation speed data from the oven and print on STDOUT

procedure t_ovenr ()
pointer	sp
pointer	hp	# host,port
int	lower, upper
int	clgeti()
int	i
int	status
int	chan
long	buf[LEN_BUF]
begin
	call smark (sp)
	call salloc (hp, SZ_FNAME, TY_CHAR)
	call clgstr ("connection", Memc[hp], SZ_FNAME)
	lower = clgeti ("lower")
	upper = clgeti ("upper")
	call strpak (Memc[hp], Memc[hp], SZ_FNAME)
	call zopnso (Memc[hp], READ_WRITE, chan)
	if (chan == ERR) {
	    call eprintf ("could not open connection\n")
	    call sfree (sp)
	    return
	}
	repeat {
	    call zardso (chan, buf, SZB_BUF, 0)
	    call zawtso (chan, status)
	    if (status != SZB_BUF)
		break
	    if (BYTE_SWAP4 == YES)
		call bswap4 (buf, 0, buf, 0, SZB_BUF)
	    do i=1,LEN_BUF {
		call printf ("%d ")
		    call pargl (buf[i])
	    }
	    call printf ("\n")
	    call flush (STDOUT)
	    if (buf[2] < lower || buf[2] > upper) {
		call eprintf ("\007")
		call flush (STDERR)
	    }
	}
	call zclsso (chan, status)
	call sfree (sp)
end
