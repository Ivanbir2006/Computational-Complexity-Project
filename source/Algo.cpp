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

Graph GetMergedTerminals(const Graph& in_graph, Vertex excluded, const std::set<Vertex>& terminals) {
  std::vector<Edge> out_graph;
  const Vertex new_terminal = in_graph.GetVertexSize();

  for (const Edge& edge : in_graph.edges_) {
    Vertex v_1 = terminals.contains(edge.v1_) ? new_terminal : edge.v1_;
    Vertex v_2 = terminals.contains(edge.v2_) ? new_terminal : edge.v2_;

    if (v_1 != v_2) {
      out_graph.push_back({v_1, v_2, edge.w_});
      out_graph.push_back({v_2, v_1, edge.w_});
    }
  }

  Graph merged_graph(
    std::move(out_graph),
    {excluded, new_terminal},
    in_graph.GetVertexSize() + 1
  );

  return std::move(merged_graph);
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
