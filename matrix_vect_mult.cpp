/*	
	Определение нормы вектора и скалярное произведение векторов (последовательная схема)
	Матрично-векторное и матрично-матричное умножение (последовательная схема) 
	Вычисление определителя (рекурсивная схема и метод Гаусса)
	Численное инвертирование матрицы (итерационный метод Ньютона-Шульца, последовательная схема)
*/

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

//	норма абсолютной погрешности
const double eps = 1e-7;

//	норма вектора
double vector_norm(double *v, int size)
{
	double res = 0.;
	for (int i = 0; i < size; i++)
		res += v[i] * v[i];
	return sqrt(res);
}

//	скалярное произведение векторов
double scalar_product(double *v1, double *v2, int size)
{
	double res = 0.;
	for (int i = 0; i < size; i++)
		res += v1[i] * v2[i];
	return res;
}

//	умножение матрицы на число - результат записывается в исходную матрицу
void matrix_scalar_mult(double **matrix, int n, int m, double scalar)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			matrix[i][j] *= scalar;
}

//	умножение матрицы на вектор
double* mv_mult(double **matrix, int n, int m, double *vector, int n_v)
{
	if (n_v != m)
	{
		printf("Error while multiplying matrix by vector: size of the vector is not equal to number of lines in matrix\n");
		printf("The product cannot be calculated\n");
		return NULL;
	}
	//	выделение памяти
	double *result = new double[n];
	//	матрично-векторное умножение
	for (int i = 0; i < n; i++)
	{
		double prod = 0;
		for (int j = 0; j < m; j++)
			prod += matrix[i][j] * vector[j];
		result[i] = prod;
	}
	return result;
}

//	умножение двух матриц
double **mm_mult(double **a, int n1, int m1, double **b, int n2, int m2) 
{
	if (m1 != n2)
	{
		printf("Error while nultiplying matrix by matrix\n");
		printf("The product cannot be calculated\n");
		return NULL;
	}
	//	выделение памяти
	double **result = new double*[n1];
	for (int i = 0; i < n1; i++)
		result[i] = new double[m2];
	//	матрично-матричное умножение
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < m2; j++)
		{
			result[i][j] = 0.;
			for (int k = 0; k < n2; k++)
				result[i][j] += a[i][k] * b[k][j];
		}
	}
	return result;
}

//	исключение из матрицы i-ой строки и j-ого столбца
double** matrix_except(double **matrix, int size, int i, int j)
{
	//	выделение памяти
	double **result = new double*[size - 1];
	for (int k = 0; k < size - 1; k++)
		result[k] = new double[size - 1];
	int di = 0, dj = 0;
	for (int ki = 0; ki < size - 1; ki++)
	{
		//	проверка индекса строки
		if (ki == i)
			di = 1;
		dj = 0;
		//	проверка индекса столбца
		for (int kj = 0; kj < size - 1; kj++)
		{
			if (kj == j)
				dj = 1;
			result[ki][kj] = matrix[ki + di][kj + dj];
		}
	}
	return result;
}

//	рекурсивное вычисление определителя
double determinant_rec(double **matrix, int size)
{
	int t = 1;
	//	терминальные условия
	if (size == 1)
		return matrix[0][0];
	if (size == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	double det = 0.;
	for (int i = 0; i < size; i++)
	{
		double **p = matrix_except(matrix, size, i, 0);
		det += t * matrix[i][0] * determinant_rec(p, size - 1);
		t = -t;
	}
	return det;
}

//	вычисление определителя с использованием прямого хода метода Гаусса
double determinant_gauss(double **matrix, int size)
{
	double det = 1.;
	//	прямой ход метода Гаусса с выбором ведущего элемента по строке
	for (int i = 0; i < size; ++i) 
	{
		int k = i;
		//	определение максимального элемента в строке
		for (int j = i + 1; j < size; ++j)
			if (abs(matrix[j][i]) > abs(matrix[k][i]))
				k = j;
		if (abs(matrix[k][i]) < eps) 
		{
			return 0.;
		}
		//	перестановка строк
		for (int j = 0; j < size; j++)
		{
			double temp = matrix[i][j];
			matrix[i][j] = matrix[k][j];
			matrix[k][j] = temp;
		}
		//	при перестановке строк определитель матрицы меняет знак
		if (i != k)
			det = -det;
		//	определитель треугольной матрицы равен произведению ее диагональных элементов
		det *= matrix[i][i];
		for (int j = i + 1; j < size; ++j)
			matrix[i][j] /= matrix[i][i];
		for (int j = 0; j < size; ++j)
			if (j != i && abs(matrix[j][i]) > eps)
				for (int k = i + 1; k < size; ++k)
					matrix[j][k] -= matrix[i][k] * matrix[j][i];
	}
	return det;
}

//	нахождение обратной матрицы итерационным методом Ньютона-Шульца
double** newton_shultz(double **matrix, int n)
{
	double det = determinant_rec(matrix, n);
	//	если матрица является вырожденной, дальнейшие вычисления не производятся
	if (abs(det) < eps)
	{
		printf("Determinant of the matrix is equal to 0, not possible to invert\n");
		return NULL;
	}
	//	копирование исходной матрицы в результат - начальное приближение
	double ** result = new double*[n];
	for (int i = 0; i < n; i++)
	{
		result[i] = new double[n];
		for (int j = 0; j < n; j++)
			result[i][j] = matrix[i][j];
	}
	//	вычисление нормы матрицы по строкам и столбцам
	double norm1 = 0, norm2 = 0;
	for (int i = 0; i < n; i++)
	{
		double rowsum = 0., colsum = 0.;
		for (int j = 0; j < n; j++)
		{
			rowsum += abs(matrix[i][j]);
			colsum += abs(matrix[j][i]);
		}
		norm1 = (norm1 > rowsum) ? norm1 : rowsum;
		norm2 = (norm2 > colsum) ? norm2 : colsum;
	}
	//	транспонирование матрицы
	for (int i = 0; i < n - 1; i++)
		for (int j = i + 1; j < n - 1; j++)
			swap(result[i][j], result[j][i]);
	//	нормирование матрицы
	matrix_scalar_mult(result, n, n, 1. / (norm1 * norm2));
	//	инициализация удвоенной единичной матрицы нужного размера
	double **e = new double*[n];
	for (int i = 0; i < n; i++)
	{
		e[i] = new double[n];
		for (int j = 0; j < n; j++)
			e[i][j] = (i == j) ? 2. : 0.;
	}
	//	итерационный процесс Ньютона-Шульца
	while (abs(determinant_gauss(mm_mult(matrix, n, n, result, n, n), n) - 1) >= eps)
	{
		double **prev = new double*[n];
		for (int i = 0; i < n; i++)
		{
			prev[i] = new double[n];
			for (int j = 0; j < n; j++)
				prev[i][j] = result[i][j];
		}
		//	результат умножается на матрицу, полученную на предыдущем шаге
		result = mm_mult(matrix, n, n, prev, n, n);
		//	результат умножается на -1
		matrix_scalar_mult(result, n, n, -1.);
		//	результат складывается с удвоенной единичной матрицей
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				result[i][j] += e[i][j];
		result = mm_mult(prev, n, n, result, n, n);
		delete[] prev;
	}
	return result;
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		//	умножение двух матриц
		if (strcmp(argv[1], "matrix") == 0)
		{
			int n1, n2, m1, m2;
			FILE *in = fopen("E:/Inf/c_linear_algebra/c_linear_algebra/input.txt", "r");
			//	считывание первой матрицы
			fscanf(in, "%d %d", &n1, &m1);
			double **a = new double*[n1];
			for (int i = 0; i < n1; i++)
				a[i] = new double[m1];
			for (int i = 0; i < n1; i++)
				for (int j = 0; j < m1; j++)
					fscanf(in, "%lf", &a[i][j]);
			//	считывание второй матрицы
			fscanf(in, "%d %d", &n2, &m2);
			double **b = new double*[n2];
			for (int i = 0; i < n2; i++)
				b[i] = new double[m2];
			for (int i = 0; i < n2; i++)
				for (int j = 0; j < m2; j++)
					fscanf(in, "%lf", &b[i][j]);
			//	умножение матриц
			double **c = mm_mult(a, n1, m1, b, n2, m2);
			//	вывод результата
			if (c == NULL)
				return EXIT_FAILURE;
			for (int i = 0; i < n1; i++)
			{
				for (int j = 0; j < m2; j++)
					printf("%3.2lf\t", c[i][j]);
				printf("\n");
			}
			//	освобождение памяти
			delete[] a;
			delete[] b;
			delete[] c;
			if (a)
				a = NULL;
			if (b)
				b = NULL;
			if (c)
				c = NULL;
			fclose(in);
			if (in)
				in = NULL;
		}
		//	умножение матрицы на вектор
		else if (strcmp(argv[1], "vector") == 0)
		{
			int n, m, nv;
			FILE *in = fopen("E:/Inf/c_linear_algebra/c_linear_algebra/input.txt", "r");
			//	считывание матрицы
			fscanf(in, "%d %d", &n, &m);
			double **a = new double*[n];
			for (int i = 0; i < n; i++)
				a[i] = new double[m];
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					fscanf(in, "%lf", &a[i][j]);
			//	считывание вектора
			fscanf(in, "%d", &nv);
			double *v = new double[nv];
			for (int i = 0; i < nv; i++)
				fscanf(in, "%lf", &v[i]);
			//	матрично-векторное умножение
			double *res = mv_mult(a, n, m, v, nv);
			//	вывод результата
			if (res == NULL)
				return EXIT_FAILURE;
			for (int i = 0; i < nv; i++)
				printf("%3.2lf\n", res[i]);
			//	освобождение памяти
			delete[] v;
			delete[] a;
			delete[] res;
			if (v)
				v = NULL;
			if (a)
				a = NULL;
			if (res)
				res = NULL;
			fclose(in);
			if (in)
				in = NULL;
		}
		else if (strcmp(argv[1], "norm_vector") == 0)
		{
			FILE *in = fopen("E:/Inf/c_linear_algebra/c_linear_algebra/input.txt", "r");
			//	считывание вектора
			int n;
			fscanf(in, "%d", &n);
			double *v = new double[n];
			for (int i = 0; i < n; i++)
				fscanf(in, "%lf", &v[i]);
			//	вычисление нормы вектора
			double norm = vector_norm(v, n);
			//	вывод результата
			printf("Norm of the vector = %lf\n", norm);
			//	освобождение памяти
			delete[] v;
			if (v)
				v = NULL;
			fclose(in);
			if (in)
				in = NULL;
		}
		//	вычисление определителя матрицы методом Гаусса
		else if (strcmp(argv[1], "det_gauss") == 0)
		{
			FILE *in = fopen("E:/Inf/c_linear_algebra/c_linear_algebra/input.txt", "r");
			//	считывание квадратной матрицы
			int n;
			fscanf(in, "%d", &n);
			double **a = new double*[n];
			for (int i = 0; i < n; i++)
				a[i] = new double[n];
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					fscanf(in, "%lf", &a[i][j]);
			//	вычисление определителя
			double det = determinant_gauss(a, n);
			//	вывод результата
			printf("Determinant = %lf\n", det);
			//	освобождение памяти
			delete[] a;
			if (a)
				a = NULL;
			fclose(in);
			if (in)
				in = NULL;
		}
		//	рекурсивное вычисление определителя
		else if (strcmp(argv[1], "det_rec") == 0)
		{
			FILE *in = fopen("E:/Inf/c_linear_algebra/c_linear_algebra/input.txt", "r");
			//	считывание квадратной матрицы
			int n;
			fscanf(in, "%d", &n);
			double **a = new double*[n];
			for (int i = 0; i < n; i++)
				a[i] = new double[n];
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					fscanf(in, "%lf", &a[i][j]);
			//	вычисление определителя
			double det = determinant_rec(a, n);
			//	вывод результата
			printf("Determinant = %lf\n", det);
			//	освобождение памяти
			delete[] a;
			if (a)
				a = NULL;
			fclose(in);
			if (in)
				in = NULL;
		}
		//	нахождение обратной матрицы
		else if (strcmp(argv[1], "inverse") == 0)
		{
			FILE *in = fopen("E:/Inf/c_linear_algebra/c_linear_algebra/input.txt", "r");
			//	считывание квадратной матрицы
			int n;
			fscanf(in, "%d", &n);
			double **a = new double*[n];
			for (int i = 0; i < n; i++)
			{
				a[i] = new double[n];
				for (int j = 0; j < n; j++)
					fscanf(in, "%lf", &a[i][j]);
			}
			//	нахождение обратной матрицы
			double **inv = newton_shultz(a, n);
			//	вывод результата
			if (inv != NULL)
			{
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
						printf("%4.3lf\t", inv[i][j]);
					printf("\n");
				}
			}
			//	освобождение памяти
			delete[] inv;
			if (inv)
				inv = NULL;
			delete[] a;
			if (a)
				a = NULL;
			fclose(in);
			if (in)
				in = NULL;
		}
		else
		{
			printf("Invalid argument\n");
		}
	}
	else
		printf("Not enough arguments\n");
	return EXIT_SUCCESS;
}