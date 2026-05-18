#include <gtest/gtest.h>
#include "Dinic.hpp"

TEST(DinicTest, SingleEdge) {
  Dinic::Dinic d(2, 0, 1, 1);
  d.AddEdge(0, 1, 5, 0);
  EXPECT_EQ(d.MaxFlow(), 5);
}

TEST(DinicTest, TwoParallelEdges) {
  Dinic::Dinic d(2, 0, 1, 2);
  d.AddEdge(0, 1, 3, 0);
  d.AddEdge(0, 1, 4, 1);
  EXPECT_EQ(d.MaxFlow(), 7);
}

TEST(DinicTest, LinearChain) {
  Dinic::Dinic d(3, 0, 2, 2);
  d.AddEdge(0, 1, 3, 0);
  d.AddEdge(1, 2, 7, 1);
  EXPECT_EQ(d.MaxFlow(), 3);
}

TEST(DinicTest, Diamond) {
  Dinic::Dinic d(4, 0, 3, 4);
  d.AddEdge(0, 1, 3, 0);
  d.AddEdge(0, 2, 2, 1);
  d.AddEdge(1, 3, 2, 2);
  d.AddEdge(2, 3, 3, 3);
  EXPECT_EQ(d.MaxFlow(), 4);
}

TEST(DinicTest, ZeroCapacityEdge) {
  Dinic::Dinic d(2, 0, 1, 1);
  d.AddEdge(0, 1, 0, 0);
  EXPECT_EQ(d.MaxFlow(), 0);
}

TEST(DinicTest, DisconnectedGraph) {
  Dinic::Dinic d(4, 0, 3, 1);
  d.AddEdge(0, 1, 10, 0);
  EXPECT_EQ(d.MaxFlow(), 0);
}
