#pragma once
#include <gtest/gtest.h>

#include "Items.hpp"
#include "dummy.hpp"

#include <random>

using namespace GraphItems;

static std::mt19937 rng(42);

inline void CheckApproximation(const Graph& g) {
  int approx = g.GetMinimumMultiCut();
  auto [_, exactDouble] = FindBestPartition(g);

  int exact = static_cast<int>(exactDouble);
  EXPECT_GE(approx, exact);

  if (exact == 0) {
    EXPECT_EQ(approx, 0);
    return;
  }

  double ratio = static_cast<double>(approx) / exact;
  EXPECT_LE(ratio, 2.0);
}

inline Graph GenerateRandomGraph(size_t n,
                          double p,
                          size_t terminalsCount,
                          int maxWeight = 20) {
  std::vector<Edge> edges;

  std::uniform_real_distribution<> prob(0.0, 1.0);
  std::uniform_int_distribution<> weight(1, maxWeight);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      if (prob(rng) < p) {
        int w = weight(rng);

        edges.push_back({(Vertex)j, (Vertex)i, w});
      }
    }
  }

  std::set<Vertex> terminals;

  while (terminals.size() < terminalsCount) {
    terminals.insert(rng() % n);
  }

  return Graph(std::move(edges), std::move(terminals), n);
}

inline Graph GenerateTreeGraph(size_t n, size_t terminalsCount) {
  std::vector<Edge> edges;

  std::uniform_int_distribution<> weight(1, 20);

  for (size_t v = 1; v < n; ++v) {
    size_t parent = rng() % v;

    int w = weight(rng);

    edges.push_back({(Vertex)v, (Vertex)parent, w});
  }

  std::set<Vertex> terminals;

  while (terminals.size() < terminalsCount) {
    terminals.insert(rng() % n);
  }

  return Graph(std::move(edges), std::move(terminals), n);
}

inline Graph GenerateCompleteGraph(size_t n, size_t terminalsCount) {
  std::vector<Edge> edges;

  std::uniform_int_distribution<> weight(1, 20);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {

      int w = weight(rng);

      edges.push_back({(Vertex)i, (Vertex)j, w});
    }
  }

  std::set<Vertex> terminals;

  while (terminals.size() < terminalsCount) {
    terminals.insert(rng() % n);
  }

  return Graph(std::move(edges), std::move(terminals), n);
}
