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
include CMakeFiles/decoding.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/decoding.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/decoding.dir/flags.make

CMakeFiles/decoding.dir/compress_decoding.cpp.o: CMakeFiles/decoding.dir/flags.make
CMakeFiles/decoding.dir/compress_decoding.cpp.o: ../compress_decoding.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/decoding.dir/compress_decoding.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoding.dir/compress_decoding.cpp.o -c /Users/wangxinhao/CLionProjects/图像压缩/compress_decoding.cpp

CMakeFiles/decoding.dir/compress_decoding.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoding.dir/compress_decoding.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wangxinhao/CLionProjects/图像压缩/compress_decoding.cpp > CMakeFiles/decoding.dir/compress_decoding.cpp.i

CMakeFiles/decoding.dir/compress_decoding.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoding.dir/compress_decoding.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wangxinhao/CLionProjects/图像压缩/compress_decoding.cpp -o CMakeFiles/decoding.dir/compress_decoding.cpp.s

# Object files for target decoding
decoding_OBJECTS = \
"CMakeFiles/decoding.dir/compress_decoding.cpp.o"

# External object files for target decoding
decoding_EXTERNAL_OBJECTS =

decoding: CMakeFiles/decoding.dir/compress_decoding.cpp.o
decoding: CMakeFiles/decoding.dir/build.make
decoding: CMakeFiles/decoding.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable decoding"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decoding.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/decoding.dir/build: decoding

.PHONY : CMakeFiles/decoding.dir/build

CMakeFiles/decoding.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/decoding.dir/cmake_clean.cmake
.PHONY : CMakeFiles/decoding.dir/clean

CMakeFiles/decoding.dir/depend:
	cd /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wangxinhao/CLionProjects/图像压缩 /Users/wangxinhao/CLionProjects/图像压缩 /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug /Users/wangxinhao/CLionProjects/图像压缩/cmake-build-debug/CMakeFiles/decoding.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/decoding.dir/depend
