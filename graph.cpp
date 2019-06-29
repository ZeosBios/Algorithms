#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

//	матрица весов графа
vector<vector<int>> graph;

//	количество вершин в графе
int N;

bool *used;

void read_graph(FILE *input)
{
	if (input == NULL)
	{
		printf("Could not read input file\n");
		return;
	}
	fscanf(input, "%d", &N);
	graph.resize(N);
	used = new bool[N];
	for (int i = 0; i < N; i++)
		used[i] = false;
	for (int i = 0; i < N; i++)
	{
		graph[i].resize(N);
		for (int j = 0; j < N; j++)
			fscanf(input, "%d", &graph[i][j]);
	}
}

//	обход в глубину
void dfs(int start)
{
	used[start] = true;
	for (int i = 0; i < N; i++)
	{
		if (graph[start][i] && !used[i])
			dfs(i);
	}
}

int main(int argc, char *argv[])
{
	FILE *in;
	in = fopen("input.txt", "r");
	read_graph(in);
	// поиск компонент связности
	int conn_comp = 1, index = 0;
	bool fin = false;
	do
	{
		dfs(index);
		int i = 0;
		for (i = 0; i < N; i++)
			if (!used[i])
			{
				index = i;
				fin = false;
				conn_comp++;
				break;
			}
		fin = i == N;
	} while (!fin);
	fclose(in);
	if (in)
		in = NULL;
	printf("Number of connectivity components = %d\n", conn_comp);
	system("pause");
	return EXIT_SUCCESS;
}