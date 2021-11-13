#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for SHARMEM_write modul
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# source
BIN = write_sharmem
HDR = write.h              ../sharmem_config.h ../debug.h
SRC = write.c main_write.c ../sharmem_config.c ../debug.c
OBJ = $(SRC:%.c=%.o)

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

#
## linking
$(BIN): $(OBJ)
	$(E) "    LINK:        " $@
	$(Q) $(CXX) $(WARNINGS) $^ -o $@ 

#
## compiling
%.o: %.c
	$(E) "    C:           " $@
	$(Q) ${CXX} -c ${CXXFLAGS} $(addprefix -I,../) $< -o $@

../%.o: ../%.c
	$(E) "    C:           " $@
	$(Q) ${CXX} -c ${CXXFLAGS} $(addprefix -I,../) $< -o $@

main_write.o:               main_write.c write.h 
write.o:                         write.c write.h ../sharmem_config.h ../debug.h
../sharmem_config.o: ../sharmem_config.c         ../sharmem_config.h
../debug.o:                   ../debug.c                             ../debug.h



