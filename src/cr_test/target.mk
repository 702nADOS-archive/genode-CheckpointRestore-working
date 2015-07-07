TARGET  = cr_test
SRC_CC  = main.cc launcher.cc child_process.cc child_process_policy.cc
INC_DIR  += $(REP_DIR)/include
LIBS   += base
CC_OPT += -fpermissive -w
