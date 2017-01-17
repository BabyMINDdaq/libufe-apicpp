MESSAGE(" examples ...")

MESSAGE(STATUS "ufe-speed-test")
add_executable (ufe-speed-test speed_test.cpp)
target_link_libraries(ufe-speed-test ufe-cpp)

MESSAGE(STATUS "ufe-conf-gen")
add_executable (ufe-conf-gen conf_gen.cpp)
target_link_libraries(ufe-conf-gen ufe-cpp)

MESSAGE(STATUS "ufe-bpar-gen")
add_executable (ufe-bpar-gen bpar_gen.cpp)
target_link_libraries(ufe-bpar-gen ufe-cpp)

# add_executable (dtest test.cpp)
# target_link_libraries(dtest ufe-cpp)

MESSAGE("")