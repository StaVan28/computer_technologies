#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for integral
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# project name
PROJECT = integral

Q = @
E = $(Q) echo

# path for directories
TEST_DIR = Dev

# source
BIN = $(PROJECT).out
SRC = $(addsuffix .c, $(PROJECT)) main.c integral.c pthread.c
OBJ = $(SRC:%.c=%.o)
HDR = $(OBJ:%.o=%.h) settings.h debug.h
LIB = $(addprefix lib, $(addsuffix .a, $(PROJECT)))

DBG_MD = -g3 -O0
RLS_MD = -g0 -O2

# compiler settings
CC         = gcc
WARNINGS  += -Wall -Wextra
  CFLAGS   = -lm -pthread -fPIC $(DBG_MD) -DDEBUG
 LDFLAGS  += -lm -pthread
LIBFLAGS  +=
INFO_LCOV  = $(addsuffix .info, $(BIN))

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

.PHONY: all test clean rebuild log

#
## launch 
all: $(BIN)

# link
$(BIN): $(OBJ)
	$(E) "    LINK:        " $@
	$(Q) $(CC) $(LDFLAGS) $(WARNINGS) $^ -o $@ $(LDFLAGS)

# compile
%.o: %.c
	$(E) "    C:           " $@
	$(Q) ${CC} $(WARNINGS) ${CFLAGS} -c $< -o $@

lib: $(OBJ)
	$(E) "    LIBRARY:     " $(LIB)
	$(Q) ar $(LIBFLAGS) $(LIB) $(OBJ)

launch: $(BIN)
	$(E) "    LAUNCH:      " $(BIN)
	$(Q) ./$(BIN)

valgrind: $(BIN)
	$(E) "    LAUNCH(valg):" $(BIN)
	$(Q) valgrind ./$(BIN)

#
## additional features
clean:
	$(E) "  CLEAN"
	$(Q) rm -rf *.o *.d *.out

rebuild: clean all

log:
	$(Q) git log --oneline --all --graph

-include $(OBJ:%.o=%.d)