#!/usr/bin/env python
import sys
import os

def print_usage():
    print 'Usage: ./setup.py prepare READY_TO_USE_GENODE_PATH_CAN_BE_RELATIVE'
    print 'then: ./setup.py run'
def run_config(name):
    print "Run module"
    command = "make run/"+name
    os.system("(cd build.foc_x86_32;"+command+")")
def prepare_build():
    print "Setup build"
    command = sys.argv[2]+"/tool/create_builddir foc_x86_32 BUILD_DIR=build.foc_x86_32"
    print "Execute: ", command
    os.system(command)
    f = open('build.foc_x86_32/etc/build.conf', 'a')
    f.write("\n#Auto generated for ease of use\n")
    f.write("MAKE += -j4\n")
    f.write("\nREPOSITORIES += $(GENODE_DIR)/repos/libports\n")
    f.write("\nREPOSITORIES += $(GENODE_DIR)/repos/ports\n")
    f.write("\nREPOSITORIES += $(GENODE_DIR)/repos/dde_linux\n")
    f.write("\nREPOSITORIES += "+os.path.dirname(os.path.realpath(__file__))+"\n")
    f.close()
# TODO need to download and prepare complete build incase no genode is present on the system

# Decision graph for console input
if len(sys.argv) == 2:
    if sys.argv[1] == "run":
        run_config("cr")
    else:
        print_usage()
elif len(sys.argv) == 3:
    if sys.argv[1] == "prepare":
        prepare_build()
    elif sys.argv[1] == "run":
        run_config(sys.argv[2])
    else:
        print_usage()
else:
    print_usage()
