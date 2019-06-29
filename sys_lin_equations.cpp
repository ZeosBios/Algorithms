#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;

//	глобальная система уравнений
double **matrix;

//	вектор-столбец правой части
double *b;

//	вектор решения
double *x;

//	размерность системы (количество уравнений)
int N;

//	точность решения системы итерационными методами
const double EPS = 1e-7;

//	максимальное количество итераций
const int MAX_ITER = 20000;

//	очистка памяти
void clear()
{
	for (int i = 0; i < N; i++)
		delete[] matrix[i];
	delete[] matrix;
	if (matrix) matrix = NULL;
	delete[] b;
	if (b) b = NULL;
	delete[] x;
	if (x) x = NULL;
}

//	вычисление нормы вектора
double normV(double *v, int size)
{
	double norm = 0.;
	for (int i = 0; i < size; i++)
		norm += v[i] * v[i];
	return norm;
}

//	чтение системы уравнений из файла
void read_system(FILE *input)
{
	if (input == NULL)
	{
		printf("Unable to read input file\n");
		N = -1;
		return;
	}
	fscanf(input, "%d", &N);
	matrix = new double*[N];
	b = new double[N];
	x = new double[N];
	for (int i = 0; i < N; i++)
	{
		matrix[i] = new double[N];
		for (int j = 0; j < N; j++)
			fscanf(input, "%lf", &matrix[i][j]);
		fscanf(input, "%lf", &b[i]);
		x[i] = 0.;
	}
}

//	вывод решения системы уравнений в консоль
void print_solution()
{
	if (N == -1)
		return;
	printf("Solution:\n");
	for (int i = 0; i < N; i++)
		printf("x [%d] = %4.3lf\n", i, x[i]);
}

//	решение системы уравнений методом Гаусса
void gauss()
{
	if (N == -1)
	{
		printf("Solution failed - the system has not been read\n");
		return;
	}
	//	прямой ход метода Гаусса
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			matrix[j][i] /= -matrix[i][i];
			for (int k = i + 1; k < N; k++)
				matrix[j][k] += matrix[j][i] * matrix[i][k];
			b[j] += matrix[j][i] * b[i];
		}
	}
	//	обратный ход метода Гаусса
	x[N - 1] = b[N - 1] / matrix[N - 1][N - 1];
	for (int i = N - 2; i >= 0; i--)
	{
		double temp = b[i];
		for (int j = i + 1; j < N; j++)
			temp -= x[j] * matrix[i][j];
		x[i] = temp / matrix[i][i];
	}
}

//	решение трехдиагональной системы уравнений методом прогонки
void tma()
{
	if (N == -1)
	{
		printf("Solution failed - the system has not been read\n");
		return;
	}
	double temp;
	//	прямая прогонка
	for (int i = 1; i < N; i++)
	{
		temp = matrix[i][i - 1] / matrix[i - 1][i - 1];
		matrix[i][i] -= temp * matrix[i - 1][i];
		b[i] -= temp * b[i - 1];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%4.3lf\t", matrix[i][j]);
		printf("%4.3lf\n", b[i]);
	}
	//	обратная прогонка
	x[N - 1] = b[N - 1] / matrix[N - 1][N - 1];
	for (int i = N - 2; i >= 0; i--)
		x[i] = (b[i] - matrix[i][i + 1] * x[i + 1]) / matrix[i][i];
}

//	решение системы методом простой итерации (метод Якоби)
void jakobi()
{
	if (N == -1)
	{
		printf("Solution failed - the system has not been read\n");
		return;
	}
	double err = 0.;
	int num_of_iter = 0;
	double *prev = new double[N];
	do
	{
		num_of_iter++;
		for (int i = 0; i < N; i++)
		{
			prev[i] = -b[i];
			for (int j = 0; j < N; j++)
			{
				if (i != j)
					prev[i] += matrix[i][j] * x[j];
			}
			prev[i] /= -matrix[i][i];
		}
		err = abs(x[0] - prev[0]);
		for (int i = 0; i < N; i++)
		{
			if (abs(x[i] - prev[i]) > err)
				err = abs(x[i] - prev[i]);
			x[i] = prev[i];
		}
	} while (err >= EPS && num_of_iter <= MAX_ITER);
	printf("Jakobi method - iterations = %d\n", num_of_iter);
	delete[] prev;
}

//	решение симметричной положительно определенной системы уравнений методом сопряженных градиентов
void conjurate_gradient_method()
{
	if (N == -1)
	{
		printf("Solution failed - the system has not been read\n");
		return;
	}
	//	выделение памяти
	double *zk = new double[N];
	double *rk = new double[N];
	double *sz = new double[N];

	double alpha, beta, mf;
	double sp, sp1, spz;

	int i = 0, j = 0, kl = 1;
	//	вычисление нормы вектора - правой части
	mf = normV(b, N);
	//	установим начальное приближение
	for (i = 0; i < N; i++)
		x[i] = 0.2;
	//	зададим начальные значения r0, z0
	for (i = 0; i < N; i++)
	{
		sz[i] = 0.;
		for (j = 0; j < N; j++)
		{
			sz[i] += matrix[i][j] * x[j];
		}
		rk[i] = b[i] - sz[i];
		zk[i] = rk[i];
	}
	int iter = 0;
	//	последовательность итераций	
	do
	{
		iter++;
		//	вычисление числителя и знаменателя для коэффициента
		spz = 0;
		sp = 0;
		for (i = 0; i < N; i++)
		{
			for (sz[i] = 0, j = 0; j < N; j++) {
				sz[i] += matrix[i][j] * zk[j];
			}
			spz += sz[i] * zk[i];
			sp += rk[i] * rk[i];
		}
		alpha = sp / spz;
		/* Вычисление вектора решения xk = xk-1 + alpha * zk - 1,
		* вектора невязки rk = rk-1 - alpha * A * zk - 1
		* и числителя для beta, равного норме вектора невязки
		*/
		sp1 = 0;
		for (i = 0; i < N; i++)
		{
			x[i] += alpha * zk[i];
			rk[i] -= alpha * sz[i];
			sp1 += rk[i] * rk[i];
		}
		kl++;
		//	вычисление beta
		beta = sp1 / sp;
		// вычисление вектора спуска
		for (i = 0; i < N; i++)
		{
			zk[i] = rk[i] + beta * zk[i];
		}

	} while (sp1 / mf > EPS * EPS && iter < MAX_ITER);
	printf("Conjurate gradient method - iterations = %d\n", iter);
	delete[] zk;
	delete[] rk;
	delete[] sz;
}

int main(int argc, char *argv[])
{
	FILE *in = fopen("input.txt", "r");
	read_system(in);
	//gauss();
	//tma();
	//jakobi();
	//conjurate_gradient_method();
	print_solution();
	clear();
	system("pause");
	return EXIT_SUCCESS;
}