#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

// Include the relevant UUT h file to conduct the unit testing

// Sorting
#include "SortingAlgorithms/SortingAlgorithms_uut.h"

// Searching

// Graphs
#include "Graphs/SimpleGraph_Unweighted_uut.h"
#include "Graphs/DepthFirstSearch_uut.h"
#include "Graphs/BreadthFirstSearch_uut.h"
#include "Graphs/ConnectedComponents_uut.h"
#include "Graphs/UndirectedBipartite_uut.h"
#include "Graphs/UndirectedCycle_uut.h"
#include "Graphs/DirectedCycle_uut.h"
#include "Graphs/TopologicalSort_uut.h"
#include "Graphs/Kosaraju_uut.h"
#include "Graphs/LazyPrim_uut.h"
#include "Graphs/EagerPrim_uut.h"
#include "Graphs/Kruskal_uut.h"
#include "Graphs/LazyDijkstra_uut.h"
#include "Graphs/EagerDijkstra_uut.h"
#include "Graphs/Topological_SP_uut.h"
// #include "Graphs/BellmanFord_uut.h"
#include "Graphs/Tarjan_uut.h"
#include "Graphs/FloydWarshall_uut.h"

// Data Structures
#include "DataStructures/Array_uut.h"
#include "DataStructures/UnionFind_uut.h"
#include "DataStructures/FenwickTree_uut.h"
#include "DataStructures/SparseTable_uut.h"
#include "DataStructures/UnorderedSet_uut.h"
#include "DataStructures/ClosedMap_uut.h"
#include "DataStructures/BiMap_uut.h"
#include "DataStructures/IndexedPriorityQueue_uut.h"
#include "DataStructures/Queue_uut.h"

// Flow Networks
#include "FlowNetworks/FlowNetwork_uut.h"
#include "FlowNetworks/FordFulkerson_uut.h"
#include "FlowNetworks/EdmondsKarp_uut.h"
#include "FlowNetworks/Dinic_uut.h"

#endif // !CATCH_CONFIG_MAIN
