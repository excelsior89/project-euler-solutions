# Copyright 2020 Paul Robertson

# A simple CMake file to collect the list of source files
# in this directory (and subdirectories) and store them
# in two variables, HEADER_FILES and SOURCE_FILES.
# These variables can be accessed with the 
# get_directory_property() command to enable the file lists
# to be used by parent CMakeLists

set(HEADER_FILES
	${CMAKE_CURRENT_LIST_DIR}/include/PeBigInt.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblemSelector.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeUtilities.h
)

set(SOURCE_FILES
	${CMAKE_CURRENT_LIST_DIR}/source/PeBigInt.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblemSelector.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeUtilities.cpp
)