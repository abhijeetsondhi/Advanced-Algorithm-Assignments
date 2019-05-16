#include "RBTreeMap.h"


RBTreeMap::RBTreeMap()
{
	root = nullptr;
}
void RBTreeMap::add(string s, vector<string> val)
{
	Node *newNode = new Node(s,val);
	root = addHelper(root,newNode);
	fixTree(root, newNode);
}
Node* RBTreeMap::addHelper(Node* root, Node* newNode)
{
	if (root == nullptr)
	{
		return newNode;
	}
	if (newNode->length <= root->length)
	{
		root->left = addHelper(root->left, newNode);
		root->left->parent = root;
	}
	else if (newNode->length > root->length)
	{
		root->right = addHelper(root->right, newNode);
		root->right->parent = root;
	}
	return root;
}

void RBTreeMap::fixTree(Node *&root, Node *&newNode)
{
	Node *parent_newNode = nullptr;
	Node *grandParent_newNode = nullptr;

	while ((newNode != root) && (!newNode->black) && (!newNode->parent->black))
	{
		parent_newNode = newNode->parent;
		grandParent_newNode = newNode->parent->parent;
		if (parent_newNode == grandParent_newNode->left)
		{
			Node *uncle_newNode = grandParent_newNode->right;
			if (uncle_newNode != nullptr && (!uncle_newNode->black))
			{
				grandParent_newNode->black = false;
				parent_newNode->black = false;
				uncle_newNode->black = false;
				newNode = grandParent_newNode;
			}
			else
			{
				if (newNode == parent_newNode->right)
				{
					rotateLeft(root, parent_newNode);
					newNode = parent_newNode;
					parent_newNode = newNode->parent;
				}

				rotateRight(root, grandParent_newNode);
				swap(parent_newNode->black, grandParent_newNode->black);
				newNode = parent_newNode;
			}
		}
		else
		{
			Node *uncle_newNode = grandParent_newNode->left;
			if ((uncle_newNode != nullptr) && (!uncle_newNode->black))
			{
				grandParent_newNode->black = false;
				parent_newNode->black = true;
				uncle_newNode->black = true;
				newNode = grandParent_newNode;
			}
			else
			{
				if (newNode == parent_newNode->left)
				{
					rotateRight(root, parent_newNode);
					newNode = parent_newNode;
					parent_newNode = newNode->parent;
				}

				rotateLeft(root, grandParent_newNode);
				swap(parent_newNode->black, grandParent_newNode->black);
				newNode = parent_newNode;
			}
		}
	}
	root->black = true;

}

RBTreeMap::~RBTreeMap()
{
	delete root;
}

void RBTreeMap::rotateLeft(Node *&root, Node *&newNode)
{
	Node *newNode_right = newNode->right;
	newNode->right = newNode_right->left;
	if (newNode->right != nullptr)
		newNode->right->parent = newNode;
	newNode_right->parent = newNode->parent;
	if (newNode->parent == nullptr)
		root = newNode_right;
	else if (newNode == newNode->parent->left)
		newNode->parent->left = newNode_right;
	else
		newNode->parent->right = newNode_right;
	newNode_right->left = newNode;
	newNode->parent = newNode_right;
}

void RBTreeMap::rotateRight(Node *&root, Node*&newNode)
{
	Node *newNode_left = newNode->left;
	newNode->left = newNode_left->right;
	if (newNode->left != nullptr)
	{
		newNode->left->parent = newNode;
	}
	newNode_left->parent = newNode->parent;
	if (newNode->parent == nullptr)
		root = newNode_left;
	else if (newNode == newNode->parent->left)
		newNode->parent->left = newNode_left;
	else
		newNode->parent->right = newNode_left;
	newNode_left->right = newNode;
	newNode->parent = newNode_left;
}

void RBTreeMap::inorderHelper(Node *root)
{
	if (root == nullptr)
		return;

	inorderHelper(root->left);
	cout << root->length << "  ";
	inorderHelper(root->right);
}

Node* RBTreeMap::getMax()
{
	Node *current = root;
	while (current->right != nullptr)
		current = current->right;	
	return current;
}

vector<string> RBTreeMap::getInfo(string s, int leng)
{
	Node *current = root;
	return getHelper(current, s, leng);
}

vector<string> RBTreeMap::getHelper(Node *&root, string s, int leng)
{
	if (root == nullptr || (root->key == s))
	{
		return root->info;
	}
	if (root->length < leng)
		return getHelper(root->right, s, leng);
	return getHelper(root->left, s, leng);
}

void RBTreeMap::printTree()
{
	Node *cur = root;
	printHelper(cur);
}

void RBTreeMap::printHelper(Node *&root)
{
	if (root == nullptr)
		return;
	printHelper(root->left);
	cout << root->key << "--" << root->length << "  ";
	printHelper(root->right);
}
