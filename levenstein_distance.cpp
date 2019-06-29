/*Расстояние Левенштейна (расстояние редактирования)*/
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

int d[255][255];

using namespace std;

int main(int argc, char *argv[])
{
	int i, j, m, n, temp, tracker;
	char from[] = "day";
	char to[] = "night";
	m = strlen(from);
	n = strlen(to);

	d[0][0] = 0;
	for (j = 0; j <= n; j++)
		d[0][j] = j;
	for (i = 0; i <= m; i++)
		d[i][0] = i;

	for (i = 1; i <= m; i++) 
	{
		for (j = 1; j <= n; j++) 
		{
			if (from[i - 1] == to[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = min(min(d[i - 1][j] + 1, d[i][j - 1] + 1), d[i - 1][j - 1] + 1);
		}
	}
	/*	Вывод на экран таблицы - для проверки
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
			cout << d[i][j] << " ";
		cout << endl;
	}*/
	cout << "The Levenstein distance between " << from << " and " << to << " = " << d[m][n] << endl;
	system("pause");
	return 0;
}