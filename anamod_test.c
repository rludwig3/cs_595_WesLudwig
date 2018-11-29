#include <stdlib.h>

/*#include </home/class/fall-18/cs595/rludwig3/soft/VictorEijkhout-salsa-025c63ed4210/anamod/anamod.h>*/
/*#include </home/class/fall-18/cs595/rludwig3/soft/VictorEijkhout-salsa-025c63ed4210/anamod/anamodsalsamodules.h>*/
#include <petsc.h>
#include <petscksp.h>
/*#include "/home/class/fall-18/cs595/rludwig3/soft/petsc/include/petsc.h"*/
/*#include "/home/class/fall-18/cs595/rludwig3/soft/petsc/include/petscksp.h"*/

int main(int argc,char **argv)
{
  /* Declare vars here */
  PetscErrorCode ierr;
  ierr = PetscInitialize(&argc,&argv,(char*)0,help);if (ierr) return ierr;
  //ierr = AnaModInitialize(); CHKERRQ(ierr);
  //ierr = AnaModFinalize(); CHKERRQ(ierr);
  PetscFinalize();
  return 0;
}
