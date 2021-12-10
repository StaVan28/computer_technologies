#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for select_project
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# project name
PROJECT = select

# choose release or debug mode
LAUNCH_MD = debug

Q = @  
E = $(Q) echo

# source
BIN = select
HDR =        config.h debug.h
SRC = main.c config.c
OBJ = $(SRC:%.c=%.o)

# compiler settings
CXX      = gcc 
WARNINGS = -Wall -Wextra -lm
CXXFLAGS = 

DBG_MD = -g3 -O0 
RLS_MD = -g0 -O2 


ifeq ($(LAUNCH_MODE),debug)
	CXXFLAGS += $(DBG_MD)
else
	CXXFLAGS += $(RLS_MD)
endif

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

default: $(BIN)

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

main.o:      main.c config.h debug.h
config.o:  config.c config.h

#
## debug
strace: $(BIN)
	$(E) "    LAUNCH(DBG): " $(BIN)
	$(Q) strace $(BIN) 28

#
## additional features
clean:
	$(E) "  CLEAN"
	$(Q) rm *.o $(BIN)

rebuild: clean default

#
## gdb
gdb: $(BIN)
	$(E) "    LAUNCH(DBG): " $(BIN)
	$(Q) gdb $(BIN)	28

log:
	$(Q) git log --oneline --all --graph