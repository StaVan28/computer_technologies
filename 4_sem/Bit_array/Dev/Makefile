#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for bit_array
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# source
BIN   = ../$(PROJECT)
SRC  +=    $(addsuffix _test.c, $(PROJECT))
OBJ   = $(SRC:%.c=%.o)
HDR   = $(OBJ:%.o=%.h)

# find ../files 
vpath %.c ../
vpath %.o ../
vpath %.h ../

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

.PHONY: all test clean rebuild log

#
## launch 
all: $(BIN)

gcov: valgrind
	$(E) "    GCOV: " $(BIN)
	$(Q) gcov $(SRC) -o ..

lcov: valgrind
	$(E) "    LCOV: " $(BIN)
	$(Q) lcov    -t "$(PROJECT)" -o             $(INFO_LCOV) -c -d .. --rc lcov_branch_coverage=1
	$(Q) genhtml                 -o report_lcov $(INFO_LCOV)          --rc lcov_branch_coverage=1
	$(Q) x-www-browser ./report_lcov/Bit_array/index.html

launch: $(BIN)
	$(E) "    LAUNCH:      " $(BIN)
	$(Q) ./$(BIN)

valgrind: $(BIN)
	$(E) "    LAUNCH(valg):" $(BIN)
	$(Q) valgrind ./$(BIN)

#
## linking
$(BIN): $(OBJ)
	$(E) "    LINK:        " $@
	$(Q) $(CC) $(LDFLAGS) $(WARNINGS) $^ -o $@ 

# compile
%.o: %.c
	$(E) "    C:           " $@
	$(Q) ${CC} ${CFLAGS} $(addprefix -I,../) -c $< -o $@

#
## additional features
clean:
	$(E) "  CLEAN"
	$(Q) rm -rf    *.o    *.d    *.info    *.gcno report_lcov \
	            ../*.o ../*.d ../*.info ../*.gcno $(BIN)

rebuild: clean default

log:
	$(E) "    GIT LOG "
	$(Q) git log --oneline --all --graph

-include $(OBJ:%.o=%.d)