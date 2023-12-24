#include "tree.hpp"
#include <functional>

auto tree::insert(int val) -> tree_node * {
    auto node = std::unique_ptr<tree_node>(new tree_node{val, nullptr, nullptr, nullptr});
    if (!root) {
        root = std::move(node);
        return root.get();
    }

    auto current = root.get();

    while (true) {
        if (node->value < current->value) {
            if (current->left) {
                current = current->left.get();
            } else {
                node->up = current;
                current->left = std::move(node);
                return current->left.get();
            }
        } else if (node->value > current->value) {
            if (current->right) {
                current = current->right.get();
            } else {
                node->up = current;
                current->right = std::move(node);
                return current->right.get();
            }
        } else {
            return current;
        }
    }
}

auto tree::remove(int val) -> bool {
    auto node = root.get();

    while (node && node->value != val) {
        if (val > node->value) {
            node = node->right.get();
        } else {
            node = node->left.get();
        }
    }
    
    if (!node) {
        return false;
    }

    if (node->is_leaf()) {
        if (node == root.get()) {
            root.reset();
        } else if (node->is_left()) {
            node->up->left.reset();
        } else {
            node->up->right.reset();
        }
        return true;
    }

    std::function<tree_node*(tree_node*)> get_right = [&get_right](tree_node* node) {
        if (node->right) {
            return get_right(node->right.get());
        } else {
            return node;
        }
    };

    std::function<tree_node*(tree_node*)> get_left = [&get_left](tree_node* node) {
        if (node->left) {
            return get_left(node->left.get());
        } else {
            return node;
        }
    };
    if (node == root.get()) {
        if (node->right) {
            auto min = get_left(node->right.get());
            node->value = min->value;
            
            if (min->is_leaf()) {
                if (min->is_left()) {
                    min->up->left = std::move(min->right);
                } else {
                    min->up->right = std::move(min->right);
                }
                return true;
            }

            min->right->up = min->up;

            if (min->is_left()) {
                min->up->left = std::move(min->right);
            } else {
                min->up->right = std::move(min->right);
            }
            return true;
        }
        
        root = std::move(root->left);
        root->up = nullptr;
        return true;
    }
    if (!node->right) {
        node->left->up = node->up;
        if (node->is_left()) {
            node->up->left = std::move(node->left);
        } else {
            node->up->right = std::move(node->left);
        }
        return true;
    }
    if (!node->left) {
        node->right->up = node->up;
        if (node->is_left()) {
            node->up->left = std::move(node->right);
        } else {
            node->up->right = std::move(node->right);
        }
        return true;
    }
    if (node->is_right()) {
        auto min = get_left(node->right.get());
        node->value = min->value;

        if (min->is_leaf()) {
            min->up->left.reset();
            return true;
        }

        min->right->up = min->up;
        min->up->left = std::move(min->right);
        return true;
    }
    if (node->is_left()) {
        auto max = get_right(node->left.get());
        node->value = max->value;

        if (max->is_leaf()) {
            max->up->right.reset();
            return true;
        }

        max->left->up = max->up;
        max->up->right = std::move(max->left);
        return true;
    }

    return false;
}
