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
BIN   = $(addsuffix _test, $(PROJECT))
SRC  += $(addsuffix .c,    $(BIN))
OBJ   = $(SRC:%.c=%.o)
HDR   = $(OBJ:%.o=%.h)
INFO  = $(addsuffix .info, $(BIN))

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
all: lcov

gcov: launch
	$(E) "    GCOV: " $(BIN)
	$(Q) gcov $(SRC) -o ..

lcov: launch
	$(E) "    LCOV: " $(BIN)
	$(Q) lcov -t "$(BIN)" -o    $(INFO) -c -d .. --rc lcov_branch_coverage=1
	$(Q) genhtml -o report_lcov $(INFO)          --rc lcov_branch_coverage=1
	$(Q) x-www-browser ./report_lcov/Bit_array/index.html

launch: $(BIN)
	$(E) "    LAUNCH:      " $(BIN)
	$(Q) ./$(BIN)

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
	$(Q) rm -rf    *.o    *.d s*.a report *.info *.gcno \
	            ../*.o ../*.d 
rebuild: clean default

log:
	$(E) "    GIT LOG "
	$(Q) git log --oneline --all --graph

-include $(OBJ:%.o=%.d)