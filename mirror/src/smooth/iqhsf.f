C   ROUTINE NAME   - IQHSF
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
      SUBROUTINE IQHSF  (XD,YD,ZD,NT,IPT,NL,IPL,PDD,ITI,XII,YII,ZII)
C                                  SPECIFICATIONS FOR ARGUMENTS
      INTEGER            NT,NL,ITI,IPT(1),IPL(1)
      DOUBLE PRECISION   XD(1),YD(1),ZD(1),PDD(1),XII,YII,ZII
C                                  SPECIFICATIONS FOR LOCAL VARIABLES
      INTEGER            IDP,IL1,IL2,IT0,I,JIPL,JIPT,JPDD,JPD,KPD,NTL
      INTEGER            ITPV
      DOUBLE PRECISION   X0,Y0,AP,BP,CP,DP,P00,P10,P20,P30,P40,P50,P5,
     1                   P01,P11,P21,P31,P41,P02,P12,P22,P32,P03,P13,
     2                   P23,P04,P14,P05
      DOUBLE PRECISION   AA,AB,ACT2,AC,ADBC,AD,A,BB,BC,BDT2,B,CC,CD,
     1                   CSUV,C,DD,DLT,DX,DY,D,G1,G2,H1,H2,H3,LU,
     2                   LV,X(3),P0,P1,P2,P3,P4,PD(15),THSV,THUS,THUV,
     3                   THXU,U,V,Y(3),Z0,ZUU(3),ZUV(3),ZU(3),ZVV(3),
     4                   ZV(3),Z(3)
      double precision   ckdat2
C ckdat2 is the IRAF name for ckdatan2()
      COMMON /IBCDPT/    X0,Y0,AP,BP,CP,DP,P00,P10,P20,P30,P40,P50,
     1                   P01,P11,P21,P31,P41,P02,P12,P22,P32,P03,P13,
     2                   P23,P04,P14,P05,ITPV
      EQUIVALENCE        (P5,P50)
C                                  FIRST EXECUTABLE STATEMENT
C                                  PRELIMINARY PROCESSING
      IT0 = ITI
      NTL = NT+NL
      IF (IT0.LE.NTL) GO TO 5
      IL1 = IT0/NTL
      IL2 = IT0-IL1*NTL
      IF (IL1.EQ.IL2) GO TO 30
      GO TO 55
C                                  CALCULATION OF ZII BY INTERPOLATION.
C                                    CHECKS IF THE NECESSARY
C                                    COEFFICIENTS HAVE BEEN CALCULATED.
    5 IF (IT0.EQ.ITPV) GO TO 25
C                                  LOADS COORDINATE AND PARTIAL
C                                    DERIVATIVE VALUES AT THE VERTEXES.
      JIPT = 3*(IT0-1)
      JPD = 0
      DO 15 I=1,3
         JIPT = JIPT+1
         IDP = IPT(JIPT)
         X(I) = XD(IDP)
         Y(I) = YD(IDP)
         Z(I) = ZD(IDP)
         JPDD = 5*(IDP-1)
         DO 10 KPD=1,5
            JPD = JPD+1
            JPDD = JPDD+1
            PD(JPD) = PDD(JPDD)
   10    CONTINUE
   15 CONTINUE
C                                  DETERMINES THE COEFFICIENTS FOR THE
C                                    COORDINATE SYSTEM TRANSFORMATION
C                                    FROM THE X-Y SYSTEM TO THE U-V
C                                    SYSTEM AND VICE VERSA.
      X0 = X(1)
      Y0 = Y(1)
      A = X(2)-X0
      B = X(3)-X0
      C = Y(2)-Y0
      D = Y(3)-Y0
      AD = A*D
      BC = B*C
      DLT = AD-BC
      AP = D/DLT
      BP = -B/DLT
      CP = -C/DLT
      DP = A/DLT
C                                  CONVERTS THE PARTIAL DERIVATIVES AT
C                                    THE VERTEXES OF THE TRIANGLE FOR
C                                    THE U-V COORDINATE SYSTEM.
      AA = A*A
      ACT2 = 2.0D0*A*C
      CC = C*C
      AB = A*B
      ADBC = AD+BC
      CD = C*D
      BB = B*B
      BDT2 = 2.0D0*B*D
      DD = D*D
      DO 20 I=1,3
         JPD = 5*I
         ZU(I) = A*PD(JPD-4)+C*PD(JPD-3)
         ZV(I) = B*PD(JPD-4)+D*PD(JPD-3)
         ZUU(I) = AA*PD(JPD-2)+ACT2*PD(JPD-1)+CC*PD(JPD)
         ZUV(I) = AB*PD(JPD-2)+ADBC*PD(JPD-1)+CD*PD(JPD)
         ZVV(I) = BB*PD(JPD-2)+BDT2*PD(JPD-1)+DD*PD(JPD)
   20 CONTINUE
C                                  CALCULATES THE COEFFICIENTS OF THE
C                                    POLYNOMIAL.
      P00 = Z(1)
      P10 = ZU(1)
      P01 = ZV(1)
      P20 = 0.5D0*ZUU(1)
      P11 = ZUV(1)
      P02 = 0.5D0*ZVV(1)
      H1 = Z(2)-P00-P10-P20
      H2 = ZU(2)-P10-ZUU(1)
      H3 = ZUU(2)-ZUU(1)
      P30 = 10.0D0*H1-4.0D0*H2+0.5D0*H3
      P40 = -15.0D0*H1+7.0D0*H2-H3
      P50 = 6.0D0*H1-3.0D0*H2+0.5D0*H3
      H1 = Z(3)-P00-P01-P02
      H2 = ZV(3)-P01-ZVV(1)
      H3 = ZVV(3)-ZVV(1)
      P03 = 10.0D0*H1-4.0D0*H2+0.5D0*H3
      P04 = -15.0D0*H1+7.0D0*H2-H3
      P05 = 6.0D0*H1-3.0D0*H2+0.5D0*H3
      LU = DSQRT(AA+CC)
      LV = DSQRT(BB+DD)
      THXU = ckdat2(C,A)
      THUV = ckdat2(D,B)-THXU
      CSUV = DCOS(THUV)
      P41 = 5.0D0*LV*CSUV/LU*P50
      P14 = 5.0D0*LU*CSUV/LV*P05
      H1 = ZV(2)-P01-P11-P41
      H2 = ZUV(2)-P11-4.0D0*P41
      P21 = 3.0D0*H1-H2
      P31 = -2.0D0*H1+H2
      H1 = ZU(3)-P10-P11-P14
      H2 = ZUV(3)-P11-4.0D0*P14
      P12 = 3.0D0*H1-H2
      P13 = -2.0D0*H1+H2
      THUS = ckdat2(D-C,B-A)-THXU
      THSV = THUV-THUS
      AA = DSIN(THSV)/LU
      BB = -DCOS(THSV)/LU
      CC = DSIN(THUS)/LV
      DD = DCOS(THUS)/LV
      AC = AA*CC
      AD = AA*DD
      BC = BB*CC
      G1 = AA*AC*(3.0D0*BC+2.0D0*AD)
      G2 = CC*AC*(3.0D0*AD+2.0D0*BC)
      H1 = -AA*AA*AA*(5.0D0*AA*BB*P50+(4.0D0*BC+AD)*P41)-CC*CC*CC*(5.0D0
     1*CC
     +*DD*P05+(4.0D0*AD+BC)*P14)
      H2 = 0.5D0*ZVV(2)-P02-P12
      H3 = 0.5D0*ZUU(3)-P20-P21
      P22 = (G1*H2+G2*H3-H1)/(G1+G2)
      P32 = H2-P22
      P23 = H3-P22
      ITPV = IT0
C                                  CONVERTS XII AND YII TO U-V SYSTEM.
   25 DX = XII-X0
      DY = YII-Y0
      U = AP*DX+BP*DY
      V = CP*DX+DP*DY
C                                  EVALUATES THE POLYNOMIAL.
      P0 = P00+V*(P01+V*(P02+V*(P03+V*(P04+V*P05))))
      P1 = P10+V*(P11+V*(P12+V*(P13+V*P14)))
      P2 = P20+V*(P21+V*(P22+V*P23))
      P3 = P30+V*(P31+V*P32)
      P4 = P40+V*P41
      ZII = P0+U*(P1+U*(P2+U*(P3+U*(P4+U*P5))))
      RETURN
C                                  CALCULATION OF ZII BY EXTRAPOLATION
C                                    IN THE RECTANGLE. CHECKS IF THE
C                                    NECESSARY COEFFICIENTS HAVE BEEN
C                                    CALCULATED.
   30 IF (IT0.EQ.ITPV) GO TO 50
C                                  LOADS COORDINATE AND PARTIAL
C                                    DERIVATIVE VALUES AT THE END
C                                    POINTS OF THE BORDER LINE SEGMENT.
      JIPL = 3*(IL1-1)
      JPD = 0
      DO 40 I=1,2
         JIPL = JIPL+1
         IDP = IPL(JIPL)
         X(I) = XD(IDP)
         Y(I) = YD(IDP)
         Z(I) = ZD(IDP)
         JPDD = 5*(IDP-1)
         DO 35 KPD=1,5
            JPD = JPD+1
            JPDD = JPDD+1
            PD(JPD) = PDD(JPDD)
   35    CONTINUE
   40 CONTINUE
C                                  DETERMINES THE COEFFICIENTS FOR THE
C                                    COORDINATE SYSTEM TRANSFORMATION
C                                    FROM THE X-Y SYSTEM TO THE U-V
C                                    SYSTEM AND VICE VERSA.
      X0 = X(1)
      Y0 = Y(1)
      A = Y(2)-Y(1)
      B = X(2)-X(1)
      C = -B
      D = A
      AD = A*D
      BC = B*C
      DLT = AD-BC
      AP = D/DLT
      BP = -B/DLT
      CP = -BP
      DP = AP
C                                  CONVERTS THE PARTIAL DERIVATIVES AT
C                                    THE END POINTS OF THE BORDER LINE
C                                    SEGMENT FOR THE U-V COORDINATE
C                                    SYSTEM.
      AA = A*A
      ACT2 = 2.0D0*A*C
      CC = C*C
      AB = A*B
      ADBC = AD+BC
      CD = C*D
      BB = B*B
      BDT2 = 2.0D0*B*D
      DD = D*D
      DO 45 I=1,2
         JPD = 5*I
         ZU(I) = A*PD(JPD-4)+C*PD(JPD-3)
         ZV(I) = B*PD(JPD-4)+D*PD(JPD-3)
         ZUU(I) = AA*PD(JPD-2)+ACT2*PD(JPD-1)+CC*PD(JPD)
         ZUV(I) = AB*PD(JPD-2)+ADBC*PD(JPD-1)+CD*PD(JPD)
         ZVV(I) = BB*PD(JPD-2)+BDT2*PD(JPD-1)+DD*PD(JPD)
   45 CONTINUE
C                                  CALCULATES THE COEFFICIENTS OF THE
C                                    POLYNOMIAL.
      P00 = Z(1)
      P10 = ZU(1)
      P01 = ZV(1)
      P20 = 0.5D0*ZUU(1)
      P11 = ZUV(1)
      P02 = 0.5D0*ZVV(1)
      H1 = Z(2)-P00-P01-P02
      H2 = ZV(2)-P01-ZVV(1)
      H3 = ZVV(2)-ZVV(1)
      P03 = 10.0D0*H1-4.0D0*H2+0.5D0*H3
      P04 = -15.0D0*H1+7.0D0*H2-H3
      P05 = 6.0D0*H1-3.0D0*H2+0.5D0*H3
      H1 = ZU(2)-P10-P11
      H2 = ZUV(2)-P11
      P12 = 3.0D0*H1-H2
      P13 = -2.0D0*H1+H2
      P21 = 0.0D0
      P23 = -ZUU(2)+ZUU(1)
      P22 = -1.5D0*P23
      ITPV = IT0
C                                  CONVERTS XII AND YII TO U-V SYSTEM.
   50 DX = XII-X0
      DY = YII-Y0
      U = AP*DX+BP*DY
      V = CP*DX+DP*DY
C                                  EVALUATES THE POLYNOMIAL.
      P0 = P00+V*(P01+V*(P02+V*(P03+V*(P04+V*P05))))
      P1 = P10+V*(P11+V*(P12+V*P13))
      P2 = P20+V*(P21+V*(P22+V*P23))
      ZII = P0+U*(P1+U*P2)
      RETURN
C                                  CALCULATION OF ZII BY EXTRAPOLATION
C                                    IN THE TRIANGLE. CHECKS IF THE
C                                    NECESSARY COEFFICIENTS HAVE BEEN
C                                    CALCULATED.
   55 IF (IT0.EQ.ITPV) GO TO 65
C                                  LOADS COORDINATE AND PARTIAL
C                                    DERIVATIVE VALUES AT THE VERTEX OF
C                                    THE TRIANGLE.
      JIPL = 3*IL2-2
      IDP = IPL(JIPL)
      X0 = XD(IDP)
      Y0 = YD(IDP)
      Z0 = ZD(IDP)
      JPDD = 5*(IDP-1)
      DO 60 KPD=1,5
         JPDD = JPDD+1
         PD(KPD) = PDD(JPDD)
   60 CONTINUE
C                                  CALCULATES THE COEFFICIENTS OF THE
C                                    POLYNOMIAL.
      P00 = Z0
      P10 = PD(1)
      P01 = PD(2)
      P20 = 0.5D0*PD(3)
      P11 = PD(4)
      P02 = 0.5D0*PD(5)
      ITPV = IT0
C                                  CONVERTS XII AND YII TO U-V SYSTEM.
   65 U = XII-X0
      V = YII-Y0
C                                  EVALUATES THE POLYNOMIAL.
      P0 = P00+V*(P01+V*P02)
      P1 = P10+V*P11
      ZII = P0+U*(P1+U*P20)
      RETURN
      END
