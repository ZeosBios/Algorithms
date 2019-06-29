/*
 * операции со строками
 * С.А. Зиновьев, 10.07.2017
*/
#include <iostream>
#include <cstdlib>
#include <algorithm>

#define uint unsigned int

//	максимальная допустимая длина строки
const int BUFFER_SIZE = 256;

using namespace std;

//	структура данных - строка
struct c_string
{
private:
	char *symbols;	//	массив символов, содержащихся в строке
	uint length;	//	длина строки
public:

	//	выделение памяти длЯ хранения строки
	void create_str()
	{
		symbols = new char[BUFFER_SIZE];
		fill(&symbols[0], &symbols[BUFFER_SIZE], '\0');
		length = 0;
	}

	//	выделение памЯти длЯ хранения строки фиксированной длины
	void create_str(uint len)
	{
		symbols = new char[len];
		fill(&symbols[0], &symbols[len], '\0');
		length = len;
	}

	//	Блок перегрузки операторов

	//	сравнение строк
	const bool operator ==(const c_string s)
	{
		if (s.length != length)
			return false;
		for (uint i = 0; i < length; i++)
		{
			if (symbols[i] != s.symbols[i])
				return false;
		}
		return true;
	}

	const bool operator !=(const c_string s)
	{
		if (s.length != length)
			return true;
		bool f = false;
		for (uint i = 0; i < length; i++)
		{
			if (symbols[i] != s.symbols[i])
				f = true;
		}
		return f;
	}

	//	Отношение строгого лексикографического порядка
	const bool operator <(const c_string s)
	{
		uint min_len;
		s.length < length ? min_len = s.length : min_len = length;
		for (uint i = 0; i < min_len; i++)
		{
			if (symbols[i] < s.symbols[i])
				return true;
			if (symbols[i] > s.symbols[i])
				return false;
			if (symbols[i] == s.symbols[i])
				continue;
		}
		return false;
	}

	//	конкатенация (сложение строк)
	const c_string operator +(const c_string s)
	{
		c_string res;
		res.create_str(length + s.length);
		uint i = 0, t = 0;
		for (; i < length; i++)
			res.symbols[i] = symbols[i];
		for (t = 0; t < s.length; i++)
		{
			res.symbols[i] = s.symbols[t];
			t++;
		}
		return res;
	}

	//	чтение строки с клавиатуры до первого пробела
	void readstr()
	{
		char t = '\0'; 
		uint index = 0;
		if (length == 0)
		{
			while (t != ' ' && t != '\n' && length < BUFFER_SIZE)
			{
				t = getchar();
				if (t != ' ' && t != '\n')
				{
					symbols[length] = t;
					length++;
				}
				else return;
			}
		}
		else
		{
			while (index < length && t != ' ' && t != '\n')
			{
				t = getchar();
				if (t != ' ' && t != '\n')
				{
					symbols[index] = t;
					index++;
				}
				else return;
			}
		}
	}

	//	вывод строки в консоль
	void print_str()
	{
		for (uint i = 0; i < length; i++)
			cout << symbols[i];
	}

	//	метод возвращает длину строки
	uint get_length()
	{
		return length;
	}

	//	метод возвращает символ, содержащийся в строке, с указанным индексом
	char at(uint index)
	{
		if (index >= length)
			return '\0';
		return symbols[index];
	}

	//	поиск символа в строке (метод возвращает -1 при отсутствии указанного символа в строке) 
	int find_sym(char c)
	{
		for (uint i = 0; i < length; i++)
		{
			if (symbols[i] == c)
				return i;
		}
		return -1;
	}

	//	копирование строки
	c_string copy(c_string s)
	{
		c_string result;
		result.create_str(s.length);
		for (uint i = 0; i < s.length; i++)
			result.symbols[i] = s.symbols[i];
		return result;
	}

	//	удаление из строки num символов, начиная с индекса start
	c_string del(uint start, uint num)
	{
		c_string result;
		uint i = 0;
		result.create_str();
		for (; i < start; i++)
		{
			result.symbols[i] = symbols[i];
			result.length++;
		}
		uint t = i;
		for (i = start + num; i < length; i++)
		{
			result.symbols[t] = symbols[i];
			t++;
			result.length++;
		}
		return result;
	}

	// копирование подстроки, начиная с индекса start и заканчивая индексом end
	c_string copy(c_string s, uint start, uint end)
	{
		c_string result;
		result.create_str(end - start + 1);
		uint p = 0;
		for (uint i = start; i <= end; i++)
		{
			result.symbols[p] = symbols[i];
			p++;
		}
		return result;
	}

	//	поиск первого вхождения указанной подстроки
	int find(c_string substr)
	{
		uint i = 0, j = 0;
		if (substr.length > length)
			return -1;
		for (i = 0; i < length - substr.length; i++)
		{
			for (j = 0; j < substr.length; j++)
			{
				if (substr.symbols[j] != symbols[i + j])
					break;
				else continue;
			}
			if (j == substr.length)
			{
				return i;
			}
		}
		return -1;
	}

	//	преобразование строки в целое число
	uint str_to_int()
	{
		uint res = 0, i = 0;
		for (i = 0; i < length; i++)
		{
			if (symbols[i] >= '0' && symbols[i] <= '9')
			{
				res += (int)symbols[i] - (int)'0';
				break;
			}
		}
		i++;
		while (symbols[i] >= '0' && symbols[i] <= '9')
		{
			res *= 10;
			res += (int)symbols[i] - (int)'0';
			i++;
		}
		return res;
	}
};

// демонстрация работы структуры данных

int main(int argc, char *argv[])
{
	/*c_string lines[5];
	for (uint i = 0; i < 5; i++)
	{
		lines[i].create_str();
		lines[i].readstr();
	}
	sort(&lines[0], &lines[5]);
	for (uint i = 0; i < 5; i++)
	{
		lines[i].print_str();
		cout << endl;
	}*/
	c_string str, str1;
	str.create_str();
	str1.create_str();
	str.readstr();
	str1 = str.del(2, 2);
	cout << "Result: ";
	str1.print_str();
	cout << endl;
	system("pause");
	return EXIT_SUCCESS;
}