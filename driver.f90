program tester

real (kind=8) :: wall_start, wall_end
real (kind=8) :: cpu_start, cpu_end
real (kind=8) :: trace, mflops


integer :: problem_size, nthreads
real (kind=8) :: walltime
real (kind=8) :: cputime 
external walltime, cputime

character (len=8) :: carg1, carg2, carg3

real (kind=8), dimension(:), allocatable :: veca, vecb
real (kind=8), dimension(:,:), allocatable :: matrixa, matrixb, matrixc

external :: dot
double precision :: dot

! Start the normal processing here.  Read the starting, stop, and step values
! as well as the number of threads to use.
! modified to use command line arguments

call get_command_argument(1, carg1)
call get_command_argument(2, carg2)
call get_command_argument(3, carg3)

! Use Fortran internal files to convert command line arguments to ints

read (carg1,'(i8)') problem_size
read (carg2,'(i8)') nthreads 





allocate(veca(problem_size))
allocate(vecb(problem_size))


do i=1, problem_size
  !veca(i) = dble(problem_size)/DSQRT(dble(i))
  !vecb(i) = (2.0D0*dble(problem_size))/DSQRT(dble(i))
  veca(i) = 1.0
  vecb(i) = 1.0 / SQRT(dble(problem_size))
end do







if (carg3 == "DOT") then


wall_start = walltime()
trace = dot(nthreads, problem_size, veca, vecb)
wall_end = walltime()

mflops = (2.0D0 * dble(problem_size) ) / ( (wall_end-wall_start) * 1.0e6)

print *, "Trace: ", trace, " mflops: ", mflops




else if (carg3 == "VVM") then

allocate(matrixc(problem_size,problem_size))
matrixc = 0.0D0

wall_start = walltime()
call vvm(nthreads, problem_size, veca, vecb, matrixc)
wall_end = walltime()

mflops = dble(problem_size)**2.0D0 / ( (wall_end-wall_start) * 1.0e6 )

trace = 0.0D0
do i=1, problem_size
  trace = trace + matrixc(i,i)
end do

print *, "Trace: ", trace, " mflops: ", mflops

deallocate(matrixc)




else if ( carg3 == "MVV") then

allocate(matrixc(problem_size,problem_size))
matrixc = 0.0D0

call vvm(nthreads, problem_size, veca, vecb, matrixc)


wall_start = walltime()
call mvv(nthreads, problem_size, veca, vecb, matrixc)
wall_end = walltime()

do i=1, problem_size
  trace = trace + vecb(i)
end do

mflops = (2.0D0 * dble(problem_size)**2 ) / ( (wall_end-wall_start) * 1.0e6)

print *, "Trace: ", trace, " mflops: ", mflops

deallocate(matrixc)

else if (carg3 == "MMM") then


allocate(matrixa(problem_size, problem_size))
allocate(matrixb(problem_size, problem_size))
allocate(matrixc(problem_size,problem_size))


matrixa = 0.0
matrixb = 0.0
matrixc = 0.0D0

call vvm(nthreads, problem_size, veca, vecb, matrixa)
call vvm(nthreads, problem_size, veca, vecb, matrixb)

wall_start = walltime()

call mmm(nthreads, problem_size, matrixa, matrixb, matrixc)

wall_end = walltime()

trace = 0.0D0
do i=1, problem_size
  trace = trace + matrixc(i,i)
end do

mflops = ( 2.0D0 * dble(problem_size)**3.0D0 ) / ( (wall_end-wall_start) * 1.0e6 )

print *, "Trace: ", trace, " mflops: ", mflops

deallocate(matrixa)
deallocate(matrixb)
deallocate(matrixc)

end if


deallocate(veca)
deallocate(vecb)



end program tester
