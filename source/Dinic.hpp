// Алгоритм  Диница для поиска максимального потока, работает за O(V^2 E)
// Подходит и в случае кратных ребер

#pragma once
#include <iostream>
#include <queue>
#include <vector>

namespace Dinic {

class Dinic {
  struct Edge {
    long long to;
    long long capacity;
    long long flow;
    size_t rev;
    bool is_reversed;
    long long id;
  };

  static const long long cMaxValue = 1e18;
  std::vector<std::vector<Edge>> graph_;
  std::vector<long long> deep_;
  std::vector<size_t> ptr_;
  long long source_;
  long long sink_;
  std::vector<long long> edge_flows_;

  bool Bfs() {
    fill(deep_.begin(), deep_.end(), -1);
    std::queue<long long> q;
    q.push(source_);
    deep_[source_] = 0;

    while (!q.empty()) {
      long long u = q.front();
      q.pop();

      for (const Edge& edge : graph_[u]) {
        if (deep_[edge.to] == -1 && edge.flow < edge.capacity) {
          deep_[edge.to] = deep_[u] + 1;
          q.push(edge.to);
        }
      }
    }

    return deep_[sink_] != -1;
  }

  long long Dfs(long long u, long long min_capacity) {
    if (u == sink_) {
      return min_capacity;
    }

    for (; ptr_[u] < graph_[u].size(); ++ptr_[u]) {
      Edge& edge = graph_[u][ptr_[u]];

      if (deep_[edge.to] == deep_[u] + 1 && edge.flow < edge.capacity) {
        long long delta =
            Dfs(edge.to, std::min(min_capacity, edge.capacity - edge.flow));

        if (delta > 0) {
          edge.flow += delta;
          graph_[edge.to][edge.rev].flow -= delta;

          if (!edge.is_reversed) {
            edge_flows_[edge.id] += delta;
          } else {
            edge_flows_[edge.id] -= delta;
          }

          return delta;
        }
      }
    }
  
    return 0;
  }

 public:
  Dinic(long long n, long long s, long long t, long long m)
      : graph_(n), deep_(n), ptr_(n), source_(s), sink_(t), edge_flows_(m) {}
  
  template <typename Graph>
  Dinic(const Graph& graph, long long s, long long t)
      : graph_(graph.GetVertexSize()), 
        deep_(graph.GetVertexSize()),
        ptr_(graph.GetVertexSize()),
        source_(s),
        sink_(t),
        edge_flows_(graph.GetEdgesSize()) {

    size_t counter = 0;
    for (const auto& edge : graph.edges_) {
      AddEdge(edge.v1_, edge.v2_, edge.w_, counter++);
    }
  }

  void AddEdge(long long from, long long to, long long capacity, long long id) {
    graph_[from].push_back({
      to, capacity, 0, graph_[to].size(), false, id
    });

    graph_[to].push_back({
      from, 0, 0, graph_[from].size() - 1, true, id
    });
  }

  long long MaxFlow() {
    long long total_flow = 0;

    while (Bfs()) {
      fill(ptr_.begin(), ptr_.end(), 0);

      while (long long delta = Dfs(source_, cMaxValue)) {
        total_flow += delta;
      }
    }
    return total_flow;
  }

  void PrintFlows() const {
    for (long long flow : edge_flows_) {
      std::cout << flow << "\n";
    }
  }
};

} // namespace Dinic
