#include <memory>

struct tree_node {
    int value;
    tree_node *up;
    std::unique_ptr<tree_node> left, right;

    bool is_leaf() const { return !left && !right; }
    bool is_left() const { return up->left.get() == this; }
    bool is_right() const { return up->right.get() == this; }
};

struct tree {
    std::unique_ptr<tree_node> root;

    auto insert(int val) -> tree_node *;
    auto remove(int val) -> bool;
};
