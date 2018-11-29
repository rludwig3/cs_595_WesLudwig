include $(PETSC_DIR)/lib/petsc/conf/variables
include $(PETSC_DIR)/lib/petsc/conf/rules
include $(PETSC_DIR)/lib/petsc/conf/test
include $(SALSA_MODULES_DIR)/Make.inc


CFLAGS = $(PETSC_INCLUDE) $(COPTFLAGS) -I$(SALSA_MODULES_DIR) $(HAVE_NMD_DEFINE) -I$(LIBNMD_INCLUDE_DIR)

anamod_test: anamod_test.o
	$(CLINKER) -o anamod_test anamod_test.o ${PETSC_KSP_LIB}
	-L$(SALSA_MODULES_LIB_DIR) -lsalsamodules -lothermodules \
	$(PETSC_LIB)
