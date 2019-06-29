#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdarg.h>

//	тип данных, которые хранятся в дереве
typedef int T;
#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)

//	раскраска красно-черного дерева
typedef enum { BLACK, RED } nodeColor;

typedef struct Node_ {
	struct Node_ *left;         //	левый потомок
	struct Node_ *right;        //	правый потомок
	struct Node_ *parent;       //	предшественник
	nodeColor color;            //	цвет узла (красный или черный)
	T data;                     //	данные, которые хранятся в узле
} Node;

#define NIL &sentinel           //	все листья дерева (узлы, не имеющие потомков) - пусты
Node sentinel = { NIL, NIL, 0, BLACK, 0 };

//	корень красно-черного дерева
Node *root = NIL;

//	поворот узла x влево
void rotateLeft(Node *x) 
{
	Node *y = x->right;

	//	связать x с правым потомком
	x->right = y->left;
	if (y->left != NIL) 
		y->left->parent = x;
	//	связать y с предшественником
	if (y != NIL) y->parent = x->parent;
	if (x->parent) 
	{
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	else 
	{
		root = y;
	}
	//	связать x и y
	y->left = x;
	if (x != NIL) x->parent = y;
}

//	поворот узла x вправо
void rotateRight(Node *x) 
{
	Node *y = x->left;
	//	связать x с левым потомком
	x->left = y->right;
	if (y->right != NIL) y->right->parent = x;

	//	связать y с предшественником
	if (y != NIL) y->parent = x->parent;
	if (x->parent) 
	{
		if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
	}
	else 
		root = y;
	//	связать x и y
	y->right = x;
	if (x != NIL) x->parent = y;
}

//	восстановить баланс красно-черного дерева после добавления узла x
void insertFixup(Node *x) 
{
	//	проверка свойства красно-черного дерева
	while (x != root && x->parent->color == RED) 
	{
		//	нарушение свойства
		if (x->parent == x->parent->parent->left) 
		{
			Node *y = x->parent->parent->right;
			if (y->color == RED) 
			{
				//	дядя - красный
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			}
			else 
			{
				//	дядя - черный
				if (x == x->parent->right) 
				{
					//	сделать x левым потомком
					x = x->parent;
					rotateLeft(x);
				}

				//	перекрасить и повернуть узел вправо
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotateRight(x->parent->parent);
			}
		}
		else 
		{
			Node *y = x->parent->parent->left;
			if (y->color == RED) 
			{
				//	дядя - красный
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			}
			else 
			{
				//	дядя - черный
				if (x == x->parent->left) 
				{
					x = x->parent;
					rotateRight(x);
				}
				//	перекрасить и повернуть узел влево
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotateLeft(x->parent->parent);
			}
		}
	}
	root->color = BLACK;
}

//	выделить память и добавить узел в дерево
Node *insertNode(T data) 
{
	Node *current, *parent, *x;
	//	определить место для нового узла
	current = root;
	parent = 0;
	x = new Node;
	while (current != NIL) {
		if (compEQ(data, current->data)) return (current);
		parent = current;
		current = compLT(data, current->data) ? current->left : current->right;
	}
	//	создать новый узел
	x->data = data;
	x->parent = parent;
	x->left = NIL;
	x->right = NIL;
	x->color = RED;

	//	вставить узел в дерево
	if (parent) 
	{
		if (compLT(data, parent->data))
			parent->left = x;
		else
			parent->right = x;
	}
	else {
		root = x;
	}
	insertFixup(x);
	return(x);
}

//	восстановить баланс красно-черного дерева после удаления узла x
void deleteFixup(Node *x) 
{
	while (x != root && x->color == BLACK) 
	{
		if (x == x->parent->left) 
		{
			Node *w = x->parent->right;
			if (w->color == RED) 
			{
				w->color = BLACK;
				x->parent->color = RED;
				rotateLeft(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) 
			{
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) 
				{
					w->left->color = BLACK;
					w->color = RED;
					rotateRight(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rotateLeft(x->parent);
				x = root;
			}
		}
		else {
			Node *w = x->parent->left;
			if (w->color == RED) 
			{
				w->color = BLACK;
				x->parent->color = RED;
				rotateRight(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) 
			{
				w->color = RED;
				x = x->parent;
			}
			else 
			{
				if (w->left->color == BLACK) 
				{
					w->right->color = BLACK;
					w->color = RED;
					rotateLeft(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rotateRight(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

//	удаление указанного узла из дерева
void deleteNode(Node *z) 
{
	Node *x, *y;
	if (!z || z == NIL) 
		return;

	if (z->left == NIL || z->right == NIL) 
	{
		//	y имеет пустого потомка (NIL) 
		y = z;
	}
	else 
	{
		//	найти потомка, у которого, в свою очередь, имеется пустой потомок (NIL) 
		y = z->right;
		while (y->left != NIL) y = y->left;
	}

	//	если x - единственный потомок y
	if (y->left != NIL)
		x = y->left;
	else
		x = y->right;

	//	исключить y из дерева предшественников
	x->parent = y->parent;
	if (y->parent)
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	else
		root = x;

	if (y != z) z->data = y->data;


	if (y->color == BLACK)
		deleteFixup(x);

	free(y);
}

//	найти в дереве узел, содержащий указанные данные
Node *findNode(T data) 
{
	Node *current = root;
	while (current != NIL)
		if (compEQ(data, current->data))
			return (current);
		else
			current = compLT(data, current->data) ? current->left : current->right;
	return(0);
}

/* аргументы командной строки для запуска:
*
*   rbt maxnum
*	например:
*   rbt 20
*       добавить в дерево 20 узлов
*/
int main(int argc, char **argv) 
{
	int a, maxnum, ct;
	Node *t;
	maxnum = atoi(argv[1]);
	for (ct = maxnum; ct; ct--) 
	{
		a = rand() % 9 + 1;
		if ((t = findNode(a)) != NULL) 
		{
			printf("Deleted node %d\n", t->data);
			deleteNode(t);
		}
		else 
		{
			insertNode(a);
			printf("Inserted node %d\n", a);
		}
	}
	return EXIT_SUCCESS;
}