#########
# Setup #
#########

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Message
message(STATUS "COMPARE Parttitioner")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${PROJECT_SOURCE_DIR})

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project's name
project(compare)

# Set some paths
set(PATH_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(PATH_THIRDPARTY_ROOT ${PATH_PROJECT_ROOT}/thirdparty)
# set(PATH_FLUTE ${PATH_THIRDPARTY_ROOT}/flute)
set(PATH_PATOH ${PATH_THIRDPARTY_ROOT}/patoh)
# set(PATH_SALT ${PATH_THIRDPARTY_ROOT}/salt)
# set(PATH_SALT ${CMAKE_CURRENT_SOURCE_DIR}/salt)
set(PATH_KAHYPAR ${PATH_THIRDPARTY_ROOT}/kahypar)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER}) # use g++ for .c files (flute)

set(PATH_ICCAD19 ${CMAKE_CURRENT_SOURCE_DIR})

###################
# Warnings/Errors #
###################

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=return-type")

######################
# Check Dependencies #
######################
# set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
# find_package(Threads)

###############
# Source Code #
###############

file(GLOB_RECURSE SRC_FILES_ICCAD19 ${PATH_ICCAD19}/*.cpp)

# file(GLOB_RECURSE SRC_FILES_FLUTE ${PATH_FLUTE}/*.c)

# salt
# file(GLOB_RECURSE SALT_SRCS ${PATH_SALT}/*.cpp ${PATH_SALT}/*/*.cpp ${PATH_SALT}/base/flute/*.c)
# add_library(salt STATIC ${SALT_SRCS})
# message(STATUS "salt built")

set(SRC_FILES ${SRC_FILES_ICCAD19})

###########
# Targets #
###########

add_executable(compare ${SRC_FILES})
message(STATUS "compare built")

# add_executable(compare ${SRC_FILES})
# add_executable(compare ${SRC_FILES})

#######################
# Include Directories #
#######################

include_directories(${PATH_ICCAD19})

#################
# Linker Flags  #
#################

# TODO: set static under release mode only
# set_target_properties(TDP PROPERTIES LINK_FLAGS "-Wl,--whole-archive -rdynamic -ldl -lpthread -Wl,--no-whole-archive")
# set_target_properties(TDP PROPERTIES LINK_FLAGS "-static -Wl,--whole-archive -rdynamic -ldl -lpthread -Wl,--no-whole-archive")
set_target_properties(compare PROPERTIES LINK_FLAGS "-Wl,--whole-archive -rdynamic -ldl -lpthread -Wl,--no-whole-archive")

# Boost
target_include_directories(compare PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(compare PUBLIC ${PATH_THIRDPARTY_ROOT})
target_include_directories(compare PUBLIC ${PATH_KAHYPAR}/include)
target_link_libraries(compare ${Boost_LIBRARIES} pthread)
# target_link_libraries(TDP salt)

# Patoh
target_link_libraries(compare ${PATH_PATOH}/libpatoh.a)

#KaHyPar
target_link_libraries(compare ${PATH_KAHYPAR}/lib/libkahypar.so)