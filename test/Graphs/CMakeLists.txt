add_executable(
    graphs_testmain
    graphs_testmain.cpp
)

target_link_libraries(
    graphs_testmain PRIVATE
    wtl_test_util
    wtl_datastructures_lib
    wtl_sort_lib
    wtl_graphs_lib
)