// Простой алгоритм перебора всевозможых разрезов, который находит минимум

#include "Items.hpp"

#include <limits>
#include <set>
#include <unordered_map>

using namespace GraphItems;

double ComputeCutCost(const Graph& g, const std::vector<std::vector<Vertex>>& divided) {
  std::unordered_map<Vertex, size_t> vertexToGroup;

  for (size_t groupId = 0; groupId < divided.size(); ++groupId) {
    for (Vertex v : divided[groupId]) {
      vertexToGroup[v] = groupId;
    }
  }
  
  double totalCutCost = 0.0;
  for (const Edge& e : g.edges_) {
    if (vertexToGroup[e.v1_] != vertexToGroup[e.v2_]) {
      totalCutCost += e.w_;
    }
  }
  return totalCutCost;
}

void RecursivePartition(const Graph& g,
            std::vector<std::vector<Vertex>>& divided,
            const std::vector<Vertex>& verticesToAssign,
            size_t index,
            double& bestCost,
            std::vector<std::vector<Vertex>>& bestPartition) {
  if (index == verticesToAssign.size()) {
    double cost = ComputeCutCost(g, divided);

    if (cost < bestCost) {
      bestCost = cost;
      bestPartition = divided;
    }

    return;
  }
  
  Vertex v = verticesToAssign[index];

  for (size_t groupId = 0; groupId < divided.size(); ++groupId) {
    divided[groupId].push_back(v);
    RecursivePartition(g, divided, verticesToAssign, index + 1, bestCost, bestPartition);
    divided[groupId].pop_back();
  }
}

std::pair<std::vector<std::vector<Vertex>>, double> FindBestPartition(const Graph& g) {
  const size_t k = g.terminals_.size();

  std::vector<std::vector<Vertex>> divided(k);
  
  size_t groupId = 0;
  for (Vertex t : g.terminals_) {
    divided[groupId].push_back(t);
    ++groupId;
  }
  
  std::vector<Vertex> nonTerminals;
  for (Vertex v = 0; v < g.GetVertexSize(); ++v) {
    if (g.terminals_.find(v) == g.terminals_.end()) {
      nonTerminals.push_back(v);
    }
  }
  
  double bestCost = std::numeric_limits<double>::max();
  std::vector<std::vector<Vertex>> bestPartition;
  
  RecursivePartition(g, divided, nonTerminals, 0, bestCost, bestPartition);
  return {bestPartition, bestCost};
}
