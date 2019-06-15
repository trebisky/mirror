#	t_hoven - get the parameters for the hoven task

procedure t_hoven ()
int	noven
int	ncomp
int	readonly
int	remove
int	period
int	offset
int	status
int	clgeti()
bool	clgetb()
int	btoi()
int	hoven()
errchk	hoven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")
	readonly = btoi (clgetb ("readonly"))
	remove = btoi (clgetb ("remove"))
	period = clgeti ("period")
	offset = clgeti ("offset")

	status = hoven (noven, ncomp, readonly, remove, period, offset)
	if (status != 0) {
	    call printf ("hoven error %d\n")
		call pargi (status)
	}
end

#	t_doven - get the parameters for the doven task

procedure t_doven ()
int	noven
int	ncomp
int	logdata
int	period
int	offset
int	status
int	clgeti()
bool	clgetb()
int	btoi()
int	doven()
errchk	doven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")
	logdata = btoi (clgetb ("logdata"))
	period = clgeti ("period")
	offset = clgeti ("offset")

	status = doven (noven, ncomp, logdata, period, offset)
	if (status != 0) {
	    call printf ("doven error %d\n")
		call pargi (status)
	}
end

#	t_eoven - get the parameters for the eoven task

procedure t_eoven ()
int	noven
int	ncomp
int	period
int	offset
int	status
int	clgeti()
int	eoven()
errchk	eoven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")
	period = clgeti ("period")
	offset = clgeti ("offset")

	status = eoven (noven, ncomp, period, offset)
	if (status != 0) {
	    call printf ("eoven error %d\n")
		call pargi (status)
	}
end

#	t_goven - get the parameters for the goven task

procedure t_goven ()
int	noven
int	ncomp
int	period
int	offset
int	status
int	clgeti()
int	goven()
errchk	goven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")
	period = clgeti ("period")
	offset = clgeti ("offset")

	status = goven (noven, ncomp, period, offset)
	if (status != 0) {
	    call printf ("goven error %d\n")
		call pargi (status)
	}
end

#	t_woven - get the parameters for the woven task

procedure t_woven ()
int	noven
int	ncomp
int	period
int	offset
int	status
int	clgeti()
int	woven()
errchk	woven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")
	period = clgeti ("period")
	offset = clgeti ("offset")

	status = woven (noven, ncomp, period, offset)
	if (status != 0) {
	    call printf ("woven error %d\n")
		call pargi (status)
	}
end

#	t_poven - get the parameters for the poven task

procedure t_poven ()
int	noven
int	ncomp
int	clgeti()
int	status
int	poven()
errchk	poven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")

	status = poven (noven, ncomp)
	if (status != 0) {
	    call printf ("poven error %d\n")
		call pargi (status)
	}
end

#	t_boven - get the parameters for the boven task

procedure t_boven ()
int	noven
int	ncomp
int	clgeti()
int	status
int	boven()
errchk	boven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")

	status = boven (noven, ncomp)
	if (status != 0) {
	    call printf ("boven error %d\n")
		call pargi (status)
	}
end

#	oveno - get the database parameters for ograph, odisp

int	procedure oveno ()
int	noven
int	ncomp
int	clgeti()
int	status
int	ooven()

begin
	noven = clgeti ("noven")
	ncomp = clgeti ("ncomp")

	status = ooven (noven, ncomp)
	return (status)
end
