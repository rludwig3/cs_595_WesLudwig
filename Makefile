include $(PETSC_DIR)/lib/petsc/conf/variables
include $(SALSA_MODULES_DIR)/Make.inc


CFLAGS = $(PETSC_INCLUDE) $(COPTFLAGS) -I$(SALSA_MODULES_DIR) $(HAVE_NMD_DEFINE) -I$(LIBNMD_INCLUDE_DIR) -I$(LIBXMLSC_INCLUDE_DIR)

anamod_test: anamod_test.o
	$(CLINKER) -o anamod_test anamod_test.o \
	-L$(SALSA_MODULES_LIB_DIR) -lsalsamodules -lothermodules \
	$(PETSC_LIB)
