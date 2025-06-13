# Set cmake version requirement
cmake_minimum_required(VERSION 3.14)

project(p3)

# Compiler options
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "-pthread")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories("$ENV{LIBRARIES_PATH}/gtest/include")
link_directories("$ENV{LIBRARIES_PATH}/gtest/lib")

# -------------------------------------------------
# Sources for library target
# -------------------------------------------------
# General includes
set(SOURCES_TARGET_LIBRARY
	# Byte level utilities
	include/bytes/stream.h
	source/bytes/stream.cpp
)

# -------------------------------------------------
# Sources for executable target
# -------------------------------------------------
set(SOURCES_TARGET_EXE
	# Main entry point
	source/main.cpp
)

# -------------------------------------------------
# Tests
# -------------------------------------------------
set(SOURCES_TARGET_TESTS
	tests/test_main.cpp

	tests/bytes/stream.cpp
)

# -------------------------------------------------
# Build targets
# -------------------------------------------------
add_library(p3lib STATIC ${SOURCES_TARGET_LIBRARY})
add_executable(p3run ${SOURCES_TARGET_EXE})
target_link_libraries(p3run p3lib)

# The tests
add_executable(p3tests ${SOURCES_TARGET_TESTS})
target_link_libraries(p3tests gtest p3lib)

