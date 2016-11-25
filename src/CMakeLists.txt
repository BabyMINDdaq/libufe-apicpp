MESSAGE(" libufe-apicppc ...")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              SOURCE_FILES)

if (STATIC)

  MESSAGE(STATUS "building static library\n")
  add_library(ufe-apicpp        ${SOURCE_FILES})
else (STATIC)

  MESSAGE(STATUS "building shered library\n")
  add_library(ufe-apicpp SHARED ${SOURCE_FILES})
endif()

target_link_libraries(ufe-apicpp   ${UFEC_LIBRARY}
                                   ${JSONCPP_LIBRARY})

