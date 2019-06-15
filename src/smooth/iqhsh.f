C   ROUTINE NAME   - IQHSH
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
      SUBROUTINE IQHSH  (XD,YD,NT,IPT,NL,IPL,NXI,NYI,XI,YI,NGP,IGP)
C                                  SPECIFICATIONS FOR ARGUMENTS
      INTEGER            NT,NL,NXI,NYI,IPT(1),IPL(1),NGP(1),IGP(1)
      DOUBLE PRECISION   XD(1),YD(1),XI(1),YI(1)
C                                  SPECIFICATIONS FOR LOCAL VARIABLES
      INTEGER            IL0T3,IL0,ILP1T3,ILP1,INSD,IP1,IP2,IP3,IT0T3,
     1                   IT0,IXIMN,IXIMX,IXI,IYI,IZI,JIGP0,JIGP1I,JIGP1,
     2                   JNGP0,JNGP1,L,NGP0,NGP1,NL0,NT0,NXI0,NXINYI,
     3                   NYI0
      DOUBLE PRECISION   X1,X2,X3,XII,XIMN,XIMX,XMN,XMX,Y1,Y2,Y3,YII,
     1                   YIMN,YIMX,YMN,YMX
      DOUBLE PRECISION   SPDT,VPDT,U1,U2,U3,V1,V2,V3
C                                  STATEMENT FUNCTIONS
      SPDT(U1,V1,U2,V2,U3,V3) = (U1-U2)*(U3-U2)+(V1-V2)*(V3-V2)
      VPDT(U1,V1,U2,V2,U3,V3) = (U1-U3)*(V2-V3)-(V1-V3)*(U2-U3)
C                                  FIRST EXECUTABLE STATEMENT
      NT0 = NT
      NL0 = NL
      NXI0 = NXI
      NYI0 = NYI
      NXINYI = NXI0*NYI0
      XIMN = DMIN1(XI(1),XI(NXI0))
      XIMX = DMAX1(XI(1),XI(NXI0))
      YIMN = DMIN1(YI(1),YI(NYI0))
      YIMX = DMAX1(YI(1),YI(NYI0))
C                                  DETERMINES GRID POINTS INSIDE THE
C                                    DATA AREA.
      JNGP0 = 0
      JNGP1 = 2*(NT0+2*NL0)+1
      JIGP0 = 0
      JIGP1 = NXINYI+1
      DO 80 IT0=1,NT0
         NGP0 = 0
         NGP1 = 0
         IT0T3 = IT0*3
         IP1 = IPT(IT0T3-2)
         IP2 = IPT(IT0T3-1)
         IP3 = IPT(IT0T3)
         X1 = XD(IP1)
         Y1 = YD(IP1)
         X2 = XD(IP2)
         Y2 = YD(IP2)
         X3 = XD(IP3)
         Y3 = YD(IP3)
         XMN = DMIN1(X1,X2,X3)
         XMX = DMAX1(X1,X2,X3)
         YMN = DMIN1(Y1,Y2,Y3)
         YMX = DMAX1(Y1,Y2,Y3)
         INSD = 0
         DO 10 IXI=1,NXI0
            IF (XI(IXI).GE.XMN.AND.XI(IXI).LE.XMX) GO TO 5
            IF (INSD.EQ.0) GO TO 10
            IXIMX = IXI-1
            GO TO 15
    5       IF (INSD.EQ.1) GO TO 10
            INSD = 1
            IXIMN = IXI
   10    CONTINUE
         IF (INSD.EQ.0) GO TO 75
         IXIMX = NXI0
   15    DO 70 IYI=1,NYI0
            YII = YI(IYI)
            IF (YII.LT.YMN.OR.YII.GT.YMX) GO TO 70
            DO 65 IXI=IXIMN,IXIMX
               XII = XI(IXI)
               L = 0
               IF (VPDT(X1,Y1,X2,Y2,XII,YII)) 65,20,25
   20          L = 1
   25          IF (VPDT(X2,Y2,X3,Y3,XII,YII)) 65,30,35
   30          L = 1
   35          IF (VPDT(X3,Y3,X1,Y1,XII,YII)) 65,40,45
   40          L = 1
   45          IZI = NXI0*(IYI-1)+IXI
               IF (L.EQ.1) GO TO 50
               NGP0 = NGP0+1
               JIGP0 = JIGP0+1
               IGP(JIGP0) = IZI
               GO TO 65
   50          IF (JIGP1.GT.NXINYI) GO TO 60
               DO 55 JIGP1I=JIGP1,NXINYI
                  IF (IZI.EQ.IGP(JIGP1I)) GO TO 65
   55          CONTINUE
   60          NGP1 = NGP1+1
               JIGP1 = JIGP1-1
               IGP(JIGP1) = IZI
   65       CONTINUE
   70    CONTINUE
   75    JNGP0 = JNGP0+1
         NGP(JNGP0) = NGP0
         JNGP1 = JNGP1-1
         NGP(JNGP1) = NGP1
   80 CONTINUE
C                                  DETERMINES GRID POINTS OUTSIDE THE
C                                    DATA AREA. - IN SEMI-INFINITE
C                                    RECTANGULAR AREA.
      DO 225 IL0=1,NL0
         NGP0 = 0
         NGP1 = 0
         IL0T3 = IL0*3
         IP1 = IPL(IL0T3-2)
         IP2 = IPL(IL0T3-1)
         X1 = XD(IP1)
         Y1 = YD(IP1)
         X2 = XD(IP2)
         Y2 = YD(IP2)
         XMN = XIMN
         XMX = XIMX
         YMN = YIMN
         YMX = YIMX
         IF (Y2.GE.Y1) XMN = DMIN1(X1,X2)
         IF (Y2.LE.Y1) XMX = DMAX1(X1,X2)
         IF (X2.LE.X1) YMN = DMIN1(Y1,Y2)
         IF (X2.GE.X1) YMX = DMAX1(Y1,Y2)
         INSD = 0
         DO 90 IXI=1,NXI0
            IF (XI(IXI).GE.XMN.AND.XI(IXI).LE.XMX) GO TO 85
            IF (INSD.EQ.0) GO TO 90
            IXIMX = IXI-1
            GO TO 95
   85       IF (INSD.EQ.1) GO TO 90
            INSD = 1
            IXIMN = IXI
   90    CONTINUE
         IF (INSD.EQ.0) GO TO 155
         IXIMX = NXI0
   95    DO 150 IYI=1,NYI0
            YII = YI(IYI)
            IF (YII.LT.YMN.OR.YII.GT.YMX) GO TO 150
            DO 145 IXI=IXIMN,IXIMX
               XII = XI(IXI)
               L = 0
               IF (VPDT(X1,Y1,X2,Y2,XII,YII)) 105,100,145
  100          L = 1
  105          IF (SPDT(X2,Y2,X1,Y1,XII,YII)) 145,110,115
  110          L = 1
  115          IF (SPDT(X1,Y1,X2,Y2,XII,YII)) 145,120,125
  120          L = 1
  125          IZI = NXI0*(IYI-1)+IXI
               IF (L.EQ.1) GO TO 130
               NGP0 = NGP0+1
               JIGP0 = JIGP0+1
               IGP(JIGP0) = IZI
               GO TO 145
  130          IF (JIGP1.GT.NXINYI) GO TO 140
               DO 135 JIGP1I=JIGP1,NXINYI
                  IF (IZI.EQ.IGP(JIGP1I)) GO TO 145
  135          CONTINUE
  140          NGP1 = NGP1+1
               JIGP1 = JIGP1-1
               IGP(JIGP1) = IZI
  145       CONTINUE
  150    CONTINUE
  155    JNGP0 = JNGP0+1
         NGP(JNGP0) = NGP0
         JNGP1 = JNGP1-1
         NGP(JNGP1) = NGP1
C                                  - IN SEMI-INFINITE TRIANGULAR AREA.
         NGP0 = 0
         NGP1 = 0
         ILP1 = MOD(IL0,NL0)+1
         ILP1T3 = ILP1*3
         IP3 = IPL(ILP1T3-1)
         X3 = XD(IP3)
         Y3 = YD(IP3)
         XMN = XIMN
         XMX = XIMX
         YMN = YIMN
         YMX = YIMX
         IF (Y3.GE.Y2.AND.Y2.GE.Y1) XMN = X2
         IF (Y3.LE.Y2.AND.Y2.LE.Y1) XMX = X2
         IF (X3.LE.X2.AND.X2.LE.X1) YMN = Y2
         IF (X3.GE.X2.AND.X2.GE.X1) YMX = Y2
         INSD = 0
         DO 165 IXI=1,NXI0
            IF (XI(IXI).GE.XMN.AND.XI(IXI).LE.XMX) GO TO 160
            IF (INSD.EQ.0) GO TO 165
            IXIMX = IXI-1
            GO TO 170
  160       IF (INSD.EQ.1) GO TO 165
            INSD = 1
            IXIMN = IXI
  165    CONTINUE
         IF (INSD.EQ.0) GO TO 220
         IXIMX = NXI0
  170    DO 215 IYI=1,NYI0
            YII = YI(IYI)
            IF (YII.LT.YMN.OR.YII.GT.YMX) GO TO 215
            DO 210 IXI=IXIMN,IXIMX
               XII = XI(IXI)
               L = 0
               IF (SPDT(X1,Y1,X2,Y2,XII,YII)) 180,175,210
  175          L = 1
  180          IF (SPDT(X3,Y3,X2,Y2,XII,YII)) 190,185,210
  185          L = 1
  190          IZI = NXI0*(IYI-1)+IXI
               IF (L.EQ.1) GO TO 195
               NGP0 = NGP0+1
               JIGP0 = JIGP0+1
               IGP(JIGP0) = IZI
               GO TO 210
  195          IF (JIGP1.GT.NXINYI) GO TO 205
               DO 200 JIGP1I=JIGP1,NXINYI
                  IF (IZI.EQ.IGP(JIGP1I)) GO TO 210
  200          CONTINUE
  205          NGP1 = NGP1+1
               JIGP1 = JIGP1-1
               IGP(JIGP1) = IZI
  210       CONTINUE
  215    CONTINUE
  220    JNGP0 = JNGP0+1
         NGP(JNGP0) = NGP0
         JNGP1 = JNGP1-1
         NGP(JNGP1) = NGP1
  225 CONTINUE
      RETURN
      END
