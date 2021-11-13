#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for SHARMEM_read modul
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# source
BIN = read_sharmem
HDR = read.h             ../sharmem_config.h ../debug.h
SRC = read.c main_read.c ../sharmem_config.c ../debug.c
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

main_read.o:                 main_read.c read.h
read.o:                           read.c read.h ../sharmem_config.h ../debug.h
../sharmem_config.o: ../sharmem_config.c        ../sharmem_config.h
../debug.o:                   ../debug.c                            ../debug.h
