LIBS   = base
SRC_CC = 	launcher.cc \
					child_process.cc \
					child_process_policy.cc \
					child_destructor.cc
INC_DIR  += $(REP_DIR)/include/launcher $(REP_DIR)/src/launcher
CC_OPT += -fpermissive -w

vpath % $(REP_DIR)/src/launcher
