# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wangxinhao/CLionProjects/图像压缩

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/judge.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/judge.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/judge.dir/flags.make

CMakeFiles/judge.dir/judgeResult.cpp.o: CMakeFiles/judge.dir/flags.make
CMakeFiles/judge.dir/judgeResult.cpp.o: ../judgeResult.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/judge.dir/judgeResult.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/judge.dir/judgeResult.cpp.o -c /Users/wangxinhao/CLionProjects/图像压缩/judgeResult.cpp

CMakeFiles/judge.dir/judgeResult.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/judge.dir/judgeResult.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wangxinhao/CLionProjects/图像压缩/judgeResult.cpp > CMakeFiles/judge.dir/judgeResult.cpp.i

CMakeFiles/judge.dir/judgeResult.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/judge.dir/judgeResult.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wangxinhao/CLionProjects/图像压缩/judgeResult.cpp -o CMakeFiles/judge.dir/judgeResult.cpp.s

# Object files for target judge
judge_OBJECTS = \
"CMakeFiles/judge.dir/judgeResult.cpp.o"

# External object files for target judge
judge_EXTERNAL_OBJECTS =

judge: CMakeFiles/judge.dir/judgeResult.cpp.o
judge: CMakeFiles/judge.dir/build.make
judge: CMakeFiles/judge.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable judge"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/judge.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/judge.dir/build: judge

.PHONY : CMakeFiles/judge.dir/build

CMakeFiles/judge.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/judge.dir/cmake_clean.cmake
.PHONY : CMakeFiles/judge.dir/clean

CMakeFiles/judge.dir/depend:
	cd /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wangxinhao/CLionProjects/图像压缩 /Users/wangxinhao/CLionProjects/图像压缩 /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug/CMakeFiles/judge.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/judge.dir/depend

