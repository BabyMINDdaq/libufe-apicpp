MESSAGE(" examples ...")

# add_executable (dev-test 2b_test.cpp)
# target_link_libraries(dev-test ufe-apicpp)

MESSAGE(STATUS "ufe-conf-gen")
add_executable (ufe-conf-gen conf_gen.cpp)
target_link_libraries(ufe-conf-gen ufe-apicpp)

MESSAGE(STATUS "ufe-bpar-gen")
add_executable (ufe-bpar-gen bpar_gen.cpp)
target_link_libraries(ufe-bpar-gen ufe-apicpp)

MESSAGE("")