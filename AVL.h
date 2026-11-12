#ifndef AVL_H
#define AVL_H

#include "Student.h"
#include <algorithm>
#include <iostream>

// Node representation for AVL Tree (Height-Balanced Binary Search Tree)
struct AVLNode {
    Student data;
    AVLNode* left;
    AVLNode* right;
    int height;

    explicit AVLNode(const Student& s)
        : data(s), left(nullptr), right(nullptr), height(1) {}
};

// Self-Balancing AVL Tree Implementation
class AVL {
private:
    AVLNode* root;
    int nodeCount;
    int totalRotations;

    // Helper to get height of a node safely
    int nodeHeight(AVLNode* n) const {
        return n ? n->height : 0;
    }

    // Helper to calculate balance factor: Height(Left) - Height(Right)
    int getBalance(AVLNode* n) const {
        return n ? (nodeHeight(n->left) - nodeHeight(n->right)) : 0;
    }

    // Right Rotation (Single rotation for Left-Left imbalance)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));
        x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right));

        totalRotations++;
        return x;
    }

    // Left Rotation (Single rotation for Right-Right imbalance)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right));
        y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));

        totalRotations++;
        return y;
    }

    // Recursive insertion with height recalculation and rotation triggers
    AVLNode* insertHelper(AVLNode* node, const Student& s) {
        // 1. Standard BST insertion
        if (node == nullptr) {
            nodeCount++;
            return new AVLNode(s);
        }

        if (s.regNo < node->data.regNo) {
            node->left = insertHelper(node->left, s);
        } else if (s.regNo > node->data.regNo) {
            node->right = insertHelper(node->right, s);
        } else {
            // Duplicate registration number: update existing record
            node->data = s;
            return node;
        }

        // 2. Update height of ancestor node
        node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));

        // 3. Obtain Balance Factor
        int balance = getBalance(node);

        // 4. Handle 4 Imbalance Scenarios:
        // Case 1: Left-Left (LL) -> Right Rotate
        if (balance > 1 && s.regNo < node->left->data.regNo) {
            return rightRotate(node);
        }

        // Case 2: Right-Right (RR) -> Left Rotate
        if (balance < -1 && s.regNo > node->right->data.regNo) {
            return leftRotate(node);
        }

        // Case 3: Left-Right (LR) -> Left Rotate left child, then Right Rotate node
        if (balance > 1 && s.regNo > node->left->data.regNo) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Case 4: Right-Left (RL) -> Right Rotate right child, then Left Rotate node
        if (balance < -1 && s.regNo < node->right->data.regNo) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // In-order traversal helper (yields sorted output)
    void inorderHelper(AVLNode* node) const {
        if (node == nullptr) return;
        inorderHelper(node->left);
        node->data.display();
        inorderHelper(node->right);
    }

    // Recursive cleanup helper
    void destroyTree(AVLNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    AVL() : root(nullptr), nodeCount(0), totalRotations(0) {}

    ~AVL() {
        destroyTree(root);
        root = nullptr;
    }

    // Disallow copy
    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    // Insert student record and balance automatically
    void insert(const Student& s) {
        root = insertHelper(root, s);
    }

    // Search student record by regNo, tracking exact key comparisons
    const Student* search(int regNo, int& comparisons) const {
        comparisons = 0;
        AVLNode* curr = root;
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

    // Returns the strict height of the balanced AVL tree
    int getHeight() const {
        return nodeHeight(root);
    }

    // Returns the balance factor of the root node
    int getRootBalance() const {
        return getBalance(root);
    }

    // Returns total rotations performed during insertions
    int getTotalRotations() const {
        return totalRotations;
    }

    // Print all records in ascending order of regNo
    void inorderTraversal() const {
        inorderHelper(root);
    }

    // Total records stored
    int size() const {
        return nodeCount;
    }

    bool empty() const {
        return root == nullptr;
    }
};

#endif // AVL_H

