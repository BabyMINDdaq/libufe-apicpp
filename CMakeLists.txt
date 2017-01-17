# Check if cmake has the required version
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

# Set name of our project
project(ufecpp)
MESSAGE("\n project: ufe-apicpp \n")

set(UFEAPI_DIR ${CMAKE_SOURCE_DIR})

include(${CMAKE_BINARY_DIR}/FindLibUfec.cmake)
include(${CMAKE_BINARY_DIR}/FindJsonCpp.cmake)

configure_file( ${CMAKE_BINARY_DIR}/deff.h.cmake
                ${CMAKE_SOURCE_DIR}/src/UFEDeff.h)

set(LIBRARY_OUTPUT_PATH    "${CMAKE_SOURCE_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")

if (VERBOSE GREATER 1)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUFE_INFO")
endif (VERBOSE GREATER 1)

if (VERBOSE GREATER 2)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUFE_DEBUGG")
endif (VERBOSE GREATER 2)

include_directories(  ${CMAKE_SOURCE_DIR}/.
                      ${CMAKE_SOURCE_DIR}/src
                      ${UFEC_INCLUDE_DIR}
                      ${MIC11API_INCLUDE_DIR}
                      ${JSONCPP_INCLUDE_DIR})

link_directories(     ${CMAKE_SOURCE_DIR}/lib
                      ${MIC11API_LIBRARY_DIR}
                      /usr/local/lib/)

message(STATUS "C flags      : " ${CMAKE_C_FLAGS})
message(STATUS "CXX flags    : " ${CMAKE_CXX_FLAGS})
message(STATUS "Linker flags : " ${CMAKE_EXE_LINKER_FLAGS}\n)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/tools)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)


