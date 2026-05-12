#include "Items.hpp"
#include <iostream>

#ifndef DEBUG_ON
#include "Dinic.hpp"
#endif

using namespace GraphItems;

int main() {
  Graph g1({{0, 1, 5}, {0, 2, 3}, {1, 2, 2}}, {0, 1}, 3);
  std::cout << "Minimum multicut -> " << g1.GetMinimumMultiCut() << "\n";

  Graph g({{0, 3, 1}, {1, 3, 2}, {2, 3, 3}, {0, 3, 1}}, {0, 1, 2}, 4);
  std::cout << "Minimum multicut ->" << " " << g.GetMinimumMultiCut() << "\n";
}