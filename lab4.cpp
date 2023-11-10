//Дана последовательность из N целых чисел.
//Предусмотреть выбор пользователем варианта создания последовательности :
//– ввод из файла;
//– ввод с клавиатуры;
//– генерация случайным образом.
//1. Найти количество четных элементов, расположенных после последнего минимального.
//2. Найти сумму положительных чисел, расположенных между первым и последними числами кратными 7
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <functional>

int menu1();
short menu2();
using TLambda = std::function<int()>;
int task1(int& cnt, int n, TLambda read, std::function<bool(int)> predicate);
int task2(int& sum, int n, TLambda read, std::function<bool(int)> predicate);
template <typename T, typename Predicat>
void ReadAndCheck(T& x, Predicat condition, const char* message);
void ending(int n);
void print_answer(int ind, int res);
int checkFile(std::ifstream& file);

int main()
{
	SetConsoleOutputCP(1251);

	short choice;
	do {
		choice = menu1();
		if (choice != 3) 
		{
			short option;
			option = menu2();
			TLambda read;
			if (option == 1) // из файла 
			{
				std::ifstream file("test1.txt");
				switch (checkFile(file))
				{
				case -1:
					std::cout << "Файл не нейден\n";
					break;
				case 0:
					std::cout << "Файл пуст\n";
					break;
				default:
				{
					int n;
					read = [&file]()->int {int x; file >> x; return x; };
					file >> n;
					if (choice == 1)
					{
						int cnt{};
						int res = task1(cnt, n, read, [](int x) {return abs(x) % 2 == 0; });
						print_answer(res, cnt);
					}
					else
					{
						int sum{};
						int res = task2(sum, n, read, [](int x) {return x > 0; });
						print_answer(res, sum);
					}
				}
				file.close();
				}
			}
			if (option == 2) //с клавы
			{
				read = []()->int {int x; std::cin >> x; return x; };
				if (choice == 1)
				{
					int cnt{};
					int n;
					ReadAndCheck(n, [](int n)->bool {return n > 0; }, "Введите количество элементов: \n");
					ending(n);
					int res = task1(cnt, n, read, [](int x) {return abs(x) % 2 == 0; });
					print_answer(res, cnt);
				}
				else
				{
					int sum{};
					int n;
					ReadAndCheck(n, [](int n)->bool {return n > 0; }, "Введите количество элементов: \n");
					ending(n);
					int res = task2(sum, n, read, [](int x) {return x > 0; });
					print_answer(res, sum);
				}
			}
			if (option == 3) //рандом
			{
				srand(GetTickCount());
				int n;
				ReadAndCheck(n, [](int n)->bool {return n > 0; }, "Введите количество элементов: \n");
				int a, b;
				std::cout << "Введите диапазон от a до b (a < b):";
				ReadAndCheck(a, [](int x) {return true; }, "\n");
				ReadAndCheck(b, [](int x) {return true; }, "");
				read = [a, b]()->int {int x; x = rand() % (b - a + 1) + a; return x; };
				if (choice == 1)
				{
					int cnt{};
					int res = task1(cnt, n, read, [](int x) {return abs(x) % 2 == 0; });
					print_answer(res, cnt);
				}
				else
				{
					int sum{};
					int res = task2(sum, n, read, [](int x) {return x > 0; });
					print_answer(res, sum);
				}
			}

		std::cout << "\nЗавершить работу? (Y/любая клавиша): ";
		char isExit;
		std::cin >> isExit;
		if (isExit == 'Y' || isExit == 'y')
			choice = 3;
		}
	} while (choice != 3);
}

int menu1()
{
	std::cout << "\nВыберите задачу для решения:";
	std::cout << "\n1.Найти количество четных элементов, расположенных после последнего минимального.";
	std::cout<<"\n2.Найти сумму положительных чисел, расположенных между первым и последними числами кратными 7";
	std::cout << "\n3.Завершить работу\n";
	std::function<bool(short)> Lambda = [](short x) {return x >= 1 && x <= 3; };
	short choice = 0;
	ReadAndCheck(choice, Lambda, "->");
	return choice;
}

short menu2()
{
	std::cout << "\nВыбирите способ ввода последовательности:";
	std::cout << "\n1.Из файла";
	std::cout<<"\n2.Ручной ввод с клавиатуры";
	std::cout << "\n3.Генератор случайных значений\n";
	std::function<bool(short)> Lambda = [](short x) {return x >= 1 && x <= 3; };
	short option = 0;
	ReadAndCheck(option, Lambda, "->");
	return option;
}

//1. Найти количество четных элементов, расположенных после последнего минимального.
int task1(int& cnt, int n, TLambda read, std::function<bool(int)> predicate)
{
	//сначала мин  = 1 эл потом уже считать
	int x;
	int k{};
	int minn = read();
	for (int i{ 2 }; i <= n; ++i)
	{
		x = read();
		if (predicate(x))
			++k;
		if (x <= minn) 
		{
			minn = x;
			k = 0;
		}
	}
	int result = 2;
	if (!k)//нет элементов с таким свойством
		result = -1;
	cnt = k;
	return result;
}
//2. Найти сумму положительных чисел, расположенных между первым и последними числами кратными 7
int task2(int& sum, int n, TLambda read, std::function<bool(int)> predicate)
{
	int x;
	int sum_cur{};
	int first{}, last{};
	for (int i = 1; i <= n; ++i)
	{
		x = read();
		if (!first)
		{
			if (!(x % 7))
				first = i;
		}
		else
		{
			if (!(x % 7))
			{
				last = i;
				sum = sum_cur;
			}
			if (predicate(x))
				sum_cur += x;
		}
	}
	int result{2};
	if (!first)
		result = -1;
	else
	{
		if (last - first == 1)
			result = 0;
		if (!last)
			result = 1;	
		if (last && !sum)
			result = -1;
	}
	return result;
}

void ending(int n)
{
	std::cout << "Введите " << n << " элемент";
	if (n%100 < 21 && n%100 >10)
		std::cout << "ов: ";
	else {
		switch (n % 10) {
		case 1:
			break;
		case 2:
		case 3:
		case 4:
			std::cout << "а: ";
			break;
		default:
			std::cout << "ов: ";
			break;
		}
	}
}


void print_answer(int ind, int res)
{
	switch (ind)
	{
	case -1: std::cout << "Нет элементов с заданным свойством\n";
		break;
	case 1: std::cout << "Только один элемент с заданным свойством\n";
		break;
	case 0: std::cout << "Пустой диапазон\n";
		break;
	default: std::cout << "Результат равен: " << res << '\n';
	}
}

template<typename T, typename Predicat>
void ReadAndCheck(T& x, Predicat condition, const char* message)
{
	std::cout << message;
	while (!(std::cin >> x && condition(x))) {
		std::cout << "Ошибка! Введите корректное значение:\n";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cout << message;
	}
}

int checkFile(std::ifstream& file)
{
	int result = 1;
	if (!file)
		result = -1;
	else
		if (file.peek() == EOF)
			result = 0;
	return result;
}