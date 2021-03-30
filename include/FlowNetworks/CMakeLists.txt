add_library(
    wtl_flownetworks_lib
    include/Dinic.h
    include/EdmondsKarp.h
    include/FlowNetwork.h
    include/FordFulkerson.h
    include/MinCut.h
)

set(CMAKE_CXX_STANDARD 17)

target_include_directories(wtl_flownetworks_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(wtl_flownetworks_lib PROPERTIES LINKER_LANGUAGE CXX)