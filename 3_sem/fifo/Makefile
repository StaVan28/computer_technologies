#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for FIFO_project
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

# project name
PROJECT = fifo
MODULS  = write read

# choose release or debug mode
export LAUNCH_MD = debug

export Q = @  
export E = $(Q) echo

# bins
BIN_WR = write
BIN_RD = read

# path for directories
RD_DIR = FIFO_read
WR_DIR = FIFO_write

# compiler settings
export CXX      = gcc 
export WARNINGS = -Wall -Wextra
export CXXFLAGS = 

export DBG_MD = -g3 -O0
export RLS_MD = -g0 -O2


ifeq ($(LAUNCH_MODE),debug)
	CXXFLAGS += $(DBG_MD)
else
	CXXFLAGS += $(RLS_MD)
endif

#--------------------------------------------------------------------------------------
# RULES
#--------------------------------------------------------------------------------------

.PHONY: default debug release

#
## launch 
default: $(BIN_WR) $(BIN_RD)

$(BIN_WR):
	$(E) "    MAKE RECUR(compile):" $(BIN_WR)
	$(Q) $(MAKE) -C $(WR_DIR)

$(BIN_RD):
	$(E) "    MAKE RECUR(compile):" $(BIN_RD)
	$(Q) $(MAKE) -C $(RD_DIR)

#
## debugs 
debug: write_dbg read_dbg

write_dbg: $(BIN_WR)
	$(E) "    LAUNCH(DBG): " $(BIN_WR)
	$(Q) strace $(BIN_WR)	

read_dbg:  $(BIN_RD)
	$(E) "    LAUNCH(DBG): " $(BIN_RD)
	$(Q) strace $(BIN_RD)

#
## additional features
clean:
	$(E) "  CLEAN"
	$(Q) rm $(RD_DIR)/*.o       $(WR_DIR)/*.o       \
	        $(RD_DIR)/$(BID_RD) $(WR_DIR)/$(BID_WR)

rebuild: clean default

# gdb
gdb: write_gdb read_gdb

write_gdb: $(BIN_WR)
	$(E) "    LAUNCH(DBG): " $(BIN_WR)
	$(Q) gdb $(BIN_WR)	

read_gdb:  $(BIN_RD)
	$(E) "    LAUNCH(DBG): " $(BIN_RD)
	$(Q) gdb $(BIN_RD) 

log:
	$(Q) git log --oneline --all --graph
