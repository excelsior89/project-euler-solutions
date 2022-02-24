# Copyright 2020-2022 Paul Robertson

# A simple CMake file to collect the list of source files
# in this directory (and subdirectories) and store them
# in two variables, HEADER_FILES and SOURCE_FILES.
# These variables can be accessed with the 
# get_directory_property() command to enable the file lists
# to be used by parent CMakeLists

set(HEADER_FILES
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem1.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem2.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem3.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem4.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem5.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem6.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem7.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem8.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem9.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem10.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem11.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem12.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem13.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem14.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem15.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem16.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem17.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem18.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem19.h
	${CMAKE_CURRENT_LIST_DIR}/include/PeProblem20.h
)

set(SOURCE_FILES
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem1.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem2.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem3.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem4.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem5.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem6.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem7.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem8.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem9.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem10.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem11.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem12.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem13.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem14.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem15.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem16.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem17.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem18.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem19.cpp
	${CMAKE_CURRENT_LIST_DIR}/source/PeProblem20.cpp
)