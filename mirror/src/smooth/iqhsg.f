C   ROUTINE NAME   - IQHSG
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
C   REQD. ROUTINES      - IQHSD
C
C   NOTATION            - INFORMATION ON SPECIAL NOTATION AND
C                           CONVENTIONS IS AVAILABLE IN THE MANUAL
C                           INTRODUCTION OR THROUGH ROUTINE UHELP
C
C-----------------------------------------------------------------------
C
      SUBROUTINE IQHSG  (NDP,XD,YD,NT,IPT,NL,IPL,IWL,IWP,WK,IER)
C                                  SPECIFICATIONS FOR ARGUMENTS
      INTEGER            NDP,NT,NL,IER,IPT(1),IPL(1),IWL(1),IWP(1)
      DOUBLE PRECISION   XD(1),YD(1),WK(1)
C                                  SPECIFICATIONS FOR LOCAL VARIABLES
      INTEGER            ILF,ILIV,ILT3,ILVS,IL,IP1P1,IP1,IP2,IP3,IPL1,
     1                   IPL2,IPLJ1,IPLJ2,IPMN1,IPMN2,IPT1,IPT2,IPT3,
     2                   IPTI1,IPTI2,IPTI,IP,IREP,IT1T3,IT2T3,ITF(2),
     3                   ITS,ITT3R,ITT3,IT,IXVSPV,IXVS,JL1,JL2,JLT3,JP1,
     4                   JP2,JPC,JPMN,JPMX,JP,JWL1MN,JWL1,JWL,NDP0,
     5                   NDPM1,NL0,NLFC,NLFT2,NLF,NLNT3,NLN,NLSHT3,NLSH,
     6                   NLT3,NREP,NT0,NTF,IQHSD,NTT3P3,NTT3
      DOUBLE PRECISION   DSQI,DSQMN,EPSLN,SP,TOL,VP,X1,X2,X3,XDMP,Y1,Y2,
     1                   Y3,YDMP
      DOUBLE PRECISION   DSQF,SPDT,VPDT,U1,U2,U3,V1,V2,V3
      DATA               NREP/100/
C                                  MACHINE PRECISION
      DATA               TOL/2.775557562D-17/
C                                  STATEMENT FUNCTIONS
      DSQF(U1,V1,U2,V2) = (U2-U1)**2+(V2-V1)**2
      SPDT(U1,V1,U2,V2,U3,V3) = (U2-U1)*(U3-U1)+(V2-V1)*(V3-V1)
      VPDT(U1,V1,U2,V2,U3,V3) = (V3-V1)*(U2-U1)-(U3-U1)*(V2-V1)
C                                  FIRST EXECUTABLE STATEMENT
      EPSLN = TOL*100.0D0
C                                  PRELIMINARY PROCESSING
      NDP0 = NDP
      NDPM1 = NDP0-1
C                                  DETERMINES THE CLOSEST PAIR OF DATA
C                                    POINTS AND THEIR MIDPOINT.
      DSQMN = DSQF(XD(1),YD(1),XD(2),YD(2))
      IPMN1 = 1
      IPMN2 = 2
      DO 10 IP1=1,NDPM1
         X1 = XD(IP1)
         Y1 = YD(IP1)
         IP1P1 = IP1+1
         DO 5 IP2=IP1P1,NDP0
            DSQI = DSQF(X1,Y1,XD(IP2),YD(IP2))
            IF (DSQI.EQ.0.0D0) GO TO 160
            IF (DSQI.GE.DSQMN) GO TO 5
            DSQMN = DSQI
            IPMN1 = IP1
            IPMN2 = IP2
    5    CONTINUE
   10 CONTINUE
      XDMP = (XD(IPMN1)+XD(IPMN2))/2.0D0
      YDMP = (YD(IPMN1)+YD(IPMN2))/2.0D0
C                                  SORTS THE OTHER (NDP-2) DATA POINTS
C                                    IN ASCENDING ORDER OF DISTANCE
C                                    FROM THE MIDPOINT AND STORES THE
C                                    SORTED DATA POINT NUMBERS IN THE
C                                    IWP ARRAY.
      JP1 = 2
      DO 15 IP1=1,NDP0
         IF (IP1.EQ.IPMN1.OR.IP1.EQ.IPMN2) GO TO 15
         JP1 = JP1+1
         IWP(JP1) = IP1
         WK(JP1) = DSQF(XDMP,YDMP,XD(IP1),YD(IP1))
   15 CONTINUE
      DO 25 JP1=3,NDPM1
         DSQMN = WK(JP1)
         JPMN = JP1
         DO 20 JP2=JP1,NDP0
            IF (WK(JP2).GE.DSQMN) GO TO 20
            DSQMN = WK(JP2)
            JPMN = JP2
   20    CONTINUE
         ITS = IWP(JP1)
         IWP(JP1) = IWP(JPMN)
         IWP(JPMN) = ITS
         WK(JPMN) = WK(JP1)
   25 CONTINUE
C                                  IF NECESSARY, MODIFIES THE ORDERING
C                                    IN SUCH A WAY THAT THE FIRST THREE
C                                    DATA POINTS ARE NOT COLLINEAR.
      X1 = XD(IPMN1)
      Y1 = YD(IPMN1)
      X2 = XD(IPMN2)
      Y2 = YD(IPMN2)
      DO 30 JP=3,NDP0
         IP = IWP(JP)
         SP = SPDT(XD(IP),YD(IP),X1,Y1,X2,Y2)
         VP = VPDT(XD(IP),YD(IP),X1,Y1,X2,Y2)
         IF (DABS(VP).GT.(DABS(SP)*EPSLN)) GO TO 35
   30 CONTINUE
      GO TO 165
   35 IF (JP.EQ.3) GO TO 45
      JPMX = JP
      DO 40 JPC=4,JPMX
         JP = JPMX+4-JPC
         IWP(JP) = IWP(JP-1)
   40 CONTINUE
      IWP(3) = IP
C                                  FORMS THE FIRST TRIANGLE. STORES
C                                    POINT NUMBERS OF THE VERTEXES OF
C                                    THE TRIANGLE IN THE IPT ARRAY, AND
C                                    STORES POINT NUMBERS OF THE
C                                    BORDER LINE SEGMENTS AND THE
C                                    TRIANGLE NUMBER IN THE IPL ARRAY.
   45 IP1 = IPMN1
      IP2 = IPMN2
      IP3 = IWP(3)
      IF (VPDT(XD(IP1),YD(IP1),XD(IP2),YD(IP2),XD(IP3),YD(IP3)).GE.0.0D0
     1)
     +GO TO 50
      IP1 = IPMN2
      IP2 = IPMN1
   50 NT0 = 1
      NTT3 = 3
      IPT(1) = IP1
      IPT(2) = IP2
      IPT(3) = IP3
      NL0 = 3
      NLT3 = 9
      IPL(1) = IP1
      IPL(2) = IP2
      IPL(3) = 1
      IPL(4) = IP2
      IPL(5) = IP3
      IPL(6) = 1
      IPL(7) = IP3
      IPL(8) = IP1
      IPL(9) = 1
C                                  ADDS THE REMAINING (NDP-3) DATA
C                                    POINTS, ONE BY ONE.
      DO 150 JP1=4,NDP0
         IP1 = IWP(JP1)
         X1 = XD(IP1)
         Y1 = YD(IP1)
C                                  DETERMINES THE FIRST INVISIBLE AND
C                                    VISIBLE BORDER LINE SEGMENTS,
C                                    ILIV AND ILVS.
         DO 65 IL=1,NL0
            IP2 = IPL(3*IL-2)
            IP3 = IPL(3*IL-1)
            X2 = XD(IP2)
            Y2 = YD(IP2)
            X3 = XD(IP3)
            Y3 = YD(IP3)
            SP = SPDT(X1,Y1,X2,Y2,X3,Y3)
            VP = VPDT(X1,Y1,X2,Y2,X3,Y3)
            IF (IL.NE.1) GO TO 55
            IXVS = 0
            IF (VP.LE.(DABS(SP)*(-EPSLN))) IXVS = 1
            ILIV = 1
            ILVS = 1
            GO TO 65
   55       IXVSPV = IXVS
            IF (VP.GT.(DABS(SP)*(-EPSLN))) GO TO 60
            IXVS = 1
            IF (IXVSPV.EQ.1) GO TO 65
            ILVS = IL
            IF (ILIV.NE.1) GO TO 70
            GO TO 65
   60       IXVS = 0
            IF (IXVSPV.EQ.0) GO TO 65
            ILIV = IL
            IF (ILVS.NE.1) GO TO 70
   65    CONTINUE
         IF (ILIV.EQ.1.AND.ILVS.EQ.1) ILVS = NL0
   70    IF (ILVS.LT.ILIV) ILVS = ILVS+NL0
C                                  SHIFTS (ROTATES) THE IPL ARRAY TO
C                                    HAVE THE INVISIBLE BORDER LINE
C                                    SEGMENTS CONTAINED IN THE FIRST
C                                    PART OF THE IPL ARRAY.
         IF (ILIV.EQ.1) GO TO 85
         NLSH = ILIV-1
         NLSHT3 = NLSH*3
         DO 75 JL1=1,NLSHT3
            JL2 = JL1+NLT3
            IPL(JL2) = IPL(JL1)
   75    CONTINUE
         DO 80 JL1=1,NLT3
            JL2 = JL1+NLSHT3
            IPL(JL1) = IPL(JL2)
   80    CONTINUE
         ILVS = ILVS-NLSH
C                                  ADDS TRIANGLES TO THE IPT ARRAY,
C                                    UPDATES BORDER LINE SEGMENTS IN
C                                    THE IPL ARRAY, AND SETS FLAGS FOR
C                                    THE BORDER LINE SEGMENTS TO BE
C                                    REEXAMINED IN THE IWL ARRAY.
   85    JWL = 0
         DO 105 IL=ILVS,NL0
            ILT3 = IL*3
            IPL1 = IPL(ILT3-2)
            IPL2 = IPL(ILT3-1)
            IT = IPL(ILT3)
C                                  ADDS A TRIANGLE TO THE IPT
C                                    ARRAY.
            NT0 = NT0+1
            NTT3 = NTT3+3
            IPT(NTT3-2) = IPL2
            IPT(NTT3-1) = IPL1
            IPT(NTT3) = IP1
C                                  UPDATES BORDER LINE SEGMENTS IN
C                                    THE IPL ARRAY.
            IF (IL.NE.ILVS) GO TO 90
            IPL(ILT3-1) = IP1
            IPL(ILT3) = NT0
   90       IF (IL.NE.NL0) GO TO 95
            NLN = ILVS+1
            NLNT3 = NLN*3
            IPL(NLNT3-2) = IP1
            IPL(NLNT3-1) = IPL(1)
            IPL(NLNT3) = NT0
C                                  DETERMINES THE VERTEX THAT DOES
C                                    NOT LIE ON THE BORDER LINE
C                                    SEGMENTS.
   95       ITT3 = IT*3
            IPTI = IPT(ITT3-2)
            IF (IPTI.NE.IPL1.AND.IPTI.NE.IPL2) GO TO 100
            IPTI = IPT(ITT3-1)
            IF (IPTI.NE.IPL1.AND.IPTI.NE.IPL2) GO TO 100
            IPTI = IPT(ITT3)
C                                  CHECKS IF THE EXCHANGE IS
C                                    NECESSARY.
C
  100       IF (IQHSD(XD,YD,IP1,IPTI,IPL1,IPL2).EQ.0) GO TO 105
C
C                                  MODIFIES THE IPT ARRAY WHEN
C                                    NECESSARY.
            IPT(ITT3-2) = IPTI
            IPT(ITT3-1) = IPL1
            IPT(ITT3) = IP1
            IPT(NTT3-1) = IPTI
            IF (IL.EQ.ILVS) IPL(ILT3) = IT
            IF (IL.EQ.NL0.AND.IPL(3).EQ.IT) IPL(3) = NT0
C
C                                  SETS FLAGS IN THE IWL ARRAY.
            JWL = JWL+4
            IWL(JWL-3) = IPL1
            IWL(JWL-2) = IPTI
            IWL(JWL-1) = IPTI
            IWL(JWL) = IPL2
  105    CONTINUE
         NL0 = NLN
         NLT3 = NLNT3
         NLF = JWL/2
         IF (NLF.EQ.0) GO TO 150
C                                  IMPROVES TRIANGULATION.
         NTT3P3 = NTT3+3
         DO 145 IREP=1,NREP
            DO 135 ILF=1,NLF
               IPL1 = IWL(2*ILF-1)
               IPL2 = IWL(2*ILF)
C                                  LOCATES IN THE IPT ARRAY TWO
C                                    TRIANGLES ON BOTH SIDES OF THE
C                                    FLAGGED LINE SEGMENT.
               NTF = 0
               DO 110 ITT3R=3,NTT3,3
                  ITT3 = NTT3P3-ITT3R
                  IPT1 = IPT(ITT3-2)
                  IPT2 = IPT(ITT3-1)
                  IPT3 = IPT(ITT3)
                  IF (IPL1.NE.IPT1.AND.IPL1.NE.IPT2.AND.IPL1.NE.IPT3) GO
     1             TO 110
                  IF (IPL2.NE.IPT1.AND.IPL2.NE.IPT2.AND.IPL2.NE.IPT3) GO
     1             TO 110
                  NTF = NTF+1
                  ITF(NTF) = ITT3/3
                  IF (NTF.EQ.2) GO TO 115
  110          CONTINUE
               IF (NTF.LT.2) GO TO 135
C                                  DETERMINES THE VERTEXES OF THE
C                                    TRIANGLES THAT DO NOT LIE ON
C                                    THE LINE SEGMENT.
  115          IT1T3 = ITF(1)*3
               IPTI1 = IPT(IT1T3-2)
               IF (IPTI1.NE.IPL1.AND.IPTI1.NE.IPL2) GO TO 120
               IPTI1 = IPT(IT1T3-1)
               IF (IPTI1.NE.IPL1.AND.IPTI1.NE.IPL2) GO TO 120
               IPTI1 = IPT(IT1T3)
  120          IT2T3 = ITF(2)*3
               IPTI2 = IPT(IT2T3-2)
               IF (IPTI2.NE.IPL1.AND.IPTI2.NE.IPL2) GO TO 125
               IPTI2 = IPT(IT2T3-1)
               IF (IPTI2.NE.IPL1.AND.IPTI2.NE.IPL2) GO TO 125
               IPTI2 = IPT(IT2T3)
C                                  CHECKS IF THE EXCHANGE IS
C                                    NECESSARY.
C
  125          IF (IQHSD(XD,YD,IPTI1,IPTI2,IPL1,IPL2).EQ.0) GO TO 135
C                                  MODIFIES THE IPT ARRAY WHEN
C                                    NECESSARY.
               IPT(IT1T3-2) = IPTI1
               IPT(IT1T3-1) = IPTI2
               IPT(IT1T3) = IPL1
               IPT(IT2T3-2) = IPTI2
               IPT(IT2T3-1) = IPTI1
               IPT(IT2T3) = IPL2
C                                  SETS NEW FLAGS.
               JWL = JWL+8
               IWL(JWL-7) = IPL1
               IWL(JWL-6) = IPTI1
               IWL(JWL-5) = IPTI1
               IWL(JWL-4) = IPL2
               IWL(JWL-3) = IPL2
               IWL(JWL-2) = IPTI2
               IWL(JWL-1) = IPTI2
               IWL(JWL) = IPL1
               DO 130 JLT3=3,NLT3,3
                  IPLJ1 = IPL(JLT3-2)
                  IPLJ2 = IPL(JLT3-1)
                  IF ((IPLJ1.EQ.IPL1.AND.IPLJ2.EQ.IPTI2).OR.(IPLJ2.EQ.IP
     1            L1.AND.IPLJ1.EQ.IPTI2)) IPL(JLT3)
     2            = ITF(1)
                  IF ((IPLJ1.EQ.IPL2.AND.IPLJ2.EQ.IPTI1).OR.(IPLJ2.EQ.IP
     1            L2.AND.IPLJ1.EQ.IPTI1)) IPL(JLT3)
     2            = ITF(2)
  130          CONTINUE
  135       CONTINUE
            NLFC = NLF
            NLF = JWL/2
            IF (NLF.EQ.NLFC) GO TO 150
C                                  RESETS THE IWL ARRAY FOR THE
C                                    NEXT ROUND.
            JWL1MN = 2*NLFC+1
            NLFT2 = NLF*2
            DO 140 JWL1=JWL1MN,NLFT2
               JWL = JWL1+1-JWL1MN
               IWL(JWL) = IWL(JWL1)
  140       CONTINUE
            NLF = JWL/2
  145    CONTINUE
  150 CONTINUE
C                                  REARRANGES THE IPT ARRAY SO THAT THE
C                                    VERTEXES OF EACH TRIANGLE ARE
C                                    LISTED COUNTER-CLOCKWISE.
      DO 155 ITT3=3,NTT3,3
         IP1 = IPT(ITT3-2)
         IP2 = IPT(ITT3-1)
         IP3 = IPT(ITT3)
         IF (VPDT(XD(IP1),YD(IP1),XD(IP2),YD(IP2),XD(IP3),YD(IP3)).GE.0.
     1D0   0) GO TO 155
         IPT(ITT3-2) = IP2
         IPT(ITT3-1) = IP1
  155 CONTINUE
      NT = NT0
      NL = NL0
      RETURN
C                                  ERROR EXIT
  160 IER = 131
      RETURN
  165 IER = 130
      RETURN
      END
