include <math.h>
include <mach.h>
include "../odisp/odisp.h"
include "../odisp/tc_he_bounds.h"
include "../odisp/tc_he.h"
include    "tran.h"

# XYTRAN.X - convert IRAF cartesian display coordinates to OVEN polar
# coordinates and retrieve the nearest thermocouple or heater element.
#
# Written by:    D. A. Harvey            date:    3/9/1988


procedure t_xytran ()

int    	i
int    	min_index, number, offset, minz, maxz, maxr, minr, branch
real    xg, yg, x, y, z, r, t, delta, min_delta, lxg, lyg
char    where[LINE_SIZE], what[LINE_SIZE]
char    element[LINE_SIZE]

bool	clgetb()
real    dist(), clgetr ()
real    delwall ()

include "tran.com"
include "debugx.com"

errchk	init_info

begin
	debug = clgetb ("debug")
	call init_info()

	# get the user request from the command line arguments 
        xg= clgetr ("x")
        yg= clgetr ("y")
        call clgstr ("information", what, LINE_SIZE)
        call clgstr ("aspect", where, LINE_SIZE)

	# Test the "where" argument and set up for "what" test 
        switch (where[1]) {
            case LID_TYPE:
                branch= 10
                call gr_tran_lbm (xg,yg,r,t,x,y)
            case BASE_TYPE:
                branch= 20
                call gr_tran_lbm (xg,yg,r,t,x,y)
            case MOLD_TYPE:
                branch= 30
                call gr_tran_lbm (xg,yg,r,t,x,y)
            case WALL_TYPE:
                branch= 40
                call gr_tran_w (xg,yg,r,t,x,y,z)
            case ALUM_TYPE:
                branch= 50
                call gr_tran_lbm (xg,yg,r,t,x,y)
            default:
                call eprintf ("Usage: Unrecognized aspect: %s\n")
                call pargstr (where)
                branch= 0
        }
        

	# Test the "what" argument and setup for tranlation 
        switch (what[1]) {
            case REQ_TTMP:
                call strcpy("thermocouple", element, LINE_SIZE)
                branch= branch+1
                offset= TOFFSET
                number= last_therm
            case REQ_HPWR:
                call strcpy ("heater element", element, LINE_SIZE)
                branch= branch+2
                offset= HOFFSET
                number= last_heater
            default:
                call eprintf ("Usage: Unrecognized information: %s\n")
                    call pargstr (what)
                branch= 0
        }
        
	# Translate and find nearest element 
        switch (branch) {
            case WALLTMP:
                maxr= TMAXWALL
                minr= TMINWALL
            case WALLHPWR:
                maxr= HMAXWALL
                minr= HMINWALL
            case LIDTMP:
                z= TLID
                minz= TMINLID
                maxz= TMAXLID
            case BASETMP:
                z= TBASE
                minz= TMINBASE
                maxz= TMAXBASE
            case MOLDTMP:    
                z= TMOLD
                minz= TMINMOLD
                maxz= TMAXMOLD
            case ALUMTMP:
                z= TALUM
                minz= TMINALUM
                maxz= TMAXALUM
            case LIDHPWR:
                z= HLID
                minz= HMINLID
                maxz= HMAXLID
            case BASEHPWR:
                z= HBASE
                minz= HMINBASE
                maxz= HMAXBASE
            case MOLDHPWR:    
                call fatal (6,"No such information, aspect combination")
                return
            case ALUMHPWR:
                z= HALUM
                minz= HMINALUM
                maxz= HMAXALUM
            default:
                call eprintf ("Error: Internal - Unknown branch value %d\n")
                    call pargi (branch)
                return
        }

	# Report Oven coordinates of cursor
        call printf (FORMAT)
            call pargstr ("cursor")
	    call pargr (xg)
	    call pargr (yg)
            call pargr (r)
            call pargr (t)
            call pargr (z)

	# Search for nearest element
        min_delta = MAX_REAL
        
	if (where[1] == WALL_TYPE)
            do i = offset, number {
                if (lr[i] >= minr && lr[i] <= maxr) {
                    delta = delwall (x,y,z,i)
                    if (delta < min_delta) {
                        min_index = i
                        min_delta = delta
			if (debug) {
			    call eprintf ("  d**2 = %d at index %d\n")
				call pargr (min_delta)
				call pargi (min_index)
			}
                        if (min_delta == 0.0)
                            break
                    }
	        }
	    }
	else
            do i = offset, number {
                if (lz[i] >= minz && lz[i] <= maxz) {
                    delta = dist(lx[i],ly[i],real(lz[i]),x,y,z)
                    if ( delta < min_delta) {
                        min_index = i
                        min_delta = delta
			if (debug) {
			    call eprintf ("  d**2 = %d at index %d\n")
				call pargr (min_delta)
				call pargi (min_index)
			}
                        if (min_delta == 0.0)
                            break
                    }
                }
	    }

        if (min_delta == MAX_REAL)
            call eprintf ("No element found to match.\n")
        else {
	    if (debug) {
		if (what[1] == REQ_TTMP)
		    call eprintf ("  cdtm")
		else
		    call eprintf ("  cpfe")
		call eprintf (" = '%s'\n")
		    call pargstr (name[1, min_index])
	    }
	    if (where[1] == WALL_TYPE)
                call graphwall(lt[min_index],lz[min_index],lxg,lyg)
	    else
                call graphlbm(lx[min_index],ly[min_index],lxg,lyg)

            call printf (FORMAT)
                call pargstr (element)
                call pargr (lxg)
                call pargr (lyg)
                call pargi (lr[min_index])
                call pargi (lt[min_index])
                call pargi (lz[min_index])
        }
end


# DELWALL - compute the squared distance between two wall coordinates 
real procedure delwall (x,y,z,i)
real    x,y,z
int    i

real    wr,wt,wx,wy
real    dist ()

include "tran.com"
        
begin    
        wr= real(lr[i])
        wt= DEGTORAD((real(lt[i])))
        wx= wr*sin(wt)
        wy= wr*cos(wt)
        return (dist(wx,wy,real(lz[i]),x,y,z))
end


# DIST - compute the squared distance between two cartesian coordinates 
real procedure dist(x1,y1,z1,x2,y2,z2)
real x1,x2,y1,y2,z1,z2

real delx,dely,delz,dis
        
begin
        delx= x1-x2
        dely= y1-y2
        delz= z1-z2
        dis=  (delx*delx)+(dely*dely)+(delz*delz)

        return (dis)
end    


# GRAPHLBM - compute the graphical coordinates of a lid, base, or mold coordinate 
procedure graphlbm(x,y,xg,yg)
real     x,y,xg,yg

begin
        xg= (x/DIAMLBM)*LBMSIZEX+LBMMIDX
        yg= (y/DIAMLBM)*LBMSIZEY+LBMMIDY
end


# GRAPHWALL - compute the graphical coordinates from a wall coordinate
procedure graphwall(t,z,xg,yg)
int     t,z
real    xg,yg

begin
        xg= ((((real(t))-CENTERWALLT)/CIRCLE)*WALLSIZEX)+WALLMIDX
        yg= ((((real(z))-CENTERWALLZ)/WALLSIZEZ)*WALLSIZEY)+WALLMIDY
end


# GR_TRAN_LBM - compute the oven r, theta, and z coordinate of a lid, base,
# or mold graphical coordinate
procedure gr_tran_lbm (xg,yg,r,t,x,y)
real    xg,yg,r,t,x,y
double	ckdatan2()

begin
        x= ((xg-LBMMIDX)/LBMSIZEX)*DIAMLBM
        y= ((yg-LBMMIDY)/LBMSIZEY)*DIAMLBM
        r= sqrt((x*x)+(y*y))
        if (r == 0.0)
            t= 0.0
        else 
            t= RADTODEG(real(ckdatan2(double(x/r),double(y/r))))
        if (t<0.0)
            t= t+CIRCLE
end


# GR_TRAN_W - compute the oven r, theta, and z coordinate of a wall
# graphical coordinate
procedure gr_tran_w (xg,yg,r,t,x,y,z)
real    xg,yg,r,t,x,y,z,tr

begin
        r= RADWALL
        z= ((yg-WALLMIDY)/WALLSIZEY)*WALLSIZEZ+CENTERWALLZ
        t= ((xg-WALLMIDX)/WALLSIZEX)*CIRCLE+CENTERWALLT
        tr= DEGTORAD(t)
        x= r*sin(tr)
        y= r*cos(tr)
end 
