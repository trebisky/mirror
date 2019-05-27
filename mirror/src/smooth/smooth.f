C	***********************************************************************
C	Subroutine SMOOTH					version 1.0

C	Date	1/11/1988
C	***********************************************************************

	subroutine smooth(xd,yd,zd,nd,nxi,nyi,zi,
     +                    xmin,xmax,ymin,ymax,ier)
	integer nd,nxi,nyi,izi,ier,i,iwk(84136)
	double precision xd(nd),yd(nd),zd(nd),zi(nxi,nyi)
	double precision xi(1024),yi(1024)
	double precision xmin,xmax,ymin,ymax,xinc,yinc,wk(3600)


	xinc= (xmax-xmin)/DBLE(nxi)
	yinc= (ymax-ymin)/DBLE(nyi)
	xi(1)= xmin
	yi(1)= ymin

	do 11 i=2,nxi
11		xi(i)= xi(i-1)+xinc

	do 12 i=2,nyi
12		yi(i)= yi(i-1)+yinc

	izi= nxi

	call iqhscv(xd,yd,zd,nd,xi,nxi,yi,nyi,zi,izi,iwk,wk,ier)
	
	return
	end
