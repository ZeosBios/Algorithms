#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

//	структура данных - ребро графа (откуда, куда, вес)
struct edge
{
	int start;
	int fin;
	int weight;

	bool operator<(const edge &e)
	{
		return (weight < e.weight);
	}
};

//	список весов графа
vector<edge> graph;

//	список смежности графа
vector<int> *list_of_edges;

//	количество вершин графа
int N;

//	количество ребер графа
int M;

//	массив, содержащий сведения о том, какие вершины были посещены
bool *used;

vector<int> top_sort;

//	чтение списка весов из файла
void read_graph(FILE *in)
{
	if (in == NULL)
	{
		printf("Could not read input file\n");
		return;
	}
	fscanf(in, "%d", &N);
	fscanf(in, "%d", &M);
	graph.resize(M);
	edge e;
	for (int i = 0; i < M; i++)
	{
		fscanf(in, "%d %d %d", &e.start, &e.fin, &e.weight);
		graph[i] = e;
	}
}

//	получение списка ребер графа по его матрице весов
void construct_list_of_edges(FILE *in)
{
	if (in == NULL)
	{
		printf("Could not read input file\n");
		return;
	}
	fscanf(in, "%d", &N);
	edge e;
	graph.clear();
	for (int i = 0; i < N; i++)
		for (int j = 0; j <= i; j++)
		{
			fscanf(in, "%d", &e.weight);
			if (e.weight != 0)
			{
				e.start = j;
				e.fin = i;
				graph.push_back(e);
			}
		}
	M = graph.size();
	printf("%d\n", M);
}

//	чтение списка смежности ориентированного графа из файла
void get_list_of_edges(FILE *in)
{
	if (in == NULL)
	{
		printf("Could not read input file\n");
		return;
	}
	fscanf(in, "%d", &N);
	fscanf(in, "%d", &M);
	used = new bool[N];
	list_of_edges = new vector<int>[N];
	for (int i = 0; i < N; i++)
		list_of_edges[i].clear();
	for (int i = 0; i < M; i++)
	{
		int a, b;
		fscanf(in, "%d %d", &a, &b);
		list_of_edges[a].push_back(b);
	}
}

//	обход в глубину ориентированного графа
//	используется в методе топологической сортировки
void dfs(int v)
{
	used[v] = true;
	for (int i = 0; i < list_of_edges[v].size(); i++)
	{
		int to = list_of_edges[v][i];
		if (!used[to])
			dfs(to);
	}
	top_sort.push_back(v);
}

//	топологическая сортировка ориентированного графа
void topological_sort()
{
	for (int i = 0; i < N; i++)
		used[i] = false;
	top_sort.clear();
	for (int i = 0; i < N; i++)
		if (!used[i])
			dfs(i);
	reverse(top_sort.begin(), top_sort.end());
	//	вывод новой нумерации
	printf("List of vertices, topologically ordered:\n");
	for (int i = 0; i < top_sort.size(); i++)
		printf("%d ", top_sort[i]);
	printf("\n");
}

//	алгоритм Краскала
//	используется для поиска остова минимального веса в графе, заданном списком ребер
void kruskal()
{
	//	суммарный вес минимального остова
	int sum = 0;
	vector<pair<int, int>> mst;
	mst.clear();
	//	для корректной работы алгоритма все ребра графа должны быть отсортированы по весу
	sort(graph.begin(), graph.end());
	//	для поиска остова МАКСИМАЛЬНОГО веса раскомментируйте строку ниже
	//reverse(graph.begin(), graph.end());
	vector<int> tree_id(N);
	for (int i = 0; i < N; i++)
		tree_id[i] = i;
	//	обход списка ребер методом Краскала
	for (int i = 0; i < M; i++)
	{
		int a = graph[i].start, b = graph[i].fin, w = graph[i].weight;
		if (tree_id[a] != tree_id[b])
		{
			sum += w;
			mst.push_back(make_pair(a, b));
			int old_id = tree_id[b], new_id = tree_id[a];
			for (int j = 0; j < N; j++)
				if (tree_id[j] == old_id)
					tree_id[j] = new_id;
		}
	}
	//	вывод минимального остова
	for (int i = 0; i < mst.size(); i++)
		printf("%d -> %d\n", mst[i].first, mst[i].second);
	printf("MST weight = %d\n", sum);
	mst.clear();
	tree_id.clear();
}

//	алгоритм Форда-Беллмана
//	используется для поиска маршрутов минимального веса в графах, содержащих, в т.ч., ребра отрицательного веса
void ford_bellman(int v, int to)
{
	int i = 0, *dist = new int[N], *p = new int[N];
	for (; i < N; i++)
	{
		dist[i] = INT_MAX / 2;
		p[i] = -1;
	}
	dist[v] = 0;
	for (i = 0; i < N - 1; i++)
		for (int j = 0; j < M; j++)
			//if (dist[graph[j].start] < INT_MAX)
			{
				dist[graph[j].fin] = min(dist[graph[j].fin], dist[graph[j].start] + graph[j].weight);
				p[graph[j].fin] = graph[j].start;
			}
	//	вывод массива расстояний
	for (i = 0; i < N; i++)
		printf("%d ", dist[i]);
	printf("\n");
	//	восстановление кратчайшего маршрута
	if (dist[to] == INT_MAX / 2)
		printf("No path from %d to %d\n", v, to);
	else
	{
		vector<int> path; path.clear();
		for (int cur = to; cur != -1; cur = p[cur])
			path.push_back(cur);
		reverse(path.begin(), path.end());
		printf("Path from %d to %d:\n", v, to);
		for (i = 0; i < path.size(); i++)
			printf("%d ", path[i]);
		path.clear();
	}
	delete[] dist;
	if (dist)
		dist = NULL;
}

//	вывод на экран матрицы весов графа - для проверки
void print_w_matrix()
{
	int **res = new int*[N];
	for (int i = 0; i < N; i++)
	{
		res[i] = new int[N];
		for (int j = 0; j < N; j++)
			res[i][j] = 0;
	}
	for (int i = 0; i < M; i++)
	{
		res[graph[i].start][graph[i].fin] = res[graph[i].fin][graph[i].start] = graph[i].weight;
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d ", res[i][j]);
		printf("\n");
	}
	delete[] res;
	if (res)
		res = NULL;
}

int main(int argc, char *argv[])
{
	FILE *input;
	input = fopen("input.txt", "r");
	//read_graph(input);
	//print_w_matrix();
	//kruskal();
	//construct_list_of_edges(input);
	//ford_bellman(0, 14);
	construct_list_of_edges(input);
	kruskal();
	//topological_sort();
	system("pause");
	return EXIT_SUCCESS;
}