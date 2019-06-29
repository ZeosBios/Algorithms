/*Длина наибольшей неубывающей подпоследовательности*/
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>

using namespace std;

//	бинарный поиск
int ceil_index(vector<int> &v, int l, int r, int key) 
{
	while (r - l > 1) 
	{
		int m = l + (r - l) / 2;
		if (v[m] >= key)
			r = m;
		else
			l = m;
	}
	return r;
}

//	метод возвращает длину наибольшей возрастающей подпоследовательности
int l_i_s(vector<int> &v) 
{
	if (v.size() == 0)
		return 0;

	vector<int> tail(v.size(), 0);
	int length = 1;
	tail[0] = v[0];
	for (int i = 1; i < v.size(); i++) 
	{
		if (v[i] < tail[0])
			tail[0] = v[i];
		else
			if (v[i] >= tail[length - 1])
				tail[length++] = v[i];
		else
			tail[ceil_index(tail, -1, length - 1, v[i])] = v[i];
	}
	for (int i = 0; i < tail.size(); i++)
		cout << tail[i] << " ";
	cout << endl;
	return length;
}

int main(int argc, char *argv[]) 
{
	vector<int> v{ 2, 5, 3, 7, 11, 8, 12, 13, 6 };
	cout << "Length of the longest increasing subsequence is " << l_i_s(v) << endl;
	system("pause");
	return 0;
}