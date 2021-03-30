add_library(
    wtl_test_util
    include/Entity.h
    include/catch.hpp
)

target_include_directories(wtl_test_util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(wtl_test_util PROPERTIES LINKER_LANGUAGE CXX)