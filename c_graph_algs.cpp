#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;

//	матрица весов графа
vector<vector<int>> graph;

//	количество вершин графа
int N;

//	массив, содержащий сведения о том, какие вершины графа были посещены
bool *used = new bool[N];

//	чтение матрицы весов графа из файла
void read_graph(FILE *input)
{
	if (input == NULL)
	{
		printf("ERROR - Could not read input file\n");
		return;
	}
	fscanf(input, "%d", &N);
	graph.resize(N);
	for (int i = 0; i < N; i++)
	{
		graph[i].resize(N);
		for (int j = 0; j < N; j++)
		{
			fscanf(input, "%d", &graph[i][j]);
			//	для корректной работы алгоритма Дейкстры раскомментируйте строки ниже
			//if (!graph[i][j])
				//graph[i][j] = INT_MAX;
		}
	}
}

//	поиск в ширину
//	применяется для поиска маршрутов, содержащих наименьшее количество ребер, от выбранной вершины до всех остальных
void bfs(int s, int to)
{
	//	обход в ширину
	queue<int> q;
	fill(&used[0], &used[N], false);
	vector<int> d(N), p(N);
	q.push(s);
	used[s] = true;
	p[s] = -1;
	while (!q.empty()) 
	{
		int v = q.front();
		q.pop();
		for (int i = 0; i < N; i++) 
		{
			if (!used[i] && graph[v][i]) 
			{
				used[i] = true;
				q.push(i);
				d[i] = d[v] + 1;
				p[i] = v;
			}
		}
	}
	// вывод кратчайшего маршрута до вершины to
	/*if (!used[to])
		printf("No way!\n");
	else 
	{
		vector<int> path; path.clear();
		for (int v = to; v != -1; v = p[v])
			path.push_back(v);
		reverse(path.begin(), path.end());
		printf("Path: ");
		for (int i = 0; i < path.size(); i++)
			printf("%d ", path[i]);
		printf("\n");
	}*/
	for (int i = 0; i < N; i++)
		printf("%d ", d[i]);
}

//	обход в глубину
//	применяется для нахождения количества компонент связности в графе
//	ориентированность графа не имеет значения
void dfs(int start)
{
	//	помечаем текущую вершину как посещенную
	used[start] = true;
	//	проходим циклом по всем остальным вершинам
	for (int i = 0; i < N; i++)
		if (graph[start][i] && !used[i])	//	если из текущей вершины есть ребро в другую вершину, которая еще не была посещена
			dfs(i);							//	запускаем этот же алгоритм из этой вершины
}

//	алгоритм Дейкстры
//	применяется для поиска маршрутов минимального веса из выбранной вершины до всех остальных
//	ориентированность графа не имеет значения
//	граф не должен содержать петель, кратных ребер и ребер с отрицательным весом
void dijkstra(int start)
{
	bool *visited = new bool[N];
	int *D = new int[N];
	for (int i = 0; i < N; i++)
	{
		D[i] = INT_MAX;
		visited[i] = false;
	}
	D[start] = 0;
	int index = 0, u = 0;
	for (int i = 0; i < N; i++)
	{
		int min = INT_MAX;
		for (int j = 0; j < N; j++)
		{
			if (!visited[j] && D[j] < min)
			{
				min = D[j];
				index = j;
			}
		}
		u = index;
		visited[u] = true;
		for (int j = 0; j < N; j++)
		{
			if (!visited[j] && graph[u][j] != INT_MAX && D[u] != INT_MAX && (D[u] + graph[u][j] < D[j]))
			{
				D[j] = D[u] + graph[u][j];
			}
		}
	}
	printf("Optimal ways to other vertices:\n");
	for (int i = 0; i < N; i++)
	{
		if (D[i] != INT_MAX)
		{
			printf("%d -> %d = %d\n", start, i, D[i]);
		}
		else
		{
			printf("%d -> %d = no way\n", start, i);
		}
	}
}

int main(int argc, char *argv[])
{
	FILE *in;
	in = fopen("input.txt", "r");
	read_graph(in);
	fill(&used[0], &used[N], false);
	//dijkstra(0);
	printf("\n");
	bfs(0, 4);
	//dfs(0);
	printf("\n");
	system("pause");
	return EXIT_SUCCESS;
}