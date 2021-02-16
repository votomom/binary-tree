#include <memory>

struct tree_node {
  int value;
  tree_node *up;
  std::unique_ptr<tree_node> left, right;
};

struct tree {
  std::unique_ptr<tree_node> root;

  auto insert(int val) -> tree_node *;
  auto remove(int val) -> bool;
};