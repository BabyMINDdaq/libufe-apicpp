MESSAGE(" libufe-cppc ...")

# Get a list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.  SOURCE_FILES)

if (_STATIC)

  MESSAGE(STATUS "building static library\n")
  add_library(ufe-cpp        ${SOURCE_FILES})
else (_STATIC)

  MESSAGE(STATUS "building shered library\n")
  add_library(ufe-cpp SHARED ${SOURCE_FILES})
endif()

target_link_libraries(ufe-cpp   ${UFEC_LIBRARY}
                                ${JSONCPP_LIBRARY})

