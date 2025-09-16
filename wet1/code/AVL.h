#ifndef DS_WET1_SPRING2024_AVL_H
#define DS_WET1_SPRING2024_AVL_H

#include <memory>
#include <algorithm>

template<typename Key, typename Value>
class AVLNode {
public:
    Key key;
    std::shared_ptr<Value> value;
    std::shared_ptr<AVLNode<Key, Value>> left;
    std::shared_ptr<AVLNode<Key, Value>> right;
    int height;

    AVLNode(const Key& k, std::shared_ptr<Value> v) 
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template<typename Key, typename Value>
class AVL {
private:
    std::shared_ptr<AVLNode<Key, Value>> root;

    // ---- Utility functions ----
    int getHeight(const std::shared_ptr<AVLNode<Key, Value>>& node) const {
        return node ? node->height : 0;
    }

    int getBalance(const std::shared_ptr<AVLNode<Key, Value>>& node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(std::shared_ptr<AVLNode<Key, Value>>& node) {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    std::shared_ptr<AVLNode<Key, Value>> rightRotate(std::shared_ptr<AVLNode<Key, Value>>& y) {
        auto x = y->left;
        auto T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    std::shared_ptr<AVLNode<Key, Value>> leftRotate(std::shared_ptr<AVLNode<Key, Value>>& x) {
        auto y = x->right;
        auto T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    std::shared_ptr<AVLNode<Key, Value>> balance(std::shared_ptr<AVLNode<Key, Value>>& node) {
        if (!node) return node;
        
        updateHeight(node);
        const int balanceFactor = getBalance(node);

        // Left-heavy
        if (balanceFactor > 1) {
            if (getBalance(node->left) < 0) {
                node->left = leftRotate(node->left);
            }
            return rightRotate(node);
        }

        // Right-heavy
        if (balanceFactor < -1) {
            if (getBalance(node->right) > 0) {
                node->right = rightRotate(node->right);
            }
            return leftRotate(node);
        }

        return node;
    }

    std::shared_ptr<AVLNode<Key, Value>> insertHelper(
        std::shared_ptr<AVLNode<Key, Value>>& node, 
        const Key& key, 
        std::shared_ptr<Value> value) {
        
        if (!node) {
            return std::make_shared<AVLNode<Key, Value>>(key, value);
        }

        if (key < node->key) {
            node->left = insertHelper(node->left, key, value);
        } else if (key > node->key) {
            node->right = insertHelper(node->right, key, value);
        } else {
            // Key already exists, update value
            node->value = value;
            return node;
        }

        return balance(node);
    }

    std::shared_ptr<AVLNode<Key, Value>> findMin(
        const std::shared_ptr<AVLNode<Key, Value>>& node) const 
    {
        auto current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    std::shared_ptr<AVLNode<Key, Value>> removeHelper(
        std::shared_ptr<AVLNode<Key, Value>>& node, 
        const Key& key) {
        
        if (!node) {
            return node;
        }

        if (key < node->key) {
            node->left = removeHelper(node->left, key);
        } else if (key > node->key) {
            node->right = removeHelper(node->right, key);
        } else {
            // Node to be deleted found
            if (!node->left) {
                return node->right;
            } else if (!node->right) {
                return node->left;
            }

            // Node with two children: get inorder successor
            auto successor = findMin(node->right);
            node->key = successor->key;
            node->value = successor->value;
            node->right = removeHelper(node->right, successor->key);
        }

        return balance(node);
    }

    std::shared_ptr<AVLNode<Key, Value>> findHelper(
        const std::shared_ptr<AVLNode<Key, Value>>& node, 
        const Key& key) const {
        
        if (!node || node->key == key) {
            return node;
        }
        
        if (key < node->key) {
            return findHelper(node->left, key);
        }
        
        return findHelper(node->right, key);
    }

public:
    AVL() : root(nullptr) {}
    
    ~AVL() = default;  // Smart pointers handle cleanup automatically

    void insert(const Key& key, std::shared_ptr<Value> value) {
        root = insertHelper(root, key, value);
    }

    void insert(const Key& key, const Value& value) {
        root = insertHelper(root, key, std::make_shared<Value>(value));
    }

    void remove(const Key& key) {
        root = removeHelper(root, key);
    }

    std::shared_ptr<Value> find(const Key& key) const {
        auto node = findHelper(root, key);
        return node ? node->value : nullptr;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    std::shared_ptr<AVLNode<Key, Value>> getSmallest() const {
        if (!root) return nullptr;
        return findMin(root);
    }

    std::shared_ptr<AVLNode<Key, Value>> getBiggest() const {
        if (!root) return nullptr;
        auto current = root;
        while (current->right) {
            current = current->right;
        }
        return current;
    }
};

#endif // DS_WET1_SPRING2024_AVL_H
