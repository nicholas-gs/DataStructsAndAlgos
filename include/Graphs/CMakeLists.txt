add_library(
    wtl_graphs_lib
    include/BellmanFord.h
    include/BreadthFirstSearch.h
    include/ConnectedComponents.h
    include/DepthFirstSearch.h
    include/DirectedCycle.h
    include/EagerDijkstra.h 
    include/EagerPrim.h 
    include/FloydWarshall.h 
    include/Kahn.h 
    include/Kosaraju.h 
    include/Kruskal.h 
    include/LazyDijkstra.h 
    include/LazyPrim.h 
    include/SimpleGraph_Unweighted.h 
    include/SimpleGraph_Weighted.h 
    include/Tarjan.h
    include/Topological_SP.h
    include/TopologicalSort.h 
    include/UndirectedBipartite.h 
    include/UndirectedCycle.h
)

set(CMAKE_CXX_STANDARD 17)

target_include_directories(wtl_graphs_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(wtl_graphs_lib PROPERTIES LINKER_LANGUAGE CXX)