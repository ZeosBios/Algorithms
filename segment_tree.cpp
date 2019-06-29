#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <algorithm>

using namespace std;

//	максимальное число, которое может храниться в дереве
#define INF 1000000000

//	максимальное допустимое количество элементов
#define TREE_DATA 1024

int tree_data[TREE_DATA * 2];

//	инициализация
void init()
{
	for (int i = 0; i < TREE_DATA * 2; i++)
		tree_data[i] = -INF;
}

int _find_max(int p, int l, int r, int a, int b)
{
	if (b < l || r < a) 
		return -INF;
	if (a <= l && r <= b) 
		return tree_data[p];
	// опрос левого потомка
	int r1 = _find_max(p * 2, l, (l + r) / 2, a, b);
	// опрос правого потомка
	int r2 = _find_max(p * 2 + 1, (l + r) / 2 + 1, r, a, b);
	// нахождение большего из левого и правого поддеревьев
	return max(r1, r2);
}

//	поиск максимума на отрезке [a; b]
int find_max(int a, int b)
{
	return _find_max(1, 0, TREE_DATA - 1, a, b);
}

//	записать в элемент №p значение x
void set_element(int p, int x)
{
	p += TREE_DATA;
	tree_data[p] = x;
	for (p /= 2; p; p /= 2)
	{
		if (tree_data[p * 2] > tree_data[p * 2 + 1])
			tree_data[p] = tree_data[p * 2];
		else tree_data[p] = tree_data[p * 2 + 1];
	}
}

int main(int argc, char *argv[])
{
	init();
	char c;
	while (true)
	{
		scanf("%c", &c);
		// выход
		if (c == 'Q') return 0;
		// найти максимум на отрезке [a; b]
		if (c == 'F') 
		{
			int a, b;
			scanf("%d%d", &a, &b);
			printf("%d\n", find_max(a, b));
		}
		// установить значение элемента p равым x
		if (c == 'U')
		{
			int p, x;
			scanf("%d%d", &p, &x);
			set_element(p, x);
		}
	}
	return EXIT_SUCCESS;
}