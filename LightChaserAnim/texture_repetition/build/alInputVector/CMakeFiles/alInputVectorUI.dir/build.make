# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /home/xukai/Applications/cmake-3.7.1/bin/cmake

# The command to remove a file.
RM = /home/xukai/Applications/cmake-3.7.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xukai/Documents/TestProgramFactory/texture_repetition/build

# Utility rule file for alInputVectorUI.

# Include the progress variables for this target.
include alInputVector/CMakeFiles/alInputVectorUI.dir/progress.make

alInputVector/CMakeFiles/alInputVectorUI: alInputVector/alInputVector.mtd


alInputVector/alInputVector.mtd: /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/alInputVector/alInputVector.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xukai/Documents/TestProgramFactory/texture_repetition/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating alInputVector.mtd"
	cd /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector && python /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/uigen.py /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/alInputVector/alInputVector.ui /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/alInputVector.mtd /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/alInputVectorTemplate.py /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/AEalInputVectorTemplate.xml /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/NEalInputVectorTemplate.xml /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/alInputVector.spdl /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/alInputVector.args /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/docs/alInputVector.html

alInputVectorUI: alInputVector/CMakeFiles/alInputVectorUI
alInputVectorUI: alInputVector/alInputVector.mtd
alInputVectorUI: alInputVector/CMakeFiles/alInputVectorUI.dir/build.make

.PHONY : alInputVectorUI

# Rule to build all files generated by this target.
alInputVector/CMakeFiles/alInputVectorUI.dir/build: alInputVectorUI

.PHONY : alInputVector/CMakeFiles/alInputVectorUI.dir/build

alInputVector/CMakeFiles/alInputVectorUI.dir/clean:
	cd /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector && $(CMAKE_COMMAND) -P CMakeFiles/alInputVectorUI.dir/cmake_clean.cmake
.PHONY : alInputVector/CMakeFiles/alInputVectorUI.dir/clean

alInputVector/CMakeFiles/alInputVectorUI.dir/depend:
	cd /home/xukai/Documents/TestProgramFactory/texture_repetition/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/alInputVector /home/xukai/Documents/TestProgramFactory/texture_repetition/build /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alInputVector/CMakeFiles/alInputVectorUI.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : alInputVector/CMakeFiles/alInputVectorUI.dir/depend

