C   ROUTINE NAME   - IQHSE
C
C-----------------------------------------------------------------------
C
C   COMPUTER            - VAX/DOUBLE
C
C   LATEST REVISION     - JUNE 1, 1980
C
C   PURPOSE             - NUCLEUS CALLED ONLY BY SUBROUTINE IQHSCV
C
C   PRECISION/HARDWARE  - SINGLE AND DOUBLE/H32
C                       - SINGLE/H36,H48,H60
C
C   REQD. ROUTINES      - NONE
C
C   NOTATION            - INFORMATION ON SPECIAL NOTATION AND
C                           CONVENTIONS IS AVAILABLE IN THE MANUAL
C                           INTRODUCTION OR THROUGH ROUTINE UHELP
C
C-----------------------------------------------------------------------
C
      SUBROUTINE IQHSE  (NDP,XD,YD,ZD,NT,IPT,PD,WK)
C                                  SPECIFICATIONS FOR ARGUMENTS
      INTEGER            NDP,NT,IPT(1)
      DOUBLE PRECISION   XD(1),YD(1),ZD(1),PD(1),WK(1)
C                                  SPECIFICATIONS FOR LOCAL VARIABLES
      INTEGER            IDP,IPTI(3),IT,IV,JPD0,JPDMX,JPD,JPT0,JPT,NDP0,
     1                   NT0
      DOUBLE PRECISION   DX1,DX2,DY1,DY2,DZ1,DZ2,DZX1,DZX2,DZY1,DZY2,
     1                   EPSLN,TOL,XV(3),VPXX,VPXY,VPX,VPYX,VPYY,VPY,
     2                   VPZMN,VPZ,YV(3),ZV(3),ZXV(3),ZYV(3)
C                                  MACHINE PRECISION
      DATA               TOL/2.775557562D-17/
C                                  FIRST EXECUTABLE STATEMENT
      EPSLN = TOL*100.0D0
C                                  PRELIMINARY PROCESSING
      NDP0 = NDP
      NT0 = NT
C                                  CLEARS THE PD ARRAY.
      JPDMX = 5*NDP0
      DO 5 JPD=1,JPDMX
         PD(JPD) = 0.0D0
    5 CONTINUE
      DO 10 IDP=1,NDP
         WK(IDP) = 0.0D0
   10 CONTINUE
C                                  ESTIMATES ZX AND ZY.
      DO 25 IT=1,NT0
         JPT0 = 3*(IT-1)
         DO 15 IV=1,3
            JPT = JPT0+IV
            IDP = IPT(JPT)
            IPTI(IV) = IDP
            XV(IV) = XD(IDP)
            YV(IV) = YD(IDP)
            ZV(IV) = ZD(IDP)
   15    CONTINUE
         DX1 = XV(2)-XV(1)
         DY1 = YV(2)-YV(1)
         DZ1 = ZV(2)-ZV(1)
         DX2 = XV(3)-XV(1)
         DY2 = YV(3)-YV(1)
         DZ2 = ZV(3)-ZV(1)
         VPX = DY1*DZ2-DZ1*DY2
         VPY = DZ1*DX2-DX1*DZ2
         VPZ = DX1*DY2-DY1*DX2
         VPZMN = DABS(DX1*DX2+DY1*DY2)*EPSLN
         IF (DABS(VPZ).LE.VPZMN) GO TO 25
         DO 20 IV=1,3
            IDP = IPTI(IV)
            JPD0 = 5*(IDP-1)+1
            PD(JPD0) = PD(JPD0)+VPX
            PD(JPD0+1) = PD(JPD0+1)+VPY
            WK(IDP) = WK(IDP)+VPZ
   20    CONTINUE
   25 CONTINUE
      DO 30 IDP=1,NDP0
         JPD0 = 5*(IDP-1)+1
         PD(JPD0) = -PD(JPD0)/WK(IDP)
         PD(JPD0+1) = -PD(JPD0+1)/WK(IDP)
   30 CONTINUE
C                                  ESTIMATES ZXX, ZXY, AND ZYY.
      DO 45 IT=1,NT0
         JPT0 = 3*(IT-1)
         DO 35 IV=1,3
            JPT = JPT0+IV
            IDP = IPT(JPT)
            IPTI(IV) = IDP
            XV(IV) = XD(IDP)
            YV(IV) = YD(IDP)
            JPD0 = 5*(IDP-1)+1
            ZXV(IV) = PD(JPD0)
            ZYV(IV) = PD(JPD0+1)
   35    CONTINUE
         DX1 = XV(2)-XV(1)
         DY1 = YV(2)-YV(1)
         DZX1 = ZXV(2)-ZXV(1)
         DZY1 = ZYV(2)-ZYV(1)
         DX2 = XV(3)-XV(1)
         DY2 = YV(3)-YV(1)
         DZX2 = ZXV(3)-ZXV(1)
         DZY2 = ZYV(3)-ZYV(1)
         VPXX = DY1*DZX2-DZX1*DY2
         VPXY = DZX1*DX2-DX1*DZX2
         VPYX = DY1*DZY2-DZY1*DY2
         VPYY = DZY1*DX2-DX1*DZY2
         VPZ = DX1*DY2-DY1*DX2
         VPZMN = DABS(DX1*DX2+DY1*DY2)*EPSLN
         IF (DABS(VPZ).LE.VPZMN) GO TO 45
         DO 40 IV=1,3
            IDP = IPTI(IV)
            JPD0 = 5*(IDP-1)+3
            PD(JPD0) = PD(JPD0)+VPXX
            PD(JPD0+1) = PD(JPD0+1)+VPXY+VPYX
            PD(JPD0+2) = PD(JPD0+2)+VPYY
   40    CONTINUE
   45 CONTINUE
      DO 50 IDP=1,NDP0
         JPD0 = 5*(IDP-1)+3
         PD(JPD0) = -PD(JPD0)/WK(IDP)
         PD(JPD0+1) = -PD(JPD0+1)/(2.0D0*WK(IDP))
         PD(JPD0+2) = -PD(JPD0+2)/WK(IDP)
   50 CONTINUE
      RETURN
      END
