#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for FIFO_read modul
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# source
BIN = read_fifo
HDR = read.h ../fifo_config.h
SRC = read.c main_read.c ../fifo_config.o
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

main_read.o:           main_read.c read.h
read.o:                     read.c read.h ../fifo_config.h
../fifo_config.o: ../fifo_config.c        ../fifo_config.h
