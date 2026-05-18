#pragma once

#include "Items.hpp"
#include <vector>

using namespace GraphItems;

std::pair<std::vector<std::vector<Vertex>>, double> FindBestPartition(const Graph&);
double ComputeCutCost(const Graph& g, const std::vector<std::vector<Vertex>>&);
void RecursivePartition(const Graph&, std::vector<std::vector<Vertex>>&,
                        const std::vector<Vertex>&, size_t, double&,
                        std::vector<std::vector<Vertex>>&);