#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
using namespace std;

//red black tree structure with key as pattern,value as vector of words with that pattern,black to check if node is black or not
struct Node {
	string key;
		vector<string> info;
		Node *left, *right, *parent;
		bool black;
		int length;

		Node(string data,vector<string> val)
		{
			this->key = data;
			this->info = vector<string>(val);
			this->length = val.size();
			this->left = this->right = this->parent = NULL;

		}
};
class RBTreeMap {

public:
	RBTreeMap();
	~RBTreeMap();
	Node* addHelper(Node* root,Node* newNode);
	//add the key and vector of words in a tree
	void add(string s, vector<string> val);
	//get the node with maximum occurence of the words
	Node* getMax();
	//get a node for a pattern
	vector<string> getInfo(string s,int leng);
	vector<string> getHelper(Node *&root,string s, int leng);
	void printTree();
	void printHelper(Node *&root);
	
private:
	Node *root;	
	//all methods to balance the tree
	void rotateLeft(Node *&, Node *&);
	void rotateRight(Node *&, Node *&);
	void fixTree(Node *&, Node *&);
	void inorderHelper(Node *root);

};