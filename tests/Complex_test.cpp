#include <gtest/gtest.h>

#include "Items.hpp"
#include "Generators.hpp"

#include <iostream>
#include <random>

using namespace GraphItems;

// TESTS
TEST(MultiCutApproximation, TwoTerminalsExact) {
  for (int test = 0; test < 100; ++test) {
    size_t n = 8;

    std::vector<Edge> edges;
    std::uniform_int_distribution<> weight(1, 10);

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        edges.push_back({(Vertex)i, (Vertex)j, weight(rng)});
      }
    }

    std::set<Vertex> terminals = {0, 1};
    Graph g(std::move(edges), std::move(terminals), n);
    int approx = g.GetMinimumMultiCut();

    auto [partition, exactDouble] = FindBestPartition(g);
    int exact = static_cast<int>(exactDouble);
    EXPECT_EQ(approx, exact);
  }

  std::cout << "Two terminals exact test --- OK\n\n";
}

TEST(MultiCutApproximation, AllVerticesAreTerminals) {
  for (int test = 0; test < 50; ++test) {
    size_t n = 8;
    std::vector<Edge> edges;
    std::uniform_int_distribution<> weight(1, 10);

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        int w = weight(rng);
        edges.push_back({(Vertex)i, (Vertex)j, w});
      }
    }

    std::set<Vertex> terminals;
    for (Vertex v = 0; v < n; ++v) {
      terminals.insert(v);
    }

    Graph g(std::move(edges), std::move(terminals), n);
    CheckApproximation(g);
  }

  std::cout << "All vertices are terminals --- OK\n\n";
}

TEST(MultiCutApproximation, SimpleGraph) {
  for (int test = 0; test < 1000; ++test) {
    Graph g = GenerateRandomGraph(4, 1.0, 2);
    CheckApproximation(g);
  }

  std::cout << "Simple graph --- success\n\n";
}

TEST(MultiCutApproximation, RandomGraphsSmall) {
  for (int test = 0; test < 100; ++test) {
    Graph g = GenerateRandomGraph(8, 0.4, 3);
    CheckApproximation(g);
  }
  std::cout << "Random Graphs with vertex size 8 --- success\n\n";
}

TEST(MultiCutApproximation, RandomGraphsSmallWithWeight1) {
  for (int test = 0; test < 100; ++test) {
    Graph g = GenerateRandomGraph(10, 0.4, 5, 1);
    CheckApproximation(g);
  }

  std::cout << "Random Graphs with vertex size 10 and weight 1 --- success\n\n";
}

TEST(MultiCutApproximation, Trees) {
  for (int test = 0; test < 100; ++test) {
    Graph g = GenerateTreeGraph(10, 3);
    CheckApproximation(g);
  }

  std::cout << "Trees --- success\n\n";
}

TEST(MultiCutApproximation, CompleteGraphs) {
  for (int test = 0; test < 100; ++test) {
    Graph g = GenerateCompleteGraph(10, 3);
    CheckApproximation(g);
  }

  std::cout << "Complete Graphs --- success\n\n";
}

TEST(MultiCutApproximation, RandomGraphsLarge) {
  for (int test = 0; test < 20; ++test) {
    Graph g = GenerateRandomGraph(12, 0.5, 4);
    CheckApproximation(g);
  }

  std::cout << "Random Graphs with vertex size 18 --- success\n\n";
}