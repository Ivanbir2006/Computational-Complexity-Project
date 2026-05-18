#include "Items.hpp"
#include <gtest/gtest.h>

#include <random>
#include <chrono>
#include <iostream>

using namespace GraphItems;
using namespace std::chrono;

static std::mt19937 rng(42);

Graph GenerateCompleteGraphLarge(size_t n, size_t terminalsCount) {
  std::vector<Edge> edges;
  std::uniform_int_distribution<> weight(1, 20);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      edges.push_back({(Vertex)i, (Vertex)j, weight(rng)});
    }
  }

  std::set<Vertex> terminals;
  while (terminals.size() < terminalsCount) {
    terminals.insert(rng() % n);
  }

  return Graph(std::move(edges), std::move(terminals), n);
}

Graph GenerateRandomGraph(size_t n, double p, size_t terminalsCount) {
  std::vector<Edge> edges;

  std::uniform_real_distribution<> prob(0.0, 1.0);
  std::uniform_int_distribution<> weight(1, 20);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      if (prob(rng) < p) {
        edges.push_back({(Vertex)i, (Vertex)j, weight(rng)});
      }
    }
  }

  std::set<Vertex> terminals;
  while (terminals.size() < terminalsCount) {
    terminals.insert(rng() % n);
  }

  return Graph(std::move(edges), std::move(terminals), n);
}

void Benchmark(const std::string& name,
                std::function<Graph()> generator,
                int testsCount) {

  double totalTimeMs = 0.0;

  for (int t = 0; t < testsCount; ++t) {

    Graph g = generator();

    auto start = high_resolution_clock::now();
    int result = g.GetMinimumMultiCut();
    auto finish = high_resolution_clock::now();

    double elapsed =
      duration_cast<microseconds>(finish - start).count() / 1000.0;

    totalTimeMs += elapsed;

    volatile int sink = result;
    (void)sink;
  }

  std::cout << "\n==== " << name << " ====\n";
  std::cout << "Tests: " << testsCount << "\n";
  std::cout << "Average time (ms): "
            << (totalTimeMs / testsCount) << "\n";
}

// TESTS
TEST(MultiCutBenchmark, CompleteGraphBigOnly) {
  Benchmark(
    "Complete Graph (n=200)",
    []() {
      return GenerateCompleteGraphLarge(200, 10);
    },
    20
  );
}

TEST(MultiCutBenchmark, RandomGraphBigOnlyLowProbability) {
  Benchmark(
    "Random G(n,p) (n=300, p=0.1)",
    []() {
      return GenerateRandomGraph(300, 0.1, 10);
    },
    20
  );
}

TEST(MultiCutBenchmark, RandomGraphBigOnlyHighProbability) {
  Benchmark(
    "Random G(n,p) (n=200, p=0.5)",
    []() {
      return GenerateRandomGraph(200, 0.8, 10);
    },
    20
  );
}
