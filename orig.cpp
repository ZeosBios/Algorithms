#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define M 20
#define N 20

static const double EPS = 1.e-8;
int equal(double a, double b) 
{ 
	return fabs(a - b) < EPS; 
}

typedef struct 
{
	int m, n; // m - количество строк, n - количество столбцов
	double mat[M][N];
} table;

void nl(int k) 
{ 
	int j; 
	for (j = 0; j<k; j++) 
		putchar('-'); 
	putchar('\n'); 
}

void print_table(table *tab, const char* mes) 
{
	static int counter = 0;
	int i, j;
	printf("\n%d. Table %s:\n", ++counter, mes);
	nl(70);
	printf("%-6s%5s", "col:", "b[i]");
	for (j = 1; j < tab->n; j++) 
		printf("    x%d,", j); 
	printf("\n");

	for (i = 0; i<tab->m; i++) 
	{
		if (i == 0) 
			printf("max:"); 
		else
			printf("b%d: ", i);
		for (j = 0; j<tab->n; j++) 
		{
			if (equal((int)tab->mat[i][j], tab->mat[i][j]))
				printf(" %6d", (int)tab->mat[i][j]);
			else
				printf(" %6.2lf", tab->mat[i][j]);
		}
		printf("\n");
	}
	nl(70);
}

/* Пример входных данных (файл input)
4 5
0   -0.5  -3 -1  -4
40    1     1  1   1
10   -2    -1  1   1
10    0     1  0  -1
*/
void read_table(table *tab, const char * filename) 
{
	int err, i, j;
	FILE * fp;

	fp = fopen(filename, "r");
	if (!fp) 
	{
		printf("Cannot read %s\n", filename); exit(1);
	}
	memset(tab, 0, sizeof(*tab));
	err = fscanf(fp, "%d %d", &tab->m, &tab->n);
	if (err == 0 || err == EOF) 
	{
		printf("Cannot read m or n\n"); exit(1);
	}
	for (i = 0; i < tab->m; i++) 
	{
		for (j = 0; j < tab->n; j++) 
		{
			err = fscanf(fp, "%lf", &tab->mat[i][j]);
			if (err == 0 || err == EOF) 
			{
				printf("Cannot read A[%d][%d]\n", i, j); 
				exit(1);
			}
		}
	}
	printf("Read table [%d rows x %d columns] from file '%s'.\n", tab->m, tab->n, filename);
	fclose(fp);
}

void pivot_on(table *tab, int row, int col) 
{
	int i, j;
	double pivot;

	pivot = tab->mat[row][col];
	assert(pivot > 0);
	for (j = 0; j < tab->n; j++)
		tab->mat[row][j] /= pivot;
	assert(equal(tab->mat[row][col], 1.));
	// для всех остальных элементов таблицы
	for (i = 0; i < tab->m; i++) 
	{ 
		double multiplier = tab->mat[i][col];
		if (i == row) 
			continue;
		for (j = 0; j < tab->n; j++) 
		{	// r[i] = r[i] - z * r[row];
			tab->mat[i][j] -= multiplier * tab->mat[row][j];
		}
	}
}

// Определяем ведущий столбец - с наибольшим отрицательным значением в строке mat[0][1..n]
int find_pivot_column(table *tab) 
{
	int j, pivot_col = 1;
	double lowest = tab->mat[0][pivot_col];
	for (j = 1; j<tab->n; j++) 
	{
		if (tab->mat[0][j] < lowest) 
		{
			lowest = tab->mat[0][j];
			pivot_col = j;
		}
	}
	printf("Most negative column in row[0] is col %d = %g.\n", pivot_col, lowest);
	// Вся строка mat[0][1..n] положительна - оптимальное решение найдено
	if (lowest >= 0) 
	{
		return -1;
	}
	return pivot_col;
}

// Определение ведущей строки, с наименьшим положительным отношением col[0] / col[pivot]
int find_pivot_row(table *tab, int pivot_col) 
{
	int i, pivot_row = 0;
	double min_ratio = -1;
	for (i = 1; i < tab->m; i++) 
	{
		double ratio = tab->mat[i][0] / tab->mat[i][pivot_col];
		if ((ratio > 0 && ratio < min_ratio) || min_ratio < 0) 
		{
			min_ratio = ratio;
			pivot_row = i;
		}
	}
	if (min_ratio == -1)
		return -1;
	printf("Found pivot A[%d,%d], min positive ratio=%g in row=%d.\n", pivot_row, pivot_col, min_ratio, pivot_row);
	return pivot_row;
}

void add_slack_variables(table *tab) 
{
	int i, j;
	for (i = 1; i < tab->m; i++) 
	{
		for (j = 1; j < tab->m; j++)
			tab->mat[i][j + tab->n - 1] = (i == j);
	}
	tab->n += tab->m - 1;
}

void check_b_positive(table *tab) 
{
	int i;
	for (i = 1; i < tab->m; i++)
		assert(tab->mat[i][0] >= 0);
}

//	В столбце единичной матрицы найти строку, в которой встречается 1.
//	Если столбец не соответствует единичной матрице, вернуть -1.
int find_basis_variable(table *tab, int col) 
{
	int i, xi = -1;
	for (i = 1; i < tab->m; i++) 
	{
		if (equal(tab->mat[i][col], 1)) 
		{
			if (xi == -1)
				xi = i;
			else
				return -1; //	найдена вторая единица - не столбец единичной матрицы

		}
		else if (!equal(tab->mat[i][col], 0)) 
			return -1; //		не столбец единичной матрицы
	}
	return xi;
}

void print_optimal_vector(table *tab, char *message) 
{
	int j, xi;
	printf("%s at ", message);
	for (j = 1; j < tab->n; j++) 
	{
		xi = find_basis_variable(tab, j);
		if (xi != -1)
			printf("x%d=%3.2lf, ", j, tab->mat[xi][0]);
		else
			printf("x%d=0, ", j);
	}
	printf("\n");
}

void simplex(table *tab) 
{
	int loop = 0;
	add_slack_variables(tab);
	check_b_positive(tab);
	print_table(tab, "Padded with slack variables");
	while (++loop) 
	{
		int pivot_col, pivot_row;

		pivot_col = find_pivot_column(tab);
		if (pivot_col < 0) 
		{
			printf("Found optimal value=A[0,0]=%3.2lf (no negatives in row 0).\n", tab->mat[0][0]);
			print_optimal_vector(tab, "Optimal vector");
			break;
		}
		printf("Entering variable x%d to be made basic, so pivot_col=%d.\n", pivot_col, pivot_col);

		pivot_row = find_pivot_row(tab, pivot_col);
		if (pivot_row < 0) 
		{
			printf("unbounded (no pivot_row).\n");
			break;
		}
		printf("Leaving variable x%d, so pivot_row=%d\n", pivot_row, pivot_row);

		pivot_on(tab, pivot_row, pivot_col);
		print_table(tab, "After pivoting");
		print_optimal_vector(tab, "Basic feasible solution");

		if (loop > 20) 
		{
			printf("Too many iterations > %d.\n", loop);
			break;
		}
	}
}

table tab = { 4, 3, {                     // Размеры таблицы [4 строки x 5 столбцов ]
	{ 0. , -3. , -2. },  //	z = 0.5*x + 3*y + z + 4*w -> max
	{ 15 ,  1 ,  3},  //    x + y + z + w <= 40 .. b1
	{ 7 , 1 , 1},  //  -2x - y + z + w <= 10 .. b2
	{ 12 ,  2 ,  1.0}  //        y     - w <= 10 .. b3
}
};

// Пример вызова из командной строки: orig.exe input.txt
int main(int argc, char *argv[]) 
{
	if (argc > 1) 
	{ 
		read_table(&tab, argv[1]);
	}
	print_table(&tab, "Initial");
	simplex(&tab);
	system("pause");
	return 0;
}