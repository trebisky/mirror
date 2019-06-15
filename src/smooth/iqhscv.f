C   ROUTINE NAME   - IQHSCV
C
C-----------------------------------------------------------------------
C
C   COMPUTER            - VAX/DOUBLE
C
C   LATEST REVISION     - JUNE 1, 1981
C
C   PURPOSE             - SMOOTH SURFACE FITTING WITH IRREGULARLY
C                           DISTRIBUTED DATA POINTS
C
C   USAGE               - CALL IQHSCV (XD,YD,ZD,ND,XI,NXI,YI,NYI,ZI,IZI,
C                           IWK,WK,IER)
C
C   ARGUMENTS    XD     - VECTOR OF LENGTH ND. (INPUT)
C                YD     - VECTOR OF LENGTH ND. (INPUT)
C                ZD     - VECTOR OF LENGTH ND. (INPUT)
C                         ZD(I) IS THE FUNCTION VALUE AT THE POINT
C                           (XD(I),YD(I)) FOR I=1,...,ND.
C                ND     - NUMBER OF DATA POINTS. (INPUT) ND MUST BE
C                           GREATER THAN OR EQUAL TO 4.
C                XI     - VECTOR OF LENGTH NXI. (INPUT)
C                           THE SEQUENCE XI MUST BE MONOTONE
C                           INCREASING.
C                NXI    - NUMBER OF ELEMENTS IN XI. (INPUT) NXI MUST
C                           BE GREATER THAN OR EQUAL TO 1.
C                YI     - VECTOR OF LENGTH NYI. (INPUT)
C                           THE SEQUENCE YI MUST BE MONOTONE
C                           INCREASING.
C                NYI    - NUMBER OF ELEMENTS IN YI. (INPUT) NYI MUST
C                           BE GREATER THAN OR EQUAL TO 1.
C                ZI     - NXI BY NYI MATRIX CONTAINING THE INTERPOLATED
C                           VALUES. (OUTPUT) ZI(I,J) IS THE ESTIMATED
C                           VALUE AT THE POINT (XI(I),YI(J)) FOR
C                           I=1,...,NXI AND J=1,...,NYI.
C                IZI    - ROW DIMENSION OF THE MATRIX ZI EXACTLY AS
C                           SPECIFIED IN THE DIMENSION STATEMENT
C                           IN THE CALLING PROGRAM. (INPUT)
C                IWK    - INTEGER WORK VECTOR OF LENGTH
C                           31*ND+NXI*NYI.
C                WK     - REAL WORK VECTOR OF LENGTH 6*ND.
C                IER    - ERROR PARAMETER. (OUTPUT)
C                         TERMINAL ERROR
C                           IER = 129, ND IS LESS THAN 4, OR NXI
C                             IS LESS THAN 1, OR NYI IS LESS THAN 1.
C                           IER = 130, ALL DATA POINTS ARE COLLINEAR
C                           IER = 131, SOME DATA POINTS ARE IDENTICAL,
C                             THAT IS, XD(I)=XD(J) AND YD(I)=YD(J) FOR
C                             SOME I AND J, I NOT EQUAL TO J AND
C                             I=1,...,ND, J=1,...,ND.
C
C   PRECISION/HARDWARE  - SINGLE AND DOUBLE/H32
C                       - SINGLE/H36,H48,H60
C
C   REQD. ROUTINES      - IQHSD,IQHSE,IQHSF,IQHSG,IQHSH,UERTST,UGETIO
C
C   NOTATION            - INFORMATION ON SPECIAL NOTATION AND
C                           CONVENTIONS IS AVAILABLE IN THE MANUAL
C                           INTRODUCTION OR THROUGH ROUTINE UHELP
C
C-----------------------------------------------------------------------
C
      SUBROUTINE IQHSCV (XD,YD,ZD,ND,XI,NXI,YI,NYI,ZI,IZI,IWK,WK,IER)
C                                  SPECIFICATIONS FOR ARGUMENTS
      INTEGER            ND,NXI,NYI,IZI,IER,IWK(1)
      DOUBLE PRECISION   XD(1),YD(1),ZD(1),XI(1),YI(1),ZI(IZI,1),WK(1)
C                                  SPECIFICATIONS FOR LOCAL VARIABLES
      INTEGER            IL1,IL2,ITI,IXI,IYI,IZ,JIG0MN,JIG0MX,JIG1MN,
     1                   JIG1MX,JIGP,JNGP,JWIGP0,JWIGP,JWIPL,JWIPT,
     2                   JWIWL,JWIWP,JWNGP0,JWNGP,JWWPD,NDP0,NGP0,NGP1,
     3                   NL,NNGP,NT,NXI0,NYI0
      INTEGER            ITPV
      DOUBLE PRECISION   DMMY(27)
      COMMON /IBCDPT/    DMMY,ITPV
C                                  SETTING OF SOME INPUT PARAMETERS TO
C                                    LOCAL VARIABLES.
C                                  FIRST EXECUTABLE STATEMENT
      IER = 0
      NDP0 = ND
      NXI0 = NXI
      NYI0 = NYI
C                                  ERROR CHECK.
      IF (NDP0.LT.4) GO TO 30
      IF (NXI0.LT.1.OR.NYI0.LT.1) GO TO 30
      IWK(1) = NDP0
      IWK(3) = NXI0
      IWK(4) = NYI0
C                                  ALLOCATION OF STORAGE AREAS IN THE
C                                    IWK ARRAY.
      JWIPT = 16
      JWIWL = 6*NDP0+1
      JWNGP0 = JWIWL-1
      JWIPL = 24*NDP0+1
      JWIWP = 30*NDP0+1
      JWIGP0 = 31*NDP0
      JWWPD = 5*NDP0+1
C                                  TRIANGULATES THE X-Y PLANE.
      CALL IQHSG (NDP0,XD,YD,NT,IWK(JWIPT),NL,IWK(JWIPL),IWK(JWIWL),
     1 IWK(JWIWP),WK,IER)
      IF (IER.GE.128) GO TO 9000
      IWK(5) = NT
      IWK(6) = NL
      IF (NT.EQ.0) GO TO 9005
C                                  SORTS OUTPUT GRID POINTS IN
C                                    ASCENDING ORDER OF THE TRIANGLE
C                                    NUMBER AND THE BORDER LINE SEGMENT
C                                    NUMBER.
      CALL IQHSH (XD,YD,NT,IWK(JWIPT),NL,IWK(JWIPL),NXI0,NYI0,XI,YI,
     1 IWK(JWNGP0+1),IWK(JWIGP0+1))
C
C                                  ESTIMATES PARTIAL DERIVATIVES AT ALL
C                                    DATA POINTS.
C
      CALL IQHSE (NDP0,XD,YD,ZD,NT,IWK(JWIPT),WK,WK(JWWPD))
C
C                                  INTERPOLATES THE ZI VALUES.
      ITPV = 0
      JIG0MX = 0
      JIG1MN = NXI0*NYI0+1
      NNGP = NT+2*NL
      DO 25 JNGP=1,NNGP
         ITI = JNGP
         IF (JNGP.LE.NT) GO TO 5
         IL1 = (JNGP-NT+1)/2
         IL2 = (JNGP-NT+2)/2
         IF (IL2.GT.NL) IL2 = 1
         ITI = IL1*(NT+NL)+IL2
    5    JWNGP = JWNGP0+JNGP
         NGP0 = IWK(JWNGP)
         IF (NGP0.EQ.0) GO TO 15
         JIG0MN = JIG0MX+1
         JIG0MX = JIG0MX+NGP0
         DO 10 JIGP=JIG0MN,JIG0MX
            JWIGP = JWIGP0+JIGP
            IZ = IWK(JWIGP)
            IYI = (IZ-1)/NXI0+1
            IXI = IZ-NXI0*(IYI-1)
            CALL IQHSF (XD,YD,ZD,NT,IWK(JWIPT),NL,IWK(JWIPL),WK,ITI,
     1      XI(IXI),YI(IYI),ZI(IXI,IYI))
   10    CONTINUE
   15    JWNGP = JWNGP0+2*NNGP+1-JNGP
         NGP1 = IWK(JWNGP)
         IF (NGP1.EQ.0) GO TO 25
         JIG1MX = JIG1MN-1
         JIG1MN = JIG1MN-NGP1
         DO 20 JIGP=JIG1MN,JIG1MX
            JWIGP = JWIGP0+JIGP
            IZ = IWK(JWIGP)
            IYI = (IZ-1)/NXI0+1
            IXI = IZ-NXI0*(IYI-1)
            CALL IQHSF (XD,YD,ZD,NT,IWK(JWIPT),NL,IWK(JWIPL),WK,ITI,
     1      XI(IXI),YI(IYI),ZI(IXI,IYI))
   20    CONTINUE
   25 CONTINUE
      GO TO 9005
C                                  ERROR EXIT
   30 IER = 129
 9000 CONTINUE
      CALL UERTST (IER,6HIQHSCV)
 9005 RETURN
      END
