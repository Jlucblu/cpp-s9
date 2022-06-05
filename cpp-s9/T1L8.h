#pragma once
#include <cassert>

template <typename T>
struct TreeNode {
	T value;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
};

template <typename T>
void DeleteTree(TreeNode<T>* node) {
	if (!node) {
		return;
	}
	DeleteTree(node->left);
	DeleteTree(node->right);
	delete node;
}


//Возвращает true, если узел node и его дочерние элементы находятся в диапазоне [min, max]
template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) {
	if (node == nullptr) {
		return true;
	}

	if (node->left == nullptr && node->right == nullptr) {
		return true;
	}

	if (node->left != nullptr) {
		min = &node->left->value;
	}

	if (node->right != nullptr) {
		max = &node->right->value;
	}

	if (node->left != nullptr) {
		if (node->left->right != nullptr) {
			if (node->left->right->value > node->value) {
				return false;
			}
		}
	}

	if (node->right != nullptr) {
		if (node->right->left != nullptr) {
			if (node->right->left->value < node->value) {
				return false;
			}
		}
	}

	if (*min >= *max) {
		return false;
	}
	else {
		CheckTreeProperty(node->left, min, max) && CheckTreeProperty(node->right, min, max);
	}
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
	return CheckTreeProperty<T>(node, nullptr, nullptr);
}

void TestTree() {
	using T = TreeNode<int>;
	T* root1 = new T{ 6,
		new T{4, new T{3}, new T{5}}, new T{7} };
	assert(CheckTreeProperty(root1));

	T* root2 = new T{ 6,
		new T{4, new T{3}, new T{5}}, new T{7, new T{8}} };
	assert(!CheckTreeProperty(root2));

	T* root3 = new T{ 8, new T{6 , new T{5}, new T{77}}, new T{10, new T{9}, new T{12}} };
	assert(!CheckTreeProperty(root3));

	DeleteTree(root1);
	DeleteTree(root2);
	DeleteTree(root3);
}