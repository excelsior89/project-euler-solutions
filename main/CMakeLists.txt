# Copyright 2020-2022 Paul Robertson

# A simple CMake file to collect the list of source files
# in this directory (and subdirectories) and store them
# in two variables, HEADER_FILES and SOURCE_FILES.
# These variables can be accessed with the 
# get_directory_property() command to enable the file lists
# to be used by parent CMakeLists

# N.B. this is empty for this directory for now
set(HEADER_FILES
)

set(SOURCE_FILES
	${CMAKE_CURRENT_LIST_DIR}/source/main.cpp
)