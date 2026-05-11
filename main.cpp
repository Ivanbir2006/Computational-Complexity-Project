#include "Items.hpp"
#include <iostream>

#ifndef DEBUG_ON
#include "Dinic.hpp"
#endif

using namespace GraphItems;

int main() {
  Graph g({{0, 3, 1}, {1, 3, 2}, {2, 3, 3}, {0, 3, 1}}, {0, 1, 2}, 4);
  std::cout << g.GetMinimumMultiCut();
}