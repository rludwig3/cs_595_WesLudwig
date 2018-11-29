static char help[] = "Solves a tridagonal linear system with parallel KSP.\n\n";

#include <petscksp.h>

int main(int argc, char **args)
{
  Vec 		x, b, u;
  Mat		A;
  KSP		ksp;
//  PC		pc;
  PetscReal	norm;
  PetscInt	Ii, Istart, Iend,  n = 100, col[3], its;
  PetscScalar	neg_one = -1.0, one = 1.0, value[3];
  PetscBool	nonzeroguess = PETSC_FALSE;

  PetscInitialize(&argc,&args,(char*)0,help);
  PetscOptionsGetBool(NULL,NULL,"-nonzeroguess",&nonzeroguess,NULL);

//Create Vectors

  VecCreate(PETSC_COMM_WORLD,&x);
  PetscObjectSetName((PetscObject) x, "Solution");
  VecSetSizes(x,PETSC_DECIDE,n);
  VecSetFromOptions(x);
  VecDuplicate(x,&b);
  VecDuplicate(x,&u);

//Create Matrix A, divide among processes, find idx of start and end of each local matrix

  MatCreate(PETSC_COMM_WORLD, &A);
  MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, n);
  MatSetFromOptions(A);
//  PetscSplitOwnership(MPI_COMM_WORLD,PETSC_DECIDE, n);
//  MatSetup(A);
  MatMPIAIJSetPreallocation(A,3,NULL,3,NULL); 
  MatGetOwnershipRange(A, &Istart, &Iend);

//Set matrix values within each submatrix, check if Istart == 0, if Iend == n-1 to use modified for loops
//if(Istart == 0): loop skips row 0, if(Iend == n): loop stops at n-2

  value[0] = -1.0; value[1] = 2.0; value[2] = -1.0;
  if(Istart == 0){
	for (Ii = Istart + 1; Ii < Iend; Ii++) {
		col[0] = Ii - 1; col[1] = Ii; col[2] = Ii + 1;
		MatSetValues(A,1,&Ii,3,col,value,INSERT_VALUES);
	}
  }

  if(Iend == n ){
	for (Ii = Istart; Ii < Iend - 1; Ii++) {
		col[0] = Ii - 1; col[1] = Ii; col[2] = Ii + 1;
		MatSetValues(A,1,&Ii,3,col,value,INSERT_VALUES);
	}
  }

//Regular iterator for setting interior tridiagonal values

  for (Ii = Istart; Ii < Iend; Ii++) {
    col[0] = Ii-1; col[1] = Ii; col[2] = Ii+1;
    MatSetValues(A,1,&Ii,3,col,value,INSERT_VALUES);
  }

//Cleanup rows n-1 and 0

  Ii = n - 1; col[0] = n - 2; col[1] = n - 1;
  MatSetValues(A,1,&Ii,2,col,value,INSERT_VALUES);
  Ii = 0; col[0] = 0; col[1] = 1; value[0] = 2.0; value[1] = -1.0;
  MatSetValues(A,1,&Ii,2,col,value,INSERT_VALUES);
 
//Assemble Matrix A

  MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);

//Set u = [1,1,...,1]n and find RHS vector b

  VecSet(u,one);
  MatMult(A,u,b);

//Create linear solver context and set operators

  KSPCreate(PETSC_COMM_WORLD,&ksp);
  KSPSetOperators(ksp,A,A);
  KSPSetFromOptions(ksp);
  
//Nonzeroguess to avoid premature convergence

  if (nonzeroguess) {
    PetscScalar p = .5;
    VecSet(x,p);
    KSPSetInitialGuessNonzero(ksp,PETSC_TRUE);
  }

//Solve the linear system and view output

  KSPSolve(ksp,b,x);
  KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);

//Check error

  VecAXPY(x,neg_one,u);
  VecNorm(x,NORM_2,&norm);
  KSPGetIterationNumber(ksp,&its);
  PetscPrintf(PETSC_COMM_WORLD,"Norm of error: %g, Iterations %D\n",(double)norm,its);

//Destroy all constructs to free memory

  VecDestroy(&x); VecDestroy(&u);
  VecDestroy(&b); MatDestroy(&A);
  KSPDestroy(&ksp);

//Finalize petsc to release remaining memory

  PetscFinalize();
  return 0;
}
