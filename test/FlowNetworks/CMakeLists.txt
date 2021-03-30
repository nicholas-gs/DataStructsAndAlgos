add_executable(
    flownetworks_testmain
    flownetworks_testmain.cpp
)

target_link_libraries(
    flownetworks_testmain PRIVATE
    wtl_test_util
    wtl_graphs_lib
    wtl_flownetworks_lib
)