# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/sylvain/miniconda3/bin/cmake

# The command to remove a file.
RM = /home/sylvain/miniconda3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sylvain/dev/QuantStack/xproperty

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sylvain/dev/QuantStack/xproperty/test

# Include any dependencies generated for this target.
include test/CMakeFiles/test_xproperty.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_xproperty.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_xproperty.dir/flags.make

test/CMakeFiles/test_xproperty.dir/main.cpp.o: test/CMakeFiles/test_xproperty.dir/flags.make
test/CMakeFiles/test_xproperty.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sylvain/dev/QuantStack/xproperty/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_xproperty.dir/main.cpp.o"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_xproperty.dir/main.cpp.o -c /home/sylvain/dev/QuantStack/xproperty/test/main.cpp

test/CMakeFiles/test_xproperty.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_xproperty.dir/main.cpp.i"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sylvain/dev/QuantStack/xproperty/test/main.cpp > CMakeFiles/test_xproperty.dir/main.cpp.i

test/CMakeFiles/test_xproperty.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_xproperty.dir/main.cpp.s"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sylvain/dev/QuantStack/xproperty/test/main.cpp -o CMakeFiles/test_xproperty.dir/main.cpp.s

test/CMakeFiles/test_xproperty.dir/main.cpp.o.requires:

.PHONY : test/CMakeFiles/test_xproperty.dir/main.cpp.o.requires

test/CMakeFiles/test_xproperty.dir/main.cpp.o.provides: test/CMakeFiles/test_xproperty.dir/main.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/test_xproperty.dir/build.make test/CMakeFiles/test_xproperty.dir/main.cpp.o.provides.build
.PHONY : test/CMakeFiles/test_xproperty.dir/main.cpp.o.provides

test/CMakeFiles/test_xproperty.dir/main.cpp.o.provides.build: test/CMakeFiles/test_xproperty.dir/main.cpp.o


test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o: test/CMakeFiles/test_xproperty.dir/flags.make
test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o: test_xobserved.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sylvain/dev/QuantStack/xproperty/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o -c /home/sylvain/dev/QuantStack/xproperty/test/test_xobserved.cpp

test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_xproperty.dir/test_xobserved.cpp.i"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sylvain/dev/QuantStack/xproperty/test/test_xobserved.cpp > CMakeFiles/test_xproperty.dir/test_xobserved.cpp.i

test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_xproperty.dir/test_xobserved.cpp.s"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sylvain/dev/QuantStack/xproperty/test/test_xobserved.cpp -o CMakeFiles/test_xproperty.dir/test_xobserved.cpp.s

test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.requires:

.PHONY : test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.requires

test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.provides: test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/test_xproperty.dir/build.make test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.provides.build
.PHONY : test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.provides

test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.provides.build: test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o


test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o: test/CMakeFiles/test_xproperty.dir/flags.make
test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o: test_xproperty.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sylvain/dev/QuantStack/xproperty/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o -c /home/sylvain/dev/QuantStack/xproperty/test/test_xproperty.cpp

test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_xproperty.dir/test_xproperty.cpp.i"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sylvain/dev/QuantStack/xproperty/test/test_xproperty.cpp > CMakeFiles/test_xproperty.dir/test_xproperty.cpp.i

test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_xproperty.dir/test_xproperty.cpp.s"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && /home/sylvain/miniconda3/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sylvain/dev/QuantStack/xproperty/test/test_xproperty.cpp -o CMakeFiles/test_xproperty.dir/test_xproperty.cpp.s

test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.requires:

.PHONY : test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.requires

test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.provides: test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/test_xproperty.dir/build.make test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.provides.build
.PHONY : test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.provides

test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.provides.build: test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o


# Object files for target test_xproperty
test_xproperty_OBJECTS = \
"CMakeFiles/test_xproperty.dir/main.cpp.o" \
"CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o" \
"CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o"

# External object files for target test_xproperty
test_xproperty_EXTERNAL_OBJECTS =

test/test_xproperty: test/CMakeFiles/test_xproperty.dir/main.cpp.o
test/test_xproperty: test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o
test/test_xproperty: test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o
test/test_xproperty: test/CMakeFiles/test_xproperty.dir/build.make
test/test_xproperty: /home/sylvain/miniconda3/lib/libgtest.a
test/test_xproperty: /home/sylvain/miniconda3/lib/libgtest_main.a
test/test_xproperty: test/CMakeFiles/test_xproperty.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sylvain/dev/QuantStack/xproperty/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable test_xproperty"
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_xproperty.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_xproperty.dir/build: test/test_xproperty

.PHONY : test/CMakeFiles/test_xproperty.dir/build

test/CMakeFiles/test_xproperty.dir/requires: test/CMakeFiles/test_xproperty.dir/main.cpp.o.requires
test/CMakeFiles/test_xproperty.dir/requires: test/CMakeFiles/test_xproperty.dir/test_xobserved.cpp.o.requires
test/CMakeFiles/test_xproperty.dir/requires: test/CMakeFiles/test_xproperty.dir/test_xproperty.cpp.o.requires

.PHONY : test/CMakeFiles/test_xproperty.dir/requires

test/CMakeFiles/test_xproperty.dir/clean:
	cd /home/sylvain/dev/QuantStack/xproperty/test/test && $(CMAKE_COMMAND) -P CMakeFiles/test_xproperty.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_xproperty.dir/clean

test/CMakeFiles/test_xproperty.dir/depend:
	cd /home/sylvain/dev/QuantStack/xproperty/test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sylvain/dev/QuantStack/xproperty /home/sylvain/dev/QuantStack/xproperty/test /home/sylvain/dev/QuantStack/xproperty/test /home/sylvain/dev/QuantStack/xproperty/test/test /home/sylvain/dev/QuantStack/xproperty/test/test/CMakeFiles/test_xproperty.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_xproperty.dir/depend
