#pragma once

#include <set>
#include <vector>

namespace GraphItems {

using Vertex = long long;

struct Edge {
  const Vertex v1_;
  const Vertex v2_;
  long long w_; // weight 

  Edge(Vertex v1, Vertex v2, long long w) : v1_(v1), v2_(v2), w_(w) {}
};

class Graph {
  size_t vertex_size_;
  bool is_defined_ = false;

  void Print() const;
 public:
  std::vector<Edge> edges_;
  std::set<Vertex> terminals_;

  Graph(std::vector<Edge>&& edges, std::set<Vertex>&& terminals)
    : edges_(std::move(edges)),
      terminals_(std::move(terminals)) {}
  
  Graph(std::vector<Edge>&& edges, std::set<Vertex>&& terminals, size_t v_size)
    : vertex_size_(v_size),
      is_defined_(true),
      edges_(std::move(edges)),
      terminals_(std::move(terminals)) {}
  
  Graph(Graph&&) = default;
  Graph(const Graph&) = delete;

  void SetVertexSize(size_t s) {
    is_defined_ = true;
    vertex_size_ = s;
  }

  size_t GetVertexSize() const {
    if (!is_defined_) {
      throw "Vertex Size is not defined!\n";
    }

    return vertex_size_;
  }

  size_t GetEdgesSize() const {
    return edges_.size();
  }

  int GetMinimumMultiCut() const;
};

} //namespace GraphItems
