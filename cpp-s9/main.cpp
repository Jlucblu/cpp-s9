//#include "T1L2.h"
//#include "T1L3.h"
//#include "T1L4.h"
//#include "T1L5.1.h"
//#include "T1L5.2.h"
//#include "T1L6.h"
//#include "T1L7.1.h"
//#include "T1L7.2.h"

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

	if (node == nullptr)
	{
		return true;
	}

	if (!CheckTreeProperty(node->left, min, max))
	{
		return false;
	}

	if (!CheckTreeProperty(node->right, min, max))
	{
		return false;
	}

	min = &node->left->value;
	max = &node->right->value;

	if (min == nullptr && max == nullptr)
	{
		return true;
	}

	if (min != nullptr && max != nullptr)
	{
		if (*min >= *max)
		{
			return false;
		}
	}

	if (min == nullptr && max != nullptr)
	{
		if (node->value <= *max)
		{
			return false;
		}
		return true;

	}

	if (min != nullptr && max == nullptr)
	{
		if (*min >= node->value)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (*min >= node->value && node->value <= *max)
	{
		return false;
	}
	else
	{
		return true;
	}
}

    /*if (node != nullptr) {
        if (!CheckTreeProperty(node->left, min, max)) {
            return false;
        }

        if (!CheckTreeProperty(node->right, min, max)) {
            return false;
        }

        min = &node->left->value;
        max = &node->right->value;

        // проверяем неравенство
        if ((*min > node->value) && (node->value < *max)) {
            // идем влево
            if (node->left != nullptr) {
                CheckTreeProperty(node->left, min, max);
            } 
            // идем вправо
            else {
                CheckTreeProperty(node->right, min, max);
            }
        }
        // возвращаем false если утверждение не верно
        else {
            return false;
        }
    }

    return true;
    
}*/

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

int main() {
    using T = TreeNode<int>;
    T* root1 = new T{ 6,
        new T{4, new T{3}, new T{5}}, new T{7} };
    assert(CheckTreeProperty(root1));

    T* root2 = new T{ 6,
        new T{4, new T{3}, new T{5}}, new T{7, new T{8}} };
    assert(!CheckTreeProperty(root2));

    DeleteTree(root1);
    DeleteTree(root2);
}