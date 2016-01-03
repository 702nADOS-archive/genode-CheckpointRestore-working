LIBS   = base
SRC_CC = 	manager.cc \
					child_process.cc \
					child_process_policy.cc \
					child_destructor.cc \
					services/rm_service.cc \
					services/rm_session_component.cc \
					rm_registry.cc
INC_DIR  += $(REP_DIR)/include/launcher $(REP_DIR)/src/launcher
CC_OPT += -fpermissive -w

vpath % $(REP_DIR)/src/launcher
