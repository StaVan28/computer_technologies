#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for FIFO_write modul
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# source
BIN = write_fifo
HDR = write.h ../fifo_config.h
SRC = write.c main_write.c ../fifo_config.c
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

main_write.o:         main_write.c write.h 
write.o:                   write.c write.h ../fifo_config.h
../fifo_config.o: ../fifo_config.c         ../fifo_config.h


