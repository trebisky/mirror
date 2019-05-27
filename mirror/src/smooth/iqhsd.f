C   ROUTINE NAME   - IQHSD
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
      INTEGER FUNCTION IQHSD (X,Y,I1,I2,I3,I4)
C                                  SPECIFICATIONS FOR ARGUMENTS
      INTEGER            I1,I2,I3,I4
      DOUBLE PRECISION   X(1),Y(1)
C                                  SPECIFICATIONS FOR LOCAL VARIABLES
      INTEGER            IDX
      DOUBLE PRECISION   A1SQ,A2SQ,A3SQ,A4SQ,B1SQ,B2SQ,B3SQ,B4SQ,C1SQ,
     1                   C2SQ,C3SQ,C4SQ,EPSLN,S1SQ,S2SQ,S3SQ,S4SQ,TOL,
     2                   U1,U2,U3,U4,X1,X2,X3,X4,Y1,Y2,Y3,Y4
      EQUIVALENCE        (C2SQ,C1SQ),(A3SQ,B2SQ),(B3SQ,A1SQ),
     1                   (A4SQ,B1SQ),(B4SQ,A2SQ),(C4SQ,C3SQ)
C                                  MACHINE PRECISION
      DATA               TOL/2.775557562D-17/
C                                  FIRST EXECUTABLE STATEMENT
      EPSLN = TOL*100.0D0
C                                  PRELIMINARY PROCESSING
      X1 = X(I1)
      Y1 = Y(I1)
      X2 = X(I2)
      Y2 = Y(I2)
      X3 = X(I3)
      Y3 = Y(I3)
      X4 = X(I4)
      Y4 = Y(I4)
C                                  CALCULATION
      IDX = 0
      U3 = (Y2-Y3)*(X1-X3)-(X2-X3)*(Y1-Y3)
      U4 = (Y1-Y4)*(X2-X4)-(X1-X4)*(Y2-Y4)
      IF (U3*U4.LE.0.0D0) GO TO 5
      U1 = (Y3-Y1)*(X4-X1)-(X3-X1)*(Y4-Y1)
      U2 = (Y4-Y2)*(X3-X2)-(X4-X2)*(Y3-Y2)
      A1SQ = (X1-X3)**2+(Y1-Y3)**2
      B1SQ = (X4-X1)**2+(Y4-Y1)**2
      C1SQ = (X3-X4)**2+(Y3-Y4)**2
      A2SQ = (X2-X4)**2+(Y2-Y4)**2
      B2SQ = (X3-X2)**2+(Y3-Y2)**2
      C3SQ = (X2-X1)**2+(Y2-Y1)**2
      S1SQ = U1*U1/(C1SQ*DMAX1(A1SQ,B1SQ))
      S2SQ = U2*U2/(C2SQ*DMAX1(A2SQ,B2SQ))
      S3SQ = U3*U3/(C3SQ*DMAX1(A3SQ,B3SQ))
      S4SQ = U4*U4/(C4SQ*DMAX1(A4SQ,B4SQ))
      IF ((DMIN1(S3SQ,S4SQ)-DMIN1(S1SQ,S2SQ)).GT.EPSLN) IDX = 1
    5 IQHSD = IDX
      RETURN
      END
