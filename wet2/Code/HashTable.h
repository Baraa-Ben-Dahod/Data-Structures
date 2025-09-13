#pragma once
#include <memory>
#include <vector>

template <class K, class T>
class HashTable {
private:
    struct Node {
        K key;
        std::shared_ptr<T> data;
        std::shared_ptr<Node> next;
        Node(K k, std::shared_ptr<T> d) : key(k), data(d), next(nullptr) {}
    };

    std::vector<std::shared_ptr<Node>> table;
    size_t num_elements;

    size_t hash(const K& key) const { return key % table.size(); }

    void resize() {
        size_t new_size = table.size() * 2;
        std::vector<std::shared_ptr<Node>> new_table(new_size);

        for (auto& head : table) {
            auto node = head;
            while (node) {
                size_t idx = node->key % new_size;
                auto new_node = std::make_shared<Node>(node->key, node->data);
                new_node->next = new_table[idx];
                new_table[idx] = new_node;
                node = node->next;
            }
        }
        table = std::move(new_table);
    }

public:
    HashTable(size_t initial_size = 4) : table(initial_size), num_elements(0) {}

    std::shared_ptr<T> find(const K& key) const {
        size_t idx = hash(key);
        auto node = table[idx];
        while (node) {
            if (node->key == key) return node->data;
            node = node->next;
        }
        return nullptr;
    }

    void insert(const K& key, std::shared_ptr<T> data) {
        if (num_elements + 1 > table.size()) resize();
        size_t idx = hash(key);
        auto node = table[idx];
        while (node) {
            if (node->key == key) {
                node->data = data;
                return;
            }
            node = node->next;
        }
        auto new_node = std::make_shared<Node>(key, data);
        new_node->next = table[idx];
        table[idx] = new_node;
        ++num_elements;
    }
};
