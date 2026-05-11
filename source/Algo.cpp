#include "Dinic.hpp"
#include "Items.hpp"

#include <iostream>
#include <queue>
#include <unordered_map>

using namespace GraphItems;
using GraphItems::Graph;

std::pair<Vertex, Vertex> GetSourceAndSink(const std::set<Vertex>& terminals) {
  if (terminals.size() != 2) {
    throw "Wrong size";
  }

  auto term_it = terminals.cbegin();
  Vertex source = *term_it;
  ++term_it;
  Vertex sink = *term_it;

  return {source, sink};
}

Graph ReverseEdges(const Graph& graph, Vertex source, Vertex sink) {
  size_t n = graph.GetVertexSize();

  std::vector<int> level(n, -1);
  std::queue<Vertex> q;

  level[source] = 0;
  q.push(source);

  while (!q.empty()) {
    Vertex v = q.front();
    q.pop();

    for (const Edge& e : graph.edges_) {
      if (e.v1_ == v && level[e.v2_] == -1) {
        level[e.v2_] = level[v] + 1;
        q.push(e.v2_);
      } 
      else if (e.v2_ == v && level[e.v1_] == -1) {
        level[e.v1_] = level[v] + 1;
        q.push(e.v1_);
      }
    }
  }

  std::vector<Edge> new_edges;
  new_edges.reserve(graph.edges_.size());

  for (const Edge& e : graph.edges_) {
    Vertex u = e.v1_;
    Vertex v = e.v2_;

    if (level[u] == -1 || level[v] == -1) continue;

    if (level[u] < level[v]) {
      new_edges.push_back({u, v, e.w_});
    } else if (level[v] < level[u]) {
      new_edges.push_back({v, u, e.w_});
    }
  }

  std::set<Vertex> new_terminals = {source, sink};

  return {
    std::move(new_edges),
    std::move(new_terminals),
    n
  };
}

Graph SqueezeCords(const Graph& graph) {
  std::unordered_map<Vertex, Vertex> to_new_cords;

  Vertex next_id = 0;

  for (const Edge& edge : graph.edges_) {
    if (!to_new_cords.contains(edge.v1_)) {
      to_new_cords[edge.v1_] = next_id++;
    }

    if (!to_new_cords.contains(edge.v2_)) {
      to_new_cords[edge.v2_] = next_id++;
    }
  }

  for (Vertex terminal : graph.terminals_) {
    if (!to_new_cords.contains(terminal)) {
      to_new_cords[terminal] = next_id++;
    }
  }

  std::vector<Edge> new_edges;
  new_edges.reserve(graph.edges_.size());

  for (const Edge& edge : graph.edges_) {
    new_edges.push_back({
      to_new_cords[edge.v1_],
      to_new_cords[edge.v2_],
      edge.w_
    });
  }

  std::set<Vertex> new_terminals;

  for (Vertex terminal : graph.terminals_) {
    new_terminals.insert(to_new_cords[terminal]);
  }

  return {
    std::move(new_edges),
    std::move(new_terminals),
    static_cast<size_t>(next_id)
  };
}

Graph GetMergedTerminals(const Graph& in_graph, Vertex excluded, const std::set<Vertex>& terminals) {
  std::vector<Edge> out_graph;
  const Vertex new_terminal = in_graph.GetVertexSize();

  for (const Edge& edge : in_graph.edges_) {
    Vertex v_1 = terminals.contains(edge.v1_) ? new_terminal : edge.v1_;
    Vertex v_2 = terminals.contains(edge.v2_) ? new_terminal : edge.v2_;

    if (v_1 != v_2) {
      out_graph.push_back({v_1, v_2, edge.w_});
    }
  }

  Graph merged_graph(
    std::move(out_graph),
    {excluded, new_terminal},
    in_graph.GetVertexSize() + 1
  );

  Graph squeezed_graph = SqueezeCords(merged_graph);
  auto [source, sink] = GetSourceAndSink(squeezed_graph.terminals_);

  return ReverseEdges(squeezed_graph, source, sink);
}

int Graph::GetMinimumMultiCut() const {
  if (!is_defined_) {
    throw "You must define vertex size before";
  }

  if (terminals_.size() < 2) {
    throw "No minimum multicut for less than 2 terminals";
  }

  auto terminals = terminals_;
  std::vector<int> max_flows_;

  for (auto it = terminals_.cbegin(); it != terminals_.cend(); ++it) {
    Vertex excluded = *it;
    terminals.erase(excluded);

    Graph merged_terminals_graph = GetMergedTerminals(*this, excluded, terminals);
    terminals.insert(excluded);

    auto [source, sink] = GetSourceAndSink(merged_terminals_graph.terminals_);  
    Dinic::Dinic DinicGraph(merged_terminals_graph, source, sink);

    max_flows_.push_back(DinicGraph.MaxFlow());
  }

  int sum = 0;
  int max_value = 0;

  for (int max_flow : max_flows_) {
    if (max_value <= max_flow) {
      sum += max_value;
      max_value = max_flow;
    } else {
      sum += max_flow;
    }
  }

  return sum;
}

namespace GraphItems {
  void Graph::Print() const {
    std::cout << "Edges count: " << GetEdgesSize() << '\n';
    std::cout << "Vertex count: " << GetVertexSize() << '\n';
    std::cout << "Terminals count: " << terminals_.size() << "\n";

    std::cout << "\n=========Terminals=========\n";

    for (auto t : terminals_) {
      std::cout << t << " ";
    }

    std::cout << "\n=========Edges=========\n";

    for (auto e : edges_) {
      std::cout << e.v1_ << " " << e.v2_ << " " << e.w_ << '\n';
    }
  }
} // namespace GraphItems
