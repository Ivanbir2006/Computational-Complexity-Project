#include <algorithm>
#include <chrono>
#include <iostream>
#include <functional>
#include <gtest/gtest.h>
#include "Generators.hpp"

using namespace GraphItems;
using namespace std::chrono;

void BenchmarkGraphFamily(
  const std::string& name,
  std::function<Graph()> generator,
  int testsCount) {

  double avgRatio = 0.0;
  double maxRatio = 0.0;
  double avgTimeMs = 0.0;
  int validTests = 0;

  for (int t = 0; t < testsCount; ++t) {
    Graph g = generator();
    auto start = high_resolution_clock::now();
    int approx = g.GetMinimumMultiCut();
    auto finish = high_resolution_clock::now();

    double elapsed =
      duration_cast<microseconds>(finish - start).count() / 1000.0;

    avgTimeMs += elapsed;
    auto [partition, exactDouble] = FindBestPartition(g);
    int exact = static_cast<int>(exactDouble);

    if (exact == 0) {
      continue;
    }

    double ratio =
      static_cast<double>(approx) / exact;
    avgRatio += ratio;
    maxRatio = std::max(maxRatio, ratio);

    ++validTests;
  }

  if (validTests > 0) {
    avgRatio /= validTests;
  }
  avgTimeMs /= testsCount;
  std::cout << "\n==== " << name << " ====\n";
  std::cout << "Tests: " << testsCount << "\n";
  std::cout << "Average ratio: " << avgRatio << "\n";
  std::cout << "Max ratio: " << maxRatio << "\n";
  std::cout << "Average time (ms): " << avgTimeMs << "\n";
}

TEST(MultiCutBenchmark, FullBenchmark) {
  std::cout << "Starting benchmarks...\n";
  BenchmarkGraphFamily(
    "Random G(n,p)",
    []() {
      return GenerateRandomGraph(8, 0.4, 3);
    },
    100
  );

  BenchmarkGraphFamily(
    "Random G(n,p) with weight 1",
    []() {
      return GenerateRandomGraph(10, 0.4, 5, 1);
    },
    100
  );

  BenchmarkGraphFamily(
    "Trees",
    []() {
      return GenerateTreeGraph(10, 3);
    },
    100
  );

  BenchmarkGraphFamily(
    "Complete Graphs",
    []() {
      return GenerateCompleteGraph(10, 3);
    },
    1000
  );

  BenchmarkGraphFamily(
    "Large Random Graphs with vertex size 12",
    []() {
      return GenerateRandomGraph(12, 0.5, 4);
    },
    20
  );

  std::cout << "Benchmarks completed\n\n\n\n";
}
