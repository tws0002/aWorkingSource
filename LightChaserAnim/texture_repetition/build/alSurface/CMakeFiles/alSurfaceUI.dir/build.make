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

# Utility rule file for alSurfaceUI.

# Include the progress variables for this target.
include alSurface/CMakeFiles/alSurfaceUI.dir/progress.make

alSurface/CMakeFiles/alSurfaceUI: alSurface/alSurface.mtd


alSurface/alSurface.mtd: /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/alSurface/alSurface.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xukai/Documents/TestProgramFactory/texture_repetition/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating alSurface.mtd"
	cd /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface && python /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/uigen.py /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/alSurface/alSurface.ui /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/alSurface.mtd /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/alSurfaceTemplate.py /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/AEalSurfaceTemplate.xml /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/NEalSurfaceTemplate.xml /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/alSurface.spdl /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/alSurface.args /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/docs/alSurface.html

alSurfaceUI: alSurface/CMakeFiles/alSurfaceUI
alSurfaceUI: alSurface/alSurface.mtd
alSurfaceUI: alSurface/CMakeFiles/alSurfaceUI.dir/build.make

.PHONY : alSurfaceUI

# Rule to build all files generated by this target.
alSurface/CMakeFiles/alSurfaceUI.dir/build: alSurfaceUI

.PHONY : alSurface/CMakeFiles/alSurfaceUI.dir/build

alSurface/CMakeFiles/alSurfaceUI.dir/clean:
	cd /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface && $(CMAKE_COMMAND) -P CMakeFiles/alSurfaceUI.dir/cmake_clean.cmake
.PHONY : alSurface/CMakeFiles/alSurfaceUI.dir/clean

alSurface/CMakeFiles/alSurfaceUI.dir/depend:
	cd /home/xukai/Documents/TestProgramFactory/texture_repetition/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders /home/xukai/Documents/TestProgramFactory/texture_repetition/alshaders/alSurface /home/xukai/Documents/TestProgramFactory/texture_repetition/build /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface /home/xukai/Documents/TestProgramFactory/texture_repetition/build/alSurface/CMakeFiles/alSurfaceUI.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : alSurface/CMakeFiles/alSurfaceUI.dir/depend

