/*
 *	Двоичное дерево поиска (binary search tree) с основным функционалом
 *	Зиновьев С.А., 21.07.2017
 */
#include<iostream>

using namespace std;

class BST
{
	struct node
	{
		int data;
		node* left;
		node* right;
	};

	node* root;

	node* make_empty(node* t)
	{
		if (t == NULL)
			return NULL;
		{
			make_empty(t->left);
			make_empty(t->right);
			delete t;
		}
		return NULL;
	}

	node* insert(int x, node* t)
	{
		if (t == NULL)
		{
			t = new node;
			t->data = x;
			t->left = t->right = NULL;
		}
		else if (x < t->data)
			t->left = insert(x, t->left);
		else if (x > t->data)
			t->right = insert(x, t->right);
		return t;
	}

	node* find_min(node* t)
	{
		if (t == NULL)
			return NULL;
		else if (t->left == NULL)
			return t;
		else
			return find_min(t->left);
	}

	node* find_max(node* t)
	{
		if (t == NULL)
			return NULL;
		else if (t->right == NULL)
			return t;
		else
			return find_max(t->right);
	}

	node* remove(int x, node* t)
	{
		node* temp;
		if (t == NULL)
			return NULL;
		else if (x < t->data)
			t->left = remove(x, t->left);
		else if (x > t->data)
			t->right = remove(x, t->right);
		else if (t->left && t->right)
		{
			temp = find_min(t->right);
			t->data = temp->data;
			t->right = remove(t->data, t->right);
		}
		else
		{
			temp = t;
			if (t->left == NULL)
				t = t->right;
			else if (t->right == NULL)
				t = t->left;
			delete temp;
		}

		return t;
	}

	void inorder(node* t)
	{
		if (t == NULL)
			return;
		inorder(t->left);
		cout << t->data << " ";
		inorder(t->right);
	}

	void prefix(node* t)
	{
		if (t == NULL)
			return;
		cout << t->data << " ";
		prefix(t->left);
		prefix(t->right);
	}

	node* find(node* t, int x)
	{
		if (t == NULL)
			return NULL;
		else if (x < t->data)
			return find(t->left, x);
		else if (x > t->data)
			return find(t->right, x);
		else
			return t;
	}

public:
	BST()
	{
		root = NULL;
	}

	~BST()
	{
		root = make_empty(root);
	}

	void insert(int x)
	{
		root = insert(x, root);
	}

	void remove(int x)
	{
		root = remove(x, root);
	}

	void display()
	{
		inorder(root);
		//prefix(root);
		cout << endl;
	}

	void search(int x)
	{
		root = find(root, x);
	}
};

int main(int argc, char *argv[])
{
	BST tree;
	/*tree.insert(20);
	tree.insert(25);
	tree.insert(15);
	tree.insert(10);
	tree.insert(30);
	tree.display();
	tree.remove(20);
	tree.display();
	tree.remove(25);
	tree.display();
	tree.remove(30);
	tree.display();*/
	system("pause");
	return EXIT_SUCCESS;
}