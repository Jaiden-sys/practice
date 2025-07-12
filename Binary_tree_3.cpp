#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};


TreeNode* insert(TreeNode* root, int val) {
    if (!root)
        return new TreeNode(val);
    if (val < root->val)
        root->left = insert(root->left, val);
    else
        root->right = insert(root->right, val);
    return root;
}


TreeNode* buildTree(const std::vector<int>& values) {
    TreeNode* root = nullptr;
    for (int val : values) {
        root = insert(root, val);
    }
    return root;
}


bool areIdentical(TreeNode* a, TreeNode* b) {
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;
    return a->val == b->val &&
        areIdentical(a->left, b->left) &&
        areIdentical(a->right, b->right);
}


std::vector<int> readFromFile(const std::string& filename) {
    std::vector<int> values;
    std::ifstream file(filename);
    int number;
    while (file >> number) {
        values.push_back(number);
    }
    return values;
}


std::vector<int> readFromInput() {
    std::vector<int> values;
    std::cout << "Введите числа через пробел (затем Enter): ";
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    int number;
    while (ss >> number) {
        values.push_back(number);
    }
    return values;
}


void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::vector<int> fileValues = readFromFile("input.txt");
    TreeNode* treeFromFile = buildTree(fileValues);

    std::vector<int> inputValues = readFromInput();
    TreeNode* treeFromInput = buildTree(inputValues);


    if (areIdentical(treeFromFile, treeFromInput)) {
        std::cout << "Деревья одинаковы.\n";
    }
    else {
        std::cout << "Деревья различаются.\n";
    }


    deleteTree(treeFromFile);
    deleteTree(treeFromInput);

    return 0;
}
