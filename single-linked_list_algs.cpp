#include <iostream>
#include <clocale>
#include <conio.h>

using std::cout;
using std::cin;
using std::endl;
using std::system;

class Node
{
public:
	int numer;
	int pos;
	Node* next;
	Node(){numer = 0; pos = 0; next = NULL;}	
};

class List
{
private:
	Node* head;
	int Length;
public:
	List(){head = NULL; Length = 0;}
	~List()
	{
		Node* ptr = head, *tmp = NULL;
		if(head)
		{
			while(ptr->next)
			{
				tmp = ptr;
				delete[] ptr;
				ptr = tmp;
			};
			delete[] ptr;
		}
		
	}
	void Show(bool flag);
	void Add();
	void Delete();
	void Sort();
	void Clean();	
	bool Checker(int buf);	
};

void menu();
void Numeric(Node* head);

int main()
{
	setlocale(0,"Russian");
	menu();
	cout<<endl;
	system("pause");
	return 0;
}

void menu()
{
	List point;	
	int buf = 0;
	do
	{
		system("cls");
		cout<<"Меню:"<<endl;
		cout<<"Односвязный список. Функции: создание, удаление, сортировка."<<endl;
		cout<<"Список возможных действий:"<<endl;
		cout<<"1 - Добавление элемента в список"<<endl;
		cout<<"2 - Удаление элемента из списка"<<endl;
		cout<<"3 - Сортировка элементов в списке"<<endl;		
		cout<<"4 - Вывод списка"<<endl;
		cout<<"5 - Очистка списка"<<endl;
		cout<<"ESC - Выход"<<endl;		
		cout<<"Выберите действие:"<<endl;
		switch (_getch())
		{
		case 49:
			cout<<endl;	
			cout<<"Добавление элемента в список."<<endl;
			point.Add();	
			break;
		case 50:
			cout<<endl;	
			cout<<"Удаление элемента из списка."<<endl;
			point.Delete();
			break;
		case 51:
			cout<<endl;
			cout<<"Сортировка элементов в списке."<<endl;
			point.Sort();
			break;
		case 52:
			cout<<endl;	
			cout<<"Вывод списка."<<endl;
			point.Show(false);
			break;
		case 53:
			cout<<endl;
			cout<<"Очистка списка."<<endl;
			point.Clean();			
			break;
		case 27://ESC button			
			exit(0);
			break;
		default:
			break;
		}
	}
	while(true);
}

void List::Add()
{
	int buf = -1;	
	cout<<"Введите цифру: ";
	cin.clear();
	cin.sync();
	cin >> buf;
	if(!Checker(buf))	
	{
		if(head == NULL)
		{
			Node* ptr = NULL;
			ptr = new Node;
			ptr->numer = buf;
			ptr->next = NULL;
			head = ptr;
		}
		else
		{
			Node* ptr = NULL;
			ptr = new Node;
			ptr->numer = buf;
			ptr->next = head;
			head = ptr;
		}
		Length++;
		cout<<"Цифра - "<<buf<<" добавлена"<<endl;		
	}	
	cout<<endl;
	system("pause");
}

void List::Delete()
{
	if(head)
	{
		Show(true);
		int temp = -1;
		cout<<"Выберите элемент, введя его номер позиции: ";
		cin.clear();
		cin.sync();
		cin >> temp;
		if(!Checker(temp))
		{
			if(temp > Length || temp == 0)
			{
				cout<<"Ввели неправильный номер элемента. Повторите ввод." << endl;
			}
			else
			{				
				Node* ptr = head, *tmp = NULL, *ToDel = NULL, *prev = NULL;
				int position = 0, Value = 0;
				while(ptr)
				{
					if(ptr->pos == temp)
					{
						ToDel = ptr;
						Length--;
						position = ptr->pos;
						Value = ptr->numer;
						break;
					}
					else
					{
						prev = ptr;
						ptr = ptr->next;
					}
				};
				if(ToDel)
				{
					if(ToDel == head)
					{
						tmp = ToDel->next;
						delete[] ToDel;
						head = tmp;
					}
					else
					{
						if(ToDel->next == NULL)
						{
							delete[] ToDel;
							prev->next = NULL;
						}
						else
						{
							prev->next = ToDel->next;
							delete[] ToDel;
						}
					}
					cout<<"Элемента списка под номером {" << position <<") " << Value <<"} удален успешно."<<endl;
				}

			}
		}
		cout<<endl;
		system("pause");
	}
}

void List::Sort()
{	
	Node* ptr = head, *tmp = NULL, *prev = NULL;
    bool flag = false; 
	if(head)
	{
		do
		{
			flag = false;
			ptr = head;
			while(ptr->next)
			{
				if(ptr->numer > ptr->next->numer)
				{
					if(ptr == head)
					{
						tmp = ptr;
						ptr = tmp->next;
						tmp->next = ptr->next;
						ptr->next = tmp;
						head = ptr;
						flag = true;                   
					}
					else
					{
						tmp = ptr;
						ptr = tmp->next;
						tmp->next = ptr->next;
						ptr->next = tmp;
						prev->next = ptr;
						flag = true;
					}
				}
				prev = ptr;
				ptr = ptr->next;
			}
		}
		while(flag);		
		cout<<"Список отсортирован."<<endl;
	}
	else
		cout<<"Список пуст. Сортировать нечего."<<endl;
	cout<<endl;
	system("pause");
}

void List::Show(bool flag)
{	
	cout<<"Текущий список:"<<endl;
	Node* ptr = head;
	if(!flag)
	{
		if(head == NULL)
		{
			cout<<"Список пуст."<<endl;
		}
		while(ptr)
		{		
			cout<<ptr->numer<<"\t";
			ptr=ptr->next;
		}
		cout<<endl;
		system("pause");
	}
	else
	{
		if(head == NULL)
		{
			cout<<"Список пуст."<<endl;
		}
		else
		{			
			Numeric(head);
			while(ptr)
			{
				cout<<ptr->pos<<") ";
				cout<<ptr->numer<<endl;
				ptr=ptr->next;
			}		
		}
	}
	

}

bool List::Checker(int buf)
{
	bool flag = false;
	if(buf == -1 || buf < 0)
	{
		cout<<"Введена не цифра. Повторите ввод." << endl;
		flag = true;			
	}
	else flag = false;	
	
	return flag;
}

void List::Clean()
{
	Node* ptr = head, *tmp = NULL;
	if(head)
	{
		while(ptr->next)
		{
			tmp = ptr->next;
			delete[] ptr;
			ptr = tmp;
		}
		delete[] ptr;
		head = NULL;
		Length = 0;
		cout<<"Список успешно очищен."<<endl;
	}
	else
	{
		cout<<"Список пуст. Нечего очищать."<<endl;
	}
	cout<<endl;
	system("pause");
}

void Numeric(Node* head)
{
	Node* ptr = head;
	int idx = 1;
	while(ptr)
	{
		ptr->pos = idx;
		idx++;
		ptr=ptr->next;
	}
}