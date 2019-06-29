/*длина наибольшей общей подпоследовательности*/
#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>

using namespace std;

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int l_s_c(char *x, char *y, int m, int n)
{
	int **mat = new int*[m + 1];
	for (int j = 0; j < m + 1; j++)
		mat[j] = new int[n + 1];
	int i, j;
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0)
				mat[i][j] = 0;
			else if (x[i - 1] == y[j - 1])
				mat[i][j] = mat[i - 1][j - 1] + 1;
			else
				mat[i][j] = max(mat[i][j - 1], mat[i - 1][j]);
		}
	}
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
			cout << mat[i][j] << " ";
		cout << endl;
	}
	return mat[m][n];
}

int main(int argc, char *argv[])
{
	char x[] = "agcat";//"XMJYAUZ";
	char y[] = "gac";//"MZJAWXU";
	int m = strlen(x);
	int n = strlen(y);
	cout << "Length of the longest common subsequence = " << l_s_c(x, y, m, n) << endl;
	system("pause");
	return EXIT_SUCCESS;
}