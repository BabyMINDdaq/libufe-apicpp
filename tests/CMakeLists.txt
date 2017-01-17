message(STATUS "libufecpp-tests lib ...")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              TESTS_SOURCE_FILES)
# message(STATUS "TESTS_SOURCE_FILES " ${TESTS_SOURCE_FILES})

add_library (libufecpp-tests      ${TESTS_SOURCE_FILES}
                             ${Data_DICTIONARY})

target_link_libraries(libufecpp-tests  ufe-cpp cppunit)

ADD_EXECUTABLE(unit_test              UnitTests.cpp)
TARGET_LINK_LIBRARIES(unit_test       libufecpp-tests)

