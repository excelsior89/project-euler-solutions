# Copyright 2020-2023 Paul Robertson

# A simple CMake file to collect the list of source files
# in this directory (and subdirectories) and store them
# in two variables, HEADER_FILES and SOURCE_FILES.
# These variables can be accessed with the 
# get_directory_property() command to enable the file lists
# to be used by parent CMakeLists

set(HEADER_FILES
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem001.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem002.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem003.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem004.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem005.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem006.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem007.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem008.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem009.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem010.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem011.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem012.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem013.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem014.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem015.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem016.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem017.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem018.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem019.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem020.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem021.h
)

set(SOURCE_FILES
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem001.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem002.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem003.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem004.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem005.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem006.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem007.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem008.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem009.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem010.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem011.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem012.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem013.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem014.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem015.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem016.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem017.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem018.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem019.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem020.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem021.cpp
)