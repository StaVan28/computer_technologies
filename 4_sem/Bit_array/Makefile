#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for bit_array
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# project name
export PROJECT = bit_array

export Q = @
export E = $(Q) echo

# path for directories
TEST_DIR = Dev

# source
export SRC = $(addsuffix .c, $(PROJECT))
export OBJ = $(SRC:%.c=%.o)
export HDR = $(OBJ:%.o=%.h)
       LIB = $(addprefix lib, $(addsuffix .a, $(PROJECT)))

# compiler settings
export CC        = gcc
export WARNINGS += -Wall -Wextra
export   CFLAGS += -fPIC -MMD -fprofile-arcs -ftest-coverage
export  LDFLAGS += -lgcov --coverage
       LIBFLAGS += -csru

export DBG_MD = -g3 -O0
export RLS_MD = -g0 -O2

ifdef DEBUG
	CFLAGS += $(DBG_MD)
else
	CFLAGS += $(RLS_MD)
endif

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

.PHONY: all test clean rebuild log

#
## launch 
all: test

test: $(OBJ)
	$(Q) $(MAKE) -C $(TEST_DIR)

# compile
%.o: %.c
	$(E) "    C:           " $@
	$(Q) ${CC} $(WARNINGS) ${CFLAGS} -c $< -o $@

lib: $(OBJ)
	$(E) "    LIBRARY:     " $(LIB)
	$(Q) ar $(LIBFLAGS) $(LIB) $(OBJ)

#
## additional features
clean:
	$(E) "  CLEAN"
	$(Q) rm             *.o             *.d             *.gcda             *.gcno \
	        $(TEST_DIR)/*.o $(TEST_DIR)/*.d $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcno \
	        $(TEST_DIR)/*.info

rebuild: clean all

log:
	$(Q) git log --oneline --all --graph

-include $(OBJ:%.o=%.d)