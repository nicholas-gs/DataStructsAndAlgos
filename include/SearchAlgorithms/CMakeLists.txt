add_library(
    wtl_search_lib
    include/BinarySearch.h
    include/InterpolationSearch.h
    include/JumpSearch.h
    include/TernarySearch.h
)

set(CMAKE_CXX_STANDARD 17)

target_include_directories(wtl_search_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(wtl_search_lib PROPERTIES LINKER_LANGUAGE CXX)