add_executable(
    sorting_testmain
    sorting_testmain.cpp
)

target_link_libraries(
    sorting_testmain PRIVATE
    wtl_test_util
    # wtl_datastructures_lib
    # wtl_search_lib
    wtl_sort_lib
    # wtl_graphs_lib
    # wtl_flownetworks_lib
)