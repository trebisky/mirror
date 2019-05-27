# TC.X - test clip_line routine.

task	tc = test_clip_line

procedure test_clip_line()

int	stat
real	x1, y1, x2, y2, wxl, wxr, wyb, wyt
real	ox1, oy1, ox2, oy2

int	clip_line()
real	clgetr()

begin
	x1 = clgetr ("x1")
	y1 = clgetr ("y1")
	x2 = clgetr ("x2")
	y2 = clgetr ("y2")
	wxl = clgetr ("wxl")
	wxr = clgetr ("wxr")
	wyb = clgetr ("wyb")
	wyt = clgetr ("wyt")

	stat = clip_line (x1, y1, x2, y2, wxl, wxr, wyb, wyt, ox1, oy1, ox2, oy2)

	call printf ("p: %f, %f -- %f, %f  ")
	    call pargr (x1)
	    call pargr (y1)
	    call pargr (x2)
	    call pargr (y2)
	call printf ("w: %f, %f -- %f, %f  ")
	    call pargr (wxl)
	    call pargr (wyb)
	    call pargr (wxr)
	    call pargr (wyt)
	if (stat == YES) {
	    call printf ("s: YES  %f, %f -- %f, %f\n")
		call pargr (ox1)
		call pargr (oy1)
		call pargr (ox2)
		call pargr (oy2)
	} else {
	    call printf ("s: NO\n")
	}
end
