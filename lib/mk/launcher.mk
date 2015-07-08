LIBS   = base
SRC_CC = 	launcher.cc \
					child_process.cc \
					child_process_policy.cc
INC_DIR  += $(REP_DIR)/include/launcher
CC_OPT += -fpermissive -w

vpath % $(REP_DIR)/src/launcher
