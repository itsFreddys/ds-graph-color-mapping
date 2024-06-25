#include "InterferenceGraph.hpp"
#include "CSVReader.hpp"
#include "IGWriter.hpp"
#include "register_allocation.hpp"
#include "catch_amalgamated.hpp"
#include <iostream>
#include "verifier.hpp"


namespace {
TEST_CASE("Complete6-BasicTests", "[ig-hydrohn]") {
    const auto &GRAPH = "tests/graphs/complete_6.csv";
    const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);
    const std::unordered_set<Variable> &expected_vertices = {"1", "2", "3", "4", "5", "6"};

    SECTION("CompleteVertexTest"){
        REQUIRE(ig.numVertices() == 6);
        REQUIRE(ig.vertices() == expected_vertices);
    }

    SECTION("EdgeCapacityTest"){
        REQUIRE(ig.numEdges() == 15);
    }
    
    SECTION("InterferesTestI"){
        REQUIRE(ig.interferes("1","2"));
        REQUIRE(ig.interferes("1","3"));
        REQUIRE(ig.interferes("1","4"));
        REQUIRE(ig.interferes("1","5"));
        REQUIRE(ig.interferes("1","6"));
        REQUIRE(ig.interferes("2","3"));
        REQUIRE(ig.interferes("2","4"));
        REQUIRE(ig.interferes("2","5"));
        REQUIRE(ig.interferes("2","6"));
        REQUIRE(ig.interferes("3","4"));
        REQUIRE(ig.interferes("3","5"));
        REQUIRE(ig.interferes("3","6"));
        REQUIRE(ig.interferes("4","5"));
        REQUIRE(ig.interferes("4","6"));
        REQUIRE(ig.interferes("5","6"));
    }

    SECTION("InterferesTestII"){
        REQUIRE(ig.interferes("2","1"));
        REQUIRE(ig.interferes("3","1"));
        REQUIRE(ig.interferes("4","1"));
        REQUIRE(ig.interferes("5","1"));
        REQUIRE(ig.interferes("6","1"));
        REQUIRE(ig.interferes("3","2"));
        REQUIRE(ig.interferes("4","2"));
        REQUIRE(ig.interferes("5","2"));
        REQUIRE(ig.interferes("6","2"));
        REQUIRE(ig.interferes("4","3"));
        REQUIRE(ig.interferes("5","3"));
        REQUIRE(ig.interferes("6","3"));
        REQUIRE(ig.interferes("5","4"));
        REQUIRE(ig.interferes("6","4"));
        REQUIRE(ig.interferes("6","5"));
    }

    const std::unordered_set<Variable> &expected_neighbors_1 = {"2", "3", "4", "5", "6"};
    const std::unordered_set<Variable> &expected_neighbors_2 = {"1", "3", "4", "5", "6"};
    const std::unordered_set<Variable> &expected_neighbors_3 = {"1", "2", "4", "5", "6"};
    const std::unordered_set<Variable> &expected_neighbors_4 = {"1", "2", "3", "5", "6"};
    const std::unordered_set<Variable> &expected_neighbors_5 = {"1", "2", "3", "4", "6"};
    const std::unordered_set<Variable> &expected_neighbors_6 = {"1", "2", "3", "4", "5"};


    SECTION("NeighborsTest"){
        REQUIRE(ig.neighbors("1") == expected_neighbors_1);
        REQUIRE(ig.neighbors("2") == expected_neighbors_2);
        REQUIRE(ig.neighbors("3") == expected_neighbors_3);
        REQUIRE(ig.neighbors("4") == expected_neighbors_4);
        REQUIRE(ig.neighbors("5") == expected_neighbors_5);
        REQUIRE(ig.neighbors("6") == expected_neighbors_6);
    }

    SECTION("CompleteGraphDegreeTest"){
        REQUIRE(ig.degree("1") == 5);
        REQUIRE(ig.degree("2") == 5);
        REQUIRE(ig.degree("2") == 5);
        REQUIRE(ig.degree("3") == 5);
        REQUIRE(ig.degree("4") == 5);
        REQUIRE(ig.degree("5") == 5);
        REQUIRE(ig.degree("6") == 5);
        REQUIRE(ig.numVertices() - 1 == 5);
    }
}

TEST_CASE("RuntimeTest", "[ig-complete_1000]") 
    {
    const auto &GRAPH = "tests/graphs/jake.csv";
    InterferenceGraph<Variable> ig = CSVReader::load(GRAPH);

    for(unsigned i = 0; i < ig.numVertices() - 1; i++)
    {
        const std::string s = "Node_" + std::to_string(i); 
        //REQUIRE(ig.neighbors(s).size() >= 998);
    }
    
    REQUIRE(ig.numVertices() == 1000);
    REQUIRE(ig.numEdges() == 499500);

    ig.removeVertex("Node_1");
    REQUIRE(ig.numVertices() == 999);
    REQUIRE(ig.numEdges() == 498501);

    ig.removeEdge("Node_2", "Node_420");
    REQUIRE(ig.numEdges() == 498500);

    REQUIRE(ig.vertices().size() == 999);
    REQUIRE(ig.degree("Node_5") == 998);
    REQUIRE(ig.neighbors("Node_666").size() == 998);
    }

    TEST_CASE("P2 Runtime Test", "[ig-complete-250]")
    {
    // Load the graph pointed to by graph_stress_test.csv
    const auto &GRAPH = "tests/graphs/graph_stress_test-2.csv";
    const auto NUM_REGS = 500;
    const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
    
    // Not included in time-test, adds about 6 seconds
    // IGWriter::write(CSVReader::load(GRAPH), "tests/graphs/stress_success.dot",
    //              allocation);

    REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
    }

}