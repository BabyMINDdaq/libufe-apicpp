# Find libxml++
# This module finds an installed libxml++ package.
#
# It sets the following variables:
#  LIBXMLPP_INCLUDE_DIR, where to find header, etc.
#  LIBXMLPP_LIBRARIE, the libraries needed to use jsoncpp.
#  LIBXMLPP_FOUND, If false, do not try to use jsoncpp.

MESSAGE(" Looking for libxml++ ...")

find_path( LIBXMLPP_INCLUDE_DIR NAMES libxml++/libxml++.h PATHS /usr/include/libxml++-2.6/)
find_library( LIBXMLPP_LIBRARY NAMES xml++-2.6)

# MESSAGE(STATUS "LIBXMLPP_INCLUDE_DIR: ${LIBXMLPP_INCLUDE_DIR}")
# MESSAGE(STATUS "LIBXMLPP_LIBRARY: ${LIBXMLPP_LIBRARY}")

IF (LIBXMLPP_INCLUDE_DIR AND LIBXMLPP_LIBRARY)

  SET(LIBXMLPP_FOUND TRUE)

ENDIF (LIBXMLPP_INCLUDE_DIR AND LIBXMLPP_LIBRARY)

IF (LIBXMLPP_FOUND)

  MESSAGE(STATUS "Found libxml++: ${LIBXMLPP_LIBRARY}\n")

ELSE (LIBXMLPP_FOUND)

  MESSAGE(FATAL_ERROR "Could not find libxml++!\n")

ENDIF (LIBXMLPP_FOUND)

