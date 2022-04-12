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
       BIN = $(PROJECT)
export SRC = $(addsuffix .c, $(PROJECT))
export OBJ = $(SRC:%.c=%.o)
export HDR = $(OBJ:%.o=%.h)
       LIB = $(addprefix lib, $(addsuffix .a, $(PROJECT)))

# compiler settings
export CC         = gcc
export WARNINGS  += -Wall -Wextra
export   CFLAGS   = -fPIC -MD -fprofile-arcs -ftest-coverage -g3 -O0
export  LDFLAGS  += -lgcov --coverage
       LIBFLAGS  += -csru
export INFO_LCOV  = $(addsuffix .info, $(BIN))

export DBG_MD = -g3 -O0
export RLS_MD = -g0 -O2

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

.PHONY: all test clean rebuild log

#
## launch 
all: $(BIN)

$(BIN): $(OBJ)
	$(Q) $(MAKE) -C $(TEST_DIR)

# compile
%.o: %.c
	$(E) "    C:           " $@
	$(Q) ${CC} $(WARNINGS) ${CFLAGS} -c $< -o $@

lib: $(OBJ)
	$(E) "    LIBRARY:     " $(LIB)
	$(Q) ar $(LIBFLAGS) $(LIB) $(OBJ)

gcov: valgrind
	$(E) "    GCOV: " $(BIN)
	$(Q) gcov $(SRC) -o .

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
	$(Q) rm -rf             *.o                *.d         \
	            $(TEST_DIR)/*.o    $(TEST_DIR)/*.d         \
	                        *.gcda             *.gcno      \
	                        *.gcda             *.gcov      \
	            $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcno      \
	            $(TEST_DIR)/*.info $(TEST_DIR)/report_lcov \
	            $(TEST_DIR)/bit_array_test                 \
	            $(BIN)             $(TEST_DIR)/*.txt *.txt

rebuild: clean all

log:
	$(Q) git log --oneline --all --graph

-include $(OBJ:%.o=%.d)