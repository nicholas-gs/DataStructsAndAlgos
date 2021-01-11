# Graphs

## Graph Representations

### Simple Graphs

A Simple Graph is a graph that does not allow parallel edges between vertices. Self-loops are allowed. The graphs can be either directed or undirected, as well as weighted or unweighted.

* SimpleGraph_Unweighted
* SimpleGraph_Weighted

## Algorithms

### Basic Algorithms

* __BreadthFirstSearch__
    * Connectivity - Given a source vertex, check if there is a bi-directional path to another vertex.
    * Shortest Single Source Path - Find the bidirectional shortest path (in terms of number of edges) from a given vertex to another vertex.

* __Bipartite__
    * Disjoint Sets - Check if the vertices can be divided into 2 disjoint and independent sets. Also known as Two-Colorability problem. 

* __ConnectedComponents__
    * Connectivity - Check if a path exists between any 2 vertices.

* __DirectedCycle__
    * Cycle - Does a given directed graph have a directed cycle

* __UndirectedCycle__
    * Cycle - Does a given undirected graph have a cycle.

* __DepthFirstSearch__
    * Connectivity - Given a source vertex, check if there is a bi-directional path to another vertex.
    * Single Source Path - Find a path from a given vertex to another vertex.

* __DepthFirstOrder__
     * Traversal Ordering - Preorder, Postorder or Reverse Postorder of vertices

### Strongly Connected Components

* __Kosaraju__
* __Tarjan__

### Topological Ordering

* __TopologicalSort__
* __Kahn__

### Minimum Spanning Tree

* __EagerPrim__
* __LazyPrim__
* __Kruskal__

### Shortest/Longest Weighted Path

* __LazyDijkstra__
* __EagerDijkstra__
* __Topological_SP__
* __BellmanFord__

### All-pairs Shortest Path

* __FloydWarshall__

## Categorised by Graphs

Many graph processing algorithms only works on certain types of graphs. Below are the algorithms categorised by what kind of graph they can process.

Also for simplicity, certain algorithms which should work for weighted graphs are restricted to work only on unweighted graphs. For example, `UndirectedCycle` only works for unweighted graph. To use the algorithm, get a unweighted graph representation from a weighted graph using the `convert()` method present in the `SimpleGraph_Weighted` class.

### Undirected and Unweighted

* BreadthFirstSearch
* UndirectedBipartite
* ConnectedComponents
* UndirectedCycle
* DepthFirstSearch
* DepthFirstOrder

### Directed and Unweighted

* BreadthFirstSearch
* DepthFirstSearch
* DepthFirstOrder
* DirectedCycle
* Tarjan
* TopologicalSort
* Kahn
* Kosaraju

### Undirected and Weighted

* LazyPrim
* EagerPrim
* Kruskal

### Directed and Weighted

* LazyDijkstra
* EagerDijkstra
* Topological_SP
* BellmanFord
* FloydWarshall
