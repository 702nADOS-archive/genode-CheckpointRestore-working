TARGET = launcher_manager
LIBS   = base cr_launcher #libc
SRC_CC = main.cc root.cc session_component.cc
INC_DIR  += $(REP_DIR)/include
CC_OPT += -fpermissive -w
