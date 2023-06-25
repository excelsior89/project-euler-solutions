# Copyright 2020-2023 Paul Robertson
# Root CMake file for Project Euler solutions


## TOP LEVEL SETTINGS ##

# CMake version limits (a bit arbitrary but I think this needs at least version 3.8)
cmake_minimum_required(VERSION 3.15)

# C++ language standards
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set up the project and declare languages
project(ProjectEuler LANGUAGES CXX)

# Allow organisation into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

## PROJECT ITSELF ##

# Set a target name
set(TARGET_NAME ProjectEuler)

# Declare the ProjectEuler executible
add_executable(${TARGET_NAME})

# Add source file subtrees
add_subdirectory(problems)
add_subdirectory(util)
add_subdirectory(main)

# Get file list variables definied in the above subdirectories

# Problems
get_directory_property(PROBLEM_HEADER_FILES DIRECTORY problems
                       DEFINITION HEADER_FILES)
get_directory_property(PROBLEM_SOURCE_FILES DIRECTORY problems
                       DEFINITION SOURCE_FILES)

# Util
get_directory_property(UTILITY_HEADER_FILES DIRECTORY util
                       DEFINITION HEADER_FILES)
get_directory_property(UTILITY_SOURCE_FILES DIRECTORY util
                       DEFINITION SOURCE_FILES)
				
# Main
get_directory_property(MAIN_HEADER_FILES DIRECTORY main
                       DEFINITION HEADER_FILES) # N.B. this is empty for now
get_directory_property(MAIN_SOURCE_FILES DIRECTORY main
                       DEFINITION SOURCE_FILES)
					
# Include directories
target_include_directories(${TARGET_NAME} PRIVATE
	problems/include
	util/include
)

# Use the TREE option in source group to create organisation
# folders in the created project
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES
	${MAIN_HEADER_FILES}
	${MAIN_SOURCE_FILES}
	${PROBLEM_HEADER_FILES}
	${PROBLEM_SOURCE_FILES}
	${UTILITY_HEADER_FILES}
	${UTILITY_SOURCE_FILES}
)

# Add the discovered target sources to the target
target_sources(${TARGET_NAME} PRIVATE
	${MAIN_HEADER_FILES}
	${MAIN_SOURCE_FILES}
	${PROBLEM_HEADER_FILES}
	${PROBLEM_SOURCE_FILES}
	${UTILITY_HEADER_FILES}
	${UTILITY_SOURCE_FILES}
)

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)


