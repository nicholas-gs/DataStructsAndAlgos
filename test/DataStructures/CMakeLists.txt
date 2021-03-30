add_executable(
    datastructures_testmain
    datastructures_testmain.cpp
)

target_link_libraries(
    datastructures_testmain PRIVATE
    wtl_test_util
    wtl_datastructures_lib
)