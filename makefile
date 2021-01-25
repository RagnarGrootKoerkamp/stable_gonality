
#******************** Makefile ********************#
#
# This is an automatic makefile, made by
# Ragnar Groot Koerkamp.
# Feel free to use it yourself, and please
# let me know if you have any comments or improvements.
#
# ====== SUMMARY ======
# This makefile automatically makes an executable
# for all files in the source directory containing
# a main function, using minimal dependencies.
#
# ==== EXPLANATION ====
# It uses a ./build/ directory to store all object
# (.o) files. Also, all dynamically generated
# build dependencies are stored in this directory
# keeping your working directory completely clear.
#
# This script discriminates two kinds of .cpp files:
# 'sources' and 'targets'. The sources are files used
# by other .cpp files. Targets are files that contain
# an entry point for execution.
# (That is, `grep "^int main("` returns true.)
# It doesn't handle arguments to main (yet).
#
# First, it generates dependencies for .o files
# by executing the `g++ -MM` command, and stores
# the result in ./build/(filename).o.d.
#
# For target files, it also generates dependencies
# to link the target. This is done by parsing the
# (filename).o.d file by:
#	-	replacing *.cpp	by *.o;
#	-	replacing *.h	by *.o when the corresponding
#								cpp file exists
#	-	ignoring the *.o.d part (from this script)
#
# Furthermore, it removes the ./build/ part from the
# target and prepends ./build/ to required object file.
#
# ===== EXAMPLE =====
# The file build/target.o.d (dependencies for target.o)
# containing:
# 		build/target.o.d build/target.o: target.cpp header.h
# is transformed to build/target.d (dependencies for target)
# 		target: build/target.o
#
#**************************************************#


#*************** Compiler flags ***************#

# Capital variables are recognised by make
# -g for debugging symbols
CXX		= g++
CPPFLAGS= --std=c++11 -Wall -O3 -D _LOG -Wfatal-errors

LDFLAGS	=
LDLIBS	=

#*************** Other settings ***************#

OBJ_DIR	= ./build

# don't change these
shell	= '/bin/zsh'
rm		= rm -f
#echo	= echo -e

# description size
C1	= 15
# target chars
C2	= 25

# some colors
# 0 for thin, 1 for bold
red			=\033[1;31m
green		=\033[1;32m
yellow		=\033[1;33m
blue		=\033[1;34m
purple		=\033[1;35m
lightblue	=\033[1;36m
white		=\033[1;39m

# print command
print=printf "$(green)%-$(C1)s\t$(yellow)./%-$(C2)s $(green)<<< $(blue)%s\n$(white)"


#*************** File variables ***************#

# .cpp files: all, w/, w/o main
cpp_all		=$(shell find . -name \*.cpp)
cpp_source	=$(shell grep "^int main" $(cpp_all) -L)
cpp_target	=$(shell grep "^int main" $(cpp_all) -l)

# .o files: all, w/o main
obj_all		=$(cpp_all:./%.cpp=$(OBJ_DIR)/%.o)
obj_source	=$(cpp_source:./%.cpp=$(OBJ_DIR)/%.o)

# .d files: objects: .o.d, targets: .d
d_obj		=$(cpp_all:./%.cpp=$(OBJ_DIR)/%.o.d)
d_tar		=$(cpp_target:./%.cpp=$(OBJ_DIR)/%.d)

#targets (files with main)
targets		=$(cpp_target:.cpp=)



#*************** Build targets ***************#

all: $(targets)

echo:
	@$(echo) "cpp sources:	$(cpp_source)"
	@$(echo) "cpp targets:	$(cpp_target)"
	@$(echo) "obj source: 	$(obj_source)"
	@$(echo) "obj all:    	$(obj_all)"
	@$(echo) ".d objects: 	$(d_obj)"
	@$(echo) ".d targets: 	$(d_tar)"
	@$(echo) "targets:    	$(targets)"

clean:
	$(rm) $(obj_all)

clean-targets:
	$(rm) $(targets)

clean-dependencies:
	$(rm) $(d_obj) $(d_tar)

clean-all: clean clean-targets clean-dependencies


#*************** Build commands ***************#

$(obj_all):
	@$(print) "build object:" "$@" "$^"
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

$(targets):
	@$(print) "build target:" "$@" "$^"
	@$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^


#*************** Dependency files ***************#

d_files : $(d_obj) $(d_tar)

$(d_obj) : $(OBJ_DIR)/%.o.d : %.cpp
	@$(print) "depend object:" "$@" "$^"
	@$(CPP) $(CPPFLAGS) -MM $< | \
		sed -e 's,^\($(*F)\)\.o[ :]*,$@ $(@D)/\1.o: ,g' \
			-e 's/\w*\/\.\.\///' > $@;

$(d_tar) : %.d : %.o.d
	@$(print) "depend target:" "$@" "$^"
	@xargs < $< make-depend > $@;

-include $(d_obj) $(d_tar)


#*************** Other stuff ***************#

.PHONY: clean clean-target clean-all echo all

