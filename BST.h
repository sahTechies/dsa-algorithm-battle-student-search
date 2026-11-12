#ifndef BST_H
#define BST_H

#include "Student.h"
#include <algorithm>
#include <iostream>

// Node representation for Binary Search Tree
struct BSTNode {
    Student data;
    BSTNode* left;
    BSTNode* right;

    explicit BSTNode(const Student& s)
        : data(s), left(nullptr), right(nullptr) {}
};

// Binary Search Tree (Unbalanced) Implementation
class BST {
private:
    BSTNode* root;
    int nodeCount;

    // Recursive helper for insertion
    BSTNode* insertHelper(BSTNode* node, const Student& s) {
        if (node == nullptr) {
            nodeCount++;
            return new BSTNode(s);
        }
        if (s.regNo < node->data.regNo) {
            node->left = insertHelper(node->left, s);
        } else if (s.regNo > node->data.regNo) {
            node->right = insertHelper(node->right, s);
        } else {
            // Key already exists: update record
            node->data = s;
        }
        return node;
    }

    // Recursive helper for calculating tree height
    int calculateHeight(BSTNode* node) const {
        if (node == nullptr) return 0;
        return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
    }

    // In-order traversal helper (yields sorted keys)
    void inorderHelper(BSTNode* node) const {
        if (node == nullptr) return;
        inorderHelper(node->left);
        node->data.display();
        inorderHelper(node->right);
    }

    // Destructor helper to prevent memory leaks
    void destroyTree(BSTNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr), nodeCount(0) {}

    ~BST() {
        destroyTree(root);
        root = nullptr;
    }

    // Disallow copy to avoid shallow copy leaks
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    // Insert student record
    void insert(const Student& s) {
        root = insertHelper(root, s);
    }

    // Search student record by regNo, tracking exact key comparisons
    const Student* search(int regNo, int& comparisons) const {
        comparisons = 0;
        BSTNode* curr = root;
        while (curr != nullptr) {
            comparisons++;
            if (curr->data.regNo == regNo) {
                return &(curr->data);
            } else if (regNo < curr->data.regNo) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return nullptr;
    }

    // Returns the height of the BST (0 for empty tree, 1 for root-only)
    int getHeight() const {
        return calculateHeight(root);
    }

    // Print all records in ascending order of regNo
    void inorderTraversal() const {
        inorderHelper(root);
    }

    // Returns total nodes in BST
    int size() const {
        return nodeCount;
    }

    bool empty() const {
        return root == nullptr;
    }
};

#endif // BST_H

