/*
 * Алгоритмы сортировки
 * С.А.Зиновьев, 15.07.2017
*/
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef uint
#define uint unsigned int
#endif // !uint
#ifndef RANGE
#define RANGE 255
#endif // !RANGE

using namespace std;

//	обмен местами значений звух символов
void swap(char &a, char &b)
{
	char t;
	t = a;
	a = b;
	b = t;
}

//	"разделение", или установка ведущего элемента на его место в методе быстрой сортировки
//	функция возвращает индекс ведущего элемента для следующего "разделения"
int partition(char arr[], int low, int high)
{
	char pivot = arr[high];
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++)
	{
		if (arr[j] <= pivot)
		{
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return (i + 1);
}

//	быстрая сортировка, метод Хоара
void quick_sort(char arr[], int low, int high)
{
	if (low < high)
	{
		int p = partition(arr, low, high);
		quick_sort(arr, low, p - 1);
		quick_sort(arr, p + 1, high);
	}
}

//	функция возвращает максимум в массиве
char get_max(char arr[], uint length)
{
	char res = arr[0];
	for (uint i = 1; i < length; i++)
		if (arr[i] > res)
			res = arr[i];
	return res;
}

//	построение полного двоичного дерева ("двоичной кучи")
//	корнем дерева является максимальный элемент массива
//	необходимо для пирамидальной сортировки
void heapify(char arr[], uint length, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < length && arr[l] > arr[largest])
		largest = l;
	if (r < length && arr[r] > arr[largest])
		largest = r;
	if (largest != i)
	{
		swap(arr[i], arr[largest]);
		heapify(arr, length, largest);
	}
}

//	сортировка элементов по выделенному разряду
//	exp - значение разряда (1, 10, 100, 1000, ...)
//	необходимо для поразрядной сортировки
void count(char arr[], uint length, uint exp)
{
	char *output = new char[length];
	int i = 0, count[10] = {0};

	for (; i < length; i++)
	{
		count[((int)arr[i] / exp) % 10]++;
	}
	for (i = 1; i < 10; i++)
	{
		count[i] += count[i - 1];
	}
	for (i = length - 1; i >= 0; i--)
	{
		output[count[((int)arr[i] / exp) % 10] - 1] = arr[i];
		count[((int)arr[i] / exp) % 10]--;
	}
	for (i = 0; i < length; i++)
		arr[i] = (char)output[i];
}

//	цифровая сортировка, сортировка подсчетом
void count_sort(char arr[])
{
	char *output = new char[strlen(arr)];
	int count[RANGE + 1], i;
	memset(count, 0, sizeof(count));
	for (i = 0; arr[i]; ++i)
		++count[arr[i]];
	for (i = 1; i <= RANGE; ++i)
		count[i] += count[i - 1];
	for (i = 0; arr[i]; ++i)
	{
		output[count[arr[i]] - 1] = arr[i];
		--count[arr[i]];
	}
	for (i = 0; arr[i]; ++i)
		arr[i] = output[i];
	delete[] output;
	if (output)
		output = NULL;
}

//	сортировка кучей, пирамидальная сортировка
void heap_sort(char arr[], uint length)
{
	for (int i = length / 2 - 1; i >= 0; i--)
	{
		heapify(arr, length, i);
	}
	for (int i = length - 1; i >= 0; i--)
	{
		swap(arr[0], arr[i]);
		heapify(arr, i, 0);
	}
}

//	поразрядная сортировка
void radix_sort(char arr[], uint length)
{
	char c = get_max(arr, length);
	for (uint exp = 1; (int)c / exp > 0; exp *= 10)
		count(arr, length, exp);
}

//	сортировка взбалтыванием
void shake_sort(char arr[], uint length)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = i; j < length - i - 1; j++)
		{
			if (arr[i] > arr[j])
				swap(arr[i], arr[j]);
			if (arr[length - i - 1] < arr[j])
				swap(arr[length - i - 1], arr[j]);
		}
	}
}

///сортировк слиянием
void merge_sort(int l, int r, int * A)
{
	int c = (r + l) / 2;
	
	int * T = new int[r - l];
	int i = 0;
	int j = 0;
	
	while(l + i < c && c + j <r)
	{
		if (A[l + i] < A[c + j])
		{
			T[i + j] = A[l + i];
			i++;
		}
		else
		{
			T[i + j] = A[c + j];
			j++;
		}
	}
	
	while(l + i < c)
	{
		T[i + j] = A[l + i];
		i++;
	}
	while(c + j < r)
	{
		T[i + j] = A[c + j];
		j++;
	}
	
	for (int a = 0; a < i + j; a++)
	{
		A[a] = T[a];
	}
	delete[] T;
}

///сортировка вставками
void insert_sort(int * A, int n)
{
	for (int i = 1; i < n; i++)
	{
		int x = A[i];
		int j = i;
		while (j > 0 && A[j - 1] > x)
		{
			A[j - 1] = A[j];
			j = j - 1;
		}
		A[j] = x;
	}
}

//	чтение строки
void read(char *arr, uint &length)
{
	char temp = '\0';
	while (temp != '\n')
	{
		temp = getchar();
		length++;
	}
}

//	вывод строки на экран
void print(char arr[], uint length)
{
	for (uint i = 0; i < length; i++)
		printf("%c", arr[i]);
	printf("\n");
}

int main(int argc, char *argv[])
{
	char str[] = "newertheless";
	uint length = 12;
	//radix_sort(str, length);
	//count_sort(str);
	//quick_sort(str, 0, length - 1);
	//heap_sort(str, length);
	shake_sort(str, length);
	print(str, length);
	system("pause");
	return EXIT_SUCCESS;
}