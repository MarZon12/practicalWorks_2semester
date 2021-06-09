#pragma once
#include <iostream>

class BinaryTree
{
public:
    BinaryTree() {
        mainParent = nullptr;
        treeSize = 0;
    }
    
    ~BinaryTree() {
        delete mainParent;
    }


	struct Node
	{
        Node() {
            value = 0;

            left = nullptr;
            right = nullptr;
        }
        Node(int value) {
            this->value = value;

            left = nullptr;
            right = nullptr;
        }
        Node(int value, Node* left, Node* right) {
            this->value = value;

            this->left = left;
            this->right = right;
        }
        Node(const Node& node) {
            value = node.value;

            left = node.left;
            right = node.right;
        }
        Node(const Node* node) {
            value = node->value;

            left = node->left;
            right = node->right;
        }
        
        ~Node() {
        }

        Node& operator=(Node& node) {
            value = node.value;

            left = node.left;
            right = node.right;
            return *this;
        }

		int value;

        Node* left;
		Node* right;
	};


    Node* search(int value) {
        return search(value, mainParent);
    }

    Node search(int value) const {
        return *search(value, mainParent);
    }

    void printTree(bool useNull) {
        printTree(mainParent, 1, useNull);
    };

    void erase(int value) {
        eraseNode(value);
    }

    void eraseTree()
    {
        eraseTree(mainParent);
        mainParent = nullptr;
    }

    BinaryTree& insert(int value) {
        if (mainParent == nullptr) {
            mainParent = new Node(value);
            return *this;
        }

        Node* tmpNode(search(value, mainParent));

        if (tmpNode == nullptr) {
            insert(value, mainParent);
        }

        return *this;
    }

    long long size() {
        return treeSize;
    }


    void preOrderTravers() {
        preOrderTravers(mainParent);
        std::cout << std::endl;
    }

    void inOrderTravers() {
        inOrderTravers(mainParent);
        std::cout << std::endl;
    }

    void postOrderTravers() {
        postOrderTravers(mainParent);
        std::cout << std::endl;
    }

private:
	Node* mainParent;

    mutable long long treeSize;


    Node* search(int value, Node* root) const {
        if (root != nullptr) {
            if (value == root->value) {
                return root;
            }

            if (value < root->value) {
                return search(value, root->left);
            }
            else 
            {
                return search(value, root->right);
            }
        }

        return nullptr;
    }

    void insert(int value, Node* root) {
        if (value < root->value) {
            if (root->left != nullptr) {
                insert(value, root->left);
            }
            else
            {
                root->left = new Node(value);
                treeSize++;
            }
        }

        if (value > root->value) {
            if (root->right != nullptr) {
                insert(value, root->right);
            }
            else
            {
                root->right = new Node(value);
                treeSize++;
            }
        }
    }


    void printTree(Node* root, int level, bool useNull)
    {
        if (root != nullptr)
        {
            printTree(root->right, level + 1, useNull);

            for (int i = 0; i < level; i++) {
                if (i == level - 1)
                {
                    std::cout << "+-->";
                }
                else
                {
                    std::cout << "\t";
                }
            }

            std::cout << root->value;
            
            if (level == 1)
            {
                for (int i = 0; i < 25; i++) {
                    std::cout << "   -";
                }
                std::cout << std::endl;
            }
            else
            {
                std::cout << std::endl;
            }
            
            printTree(root->left, level + 1, useNull);
        }
        else
        {
            if (useNull)
            {
                for (int i = 0; i < level; i++) {
                    if (i == level - 1)
                    {
                        std::cout << "+-->";
                    }
                    else
                    {
                        std::cout << "\t";
                    }
                }

                std::cout << "NULL" << std::endl;
            }
        }
    }


    void eraseNode(int key)
    {
        Node* leaf = mainParent;
        Node* root = nullptr;

        while (leaf != nullptr && leaf->value != key)
        {
            root = leaf;
            if (leaf->value > key)
            {
                leaf = leaf->left;
            }
            else
            {
                leaf = leaf->right;
            }
        }

        if (leaf == nullptr)
            return;

        if (leaf->left == nullptr)
        {
            if (root != nullptr)
            {
                if (root->left == leaf)
                    root->left = leaf->right;

                if (root->right == leaf)
                    root->right = leaf->right;
            }
            else
            {
                if (leaf->right != nullptr)
                    mainParent = leaf->right;
                else
                    mainParent = nullptr; // WARNING: FULL DESTROY
            }
            treeSize--;
            delete leaf;

            return;
        }

        if (leaf->right == nullptr)
        {
            if (root != nullptr)
            {
                if (root->left == leaf)
                    root->left = leaf->left;

                if (root->right == leaf)
                    root->right = leaf->left;
            }
            else
            {
                if (leaf->left != nullptr)
                    mainParent = leaf->left;
                else
                    mainParent = nullptr; // WARNING: FULL DESTROY
            }
            treeSize--;
            delete leaf;

            return;
        }

        Node* tmpNode = leaf->right;

        while (tmpNode->left)
            tmpNode = tmpNode->left;

        int tmpValue = tmpNode->value;
        eraseNode(tmpValue);

        leaf->value = tmpValue;
    }

    void eraseTree(Node* root)
    {
        if (root != nullptr)
        {
            eraseTree(root->left);
            eraseTree(root->right);
            delete root;
        }
    }


    void preOrderTravers(Node* root) {
        if (root != nullptr) {
            std::cout << root->value << ' ';
            preOrderTravers(root->left);
            preOrderTravers(root->right);
        }
    }

    void inOrderTravers(Node* root) {
        if (root != nullptr) {
            inOrderTravers(root->left);
            std::cout << root->value << ' ';
            inOrderTravers(root->right);
        }
    }

    void postOrderTravers(Node* root) {
        if (root != nullptr) {
            postOrderTravers(root->left);
            postOrderTravers(root->right);
            std::cout << root->value << ' ';
        }
    }
};
