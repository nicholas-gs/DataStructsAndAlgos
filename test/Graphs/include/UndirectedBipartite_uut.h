//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include "catch.hpp"
#include "SimpleGraph_Unweighted.h"
#include "UndirectedBipartite.h"

TEST_CASE("Undirected Bipartite") {

    SECTION("Single Vertex Graph with no self-loop"){
        wtl::SimpleGraph_Unweighted<false> graph(1);
        wtl::UndirectedBipartite bipartite(graph);
        REQUIRE(bipartite.isBipartite() == true);
    }

    SECTION("Graph with self-loop"){
        wtl::SimpleGraph_Unweighted<false> graph(3);
        graph.addEdge(0,1);
        graph.addEdge(0,2);
        graph.addEdge(2,2);

        wtl::UndirectedBipartite bipartite(graph);
        REQUIRE(bipartite.isBipartite() == false);
    }

    SECTION("10 vertex connected bipartite graph"){
        wtl::SimpleGraph_Unweighted<false> graph(10);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(3, 9);
        graph.addEdge(3, 4);
        graph.addEdge(4, 5);
        graph.addEdge(5, 0);
        graph.addEdge(0, 6);
        graph.addEdge(6, 7);
        graph.addEdge(7, 8);
        graph.addEdge(8, 0);

        wtl::UndirectedBipartite bipartite(graph);
        REQUIRE(bipartite.isBipartite() == true);
    }

    SECTION("9 vertex non-connected non-bipartite graph"){
        wtl::SimpleGraph_Unweighted<false> graph(9);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(0, 2);
        graph.addEdge(8, 1);
        graph.addEdge(3, 4);
        graph.addEdge(4, 7);
        graph.addEdge(7, 6);
        graph.addEdge(5, 6);
        graph.addEdge(4, 5);

        wtl::UndirectedBipartite bipartite(graph);
        REQUIRE(bipartite.isBipartite() == false);
    }

    SECTION("8 vertex non-connected non-bipartite graph"){
        wtl::SimpleGraph_Unweighted<false> graph(8);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(0, 2);
        graph.addEdge(3, 4);
        graph.addEdge(4, 7);
        graph.addEdge(7, 6);
        graph.addEdge(5, 6);
        graph.addEdge(4, 5);

        wtl::UndirectedBipartite bipartite(graph);
        REQUIRE(bipartite.isBipartite() == false);
    }

    SECTION("8 vertex connected non-bipartite graph") {
        wtl::SimpleGraph_Unweighted<false> graph(8);
        graph.addEdge(0, 1);
        graph.addEdge(0, 2);
        graph.addEdge(1, 2);
        graph.addEdge(1, 3);
        graph.addEdge(1, 4);
        graph.addEdge(4, 3);
        graph.addEdge(5, 6);
        graph.addEdge(0, 6);
        graph.addEdge(7, 6);

        wtl::UndirectedBipartite bipartite(graph);
        REQUIRE(bipartite.isBipartite() == false);
    }

}
