! Optimization Example
! v1.0 - 2012/02/01 - carlos@tacc.utexas.edu

PROGRAM main
  IMPLICIT NONE
  INTEGER, PARAMETER :: MAX_SIZE = 50000
  INTEGER :: i, k, rate, tStart, tSetup, tStop
  REAL    :: setup, kernel
  REAL    :: x(1:MAX_SIZE)
  REAL    :: y(1:MAX_SIZE)
  

  setup  = 0.0
  kernel = 0.0
  CALL SYSTEM_CLOCK( COUNT_RATE = rate )
  ! External loop is only used to make things take longer
  ! DO NOT CHANGE THIS EXTERNAL LOOP
  DO k = 1, 1000

    ! Initialize the arrays
    CALL SYSTEM_CLOCK( COUNT = tStart )
    DO i = 1, MAX_SIZE
        x(i) = 2.0*i
    END DO
    CALL SYSTEM_CLOCK( COUNT = tSetup )

    DO i = 1, MAX_SIZE
        y(i) = x(i)
    END DO

    ! KERNEL
    DO i = 2, MAX_SIZE-1
        x(i) = y(i-1)/4 + y(i)/2 + y(i+1)/4;
    END DO

    CALL SYSTEM_CLOCK( COUNT = tStop )

  setup  = setup  + REAL( ( tSetup - tStart ) ) / REAL( rate )
  kernel = kernel + REAL( ( tStop - tSetup ) ) / REAL( rate )
  END DO

  WRITE(*,*)'Two values for validation'
  WRITE(*,*)'========================='
  WRITE(*,*)'X(2) = ',x(2)
  WRITE(*,*)'X(',MAX_SIZE-1,') = ',x(MAX_SIZE-1)
  WRITE(*,*)
  WRITE(*,*)'Execution Times'
  WRITE(*,*)'==============='
  WRITE(*,*)'Setup Time  = ',setup
  WRITE(*,*)'Kernel Time = ',kernel
  WRITE(*,*)

 END PROGRAM

