#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;

enum KeyCodes {ENTER = 13,ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32};
enum Colors { GREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15};
enum Objects {HALL = 0, WALL = 1, COIN = 2, ENEMY = 3, MEDICINE = 4, COFFEE = 5};

int main()
{
	int coins = 0;
	int totalCoins = 0;
	int health = 100;
	int energy = 500;
	int movesSinceLastCoffee = 0;

	system("title Бомбер гена!");
	// запуск алгоритма генерации случайных чисел
	srand(time(0));
	rand();

	// таблица аски для поиска символов псевдографики
	//for (int code = 0; code < 256; code++)
	//{
	//	cout << code << " - " << (char)code << "\n";
	//}

	// дескриптор окна консоли (порядковый номер окна в системе)
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);

	const int WIDTH = 50;
	const int HEIGHT = 15;
	int location[HEIGHT][WIDTH] = {};

	// модель локации
	// 0 - коридоры (пустоты)
	// 1 - стена разрушаемая
	// 2 - монетки
	// 3 - враги
	// 4 - аптечка

	// генерация локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			// по дефолту пишется случайное число
			location[y][x] = rand() % 6; // 0 1 2 3

			// стены по краям
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = 1;

			// вход и выход
			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = 0;

			if (location[y][x] == 3) 
			{
				// определяется вероятность того, останется враг или нет
				// допустим, вероястность остаться на уровне - 10%
				int prob = rand() % 10; // 0-9
				if (prob != 0) // 1 2 3 4 5 6 7 8 9
					location[y][x] = 0;
			}

			if (location[y][x] == COIN) 
			{
				totalCoins++; // Подсчет общего количества монет
			}
		}
	}

	// показ локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// cout << location[y][x];
			switch (location[y][x]) 
			{
			case HALL: // коридор
				cout << " ";
				break;
			case WALL: // стена
				SetConsoleTextAttribute(h, GREEN); // 0-255
				cout << (char)177; //(char)219;
				break;
			case COIN: // монетки
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)15; // 249
				break;
			case ENEMY: // враги
				SetConsoleTextAttribute(h, RED);
				cout << (char)1;
				break;
			case MEDICINE: // Показ лекарства
				SetConsoleTextAttribute(h, WHITE);
				cout << (char)3;
				break;
			case COFFEE:
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)21; // чашка коффе
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << "\n";
	}

	// размещение гг
	COORD position;
	position.X = 1;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;
	
	while (true) 
	{

		string title = "Здоровье: " + to_string(health) + "%, Энергия: " + to_string(energy);
		SetConsoleTitleA(title.c_str());

		int code = _getch();
		if (code == 224)
			code = _getch();

		//стирание позиция гг
		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << " ";

		switch (code) 
		{
		case ENTER:
			//cout << "ENTER\n";
			//break;
		case SPACEBAR:
			//cout << "SPACEBAR\n";
			//break;
		case ESCAPE:
			//cout << "ESCAPE\n";
			//break;
		case RIGHT:
			if (location[position.Y][position.X + 1] != WALL)
				position.X++;
			break;
		case LEFT:
			if (position.X > 0 && location[position.Y][position.X - 1] != WALL)
				position.X--;
			break;
		case UP:
			if (position.Y > 0 && location[position.Y - 1][position.X] != WALL)
				position.Y--;
			break;
		case DOWN:
			if (position.Y < HEIGHT - 1 && location[position.Y + 1][position.X] != WALL)
                position.Y++;	
			break;
		default:
			//cout << code << "\n";
			break;
		}

		energy--;
		movesSinceLastCoffee++;

		if (energy <= 0)
		{
			MessageBoxA(NULL, "Поражение – закончилась энергия", "Game Over", MB_OK | MB_ICONERROR);
			return 0;
		}

		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << (char)1;

		if (location[position.Y][position.X] == COIN) 
		{
			coins++;
			SetConsoleCursorPosition(h, { 0, HEIGHT });
			cout << "Coins: " << coins << "     ";
			location[position.Y][position.X] = HALL;

			// Проверка на победу
			if (coins == totalCoins) 
			{
				SetConsoleCursorPosition(h, { 0, HEIGHT + 1 });
				SetConsoleTextAttribute(h, GREEN);
				cout << "Победа - все монеты собраны!" << endl;
				return 0; // Завершение игры
			}
		}

		if (position.X == WIDTH - 1 && position.Y == HEIGHT - 3) 
		{
			MessageBoxA(NULL, "Victory", "Congratulations!", MB_OK | MB_ICONINFORMATION);
			return 0;
		}

		if (location[position.Y][position.X] == ENEMY) 
		{
            int damage = 20 + rand() % 6; // Урон от 20 до 25%
            health -= damage;
            location[position.Y][position.X] = HALL;

            // Проверка на проигрыш
            if (health <= 0) 
			{
				MessageBoxA(NULL, "Defeat", "GameOver", MB_OK | MB_ICONERROR);
                return 0;
            }
        }

        if (location[position.Y][position.X] == MEDICINE) 
		{
            if (health < 100) 
			{
                health += 5;
                if (health > 100) health = 100; // Здоровье не может превышать 100%
                location[position.Y][position.X] = HALL;
            }
        }

		if (location[position.Y][position.X] == COFFEE)
		{
			if (movesSinceLastCoffee >= 10)
			{
				energy += 25;
				movesSinceLastCoffee = 0;
				location[position.Y][position.X] = HALL;
			}
		}

	}
}


