#include <Windows.h>
#include <conio.h>
#include <time.h>
#include<iostream>
#include <cwchar>

#define BLACK 0
#define BLUE  1
#define GREEN 2
#define AQUA  3
#define RED   4
#define PURPLE 5
#define YELLOW 6
#define WHITE  7
#define GRAY   8
#define LIGHT_BLUE 9
#define LIGHT_GREEN A
#define LIGHT_AQUA  B
#define LIGHT_RED   C
#define LIGHT_PURPLE D
#define LIGHT_YELLOW E
#define BRIGHT_WHITE F

using namespace std;

void SetConsoleSize(int Width, int Height) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, Width, Height, TRUE);
}

void SetConsoleFontSize(int width, int height) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX lpConsoleCurrenFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrenFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrenFontEx);
	lpConsoleCurrenFontEx->dwFontSize.X = width;
	lpConsoleCurrenFontEx->dwFontSize.Y = height;
	SetCurrentConsoleFontEx(out, 0, lpConsoleCurrenFontEx);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void gotoxy(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideCursor(int visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = visible;
	cursor.dwSize = 20;
	SetConsoleCursorInfo(console, &cursor);
}

int life = 5;
int score = 0;
int Time = 10 * 20;
int item_time = 0;
int const ScreenHeight = 22;
int const ScreenWidth = 22;
int map[ScreenHeight][ScreenWidth];
char stage[ScreenHeight][ScreenWidth] = { " ################### ",
										  " #........#........# ",
										  " #.##.###.#.###.##.# ",
										  " #.................# ",
										  " #.##.#.#####.#.##.# ",
										  " #....#...#...#....# ",
										  " ####.###.#.###.#### ",
										  "    #.#.......#.#    ",
										  "#####.#.##.##.#.#####",
										  "#@.................@#",
										  "#####.#.##.##.#.#####",
										  "    #.#.......#.#    ",
										  " ####.#.#####.#.#### ",
										  " #........#........# ",
										  " #.##.###.#.###.##.# ",
										  " #..#...........#..# ",
										  " ##.#.#.#####.#.#.## ",
										  " #....#...#...#....# ",
										  " #.######.#.######.# ",
										  " #.................# ",
										  " ################### " };

void coin() {
	srand(time(0));
	int x = rand() % 20;
	int y = rand() % 20;
	while (1) {
		if (stage[x][y] == '.') {
			stage[x][y] = '*';
			break;
		}
		x = rand() % 20;
		y = rand() % 20;
	}
}

class player {
public:
	int x;
	int y;
	char ch;
	int delay;
	int countDelay;

	void draw() {
		if (ch == 'U') map[y][x] = 1;
		else if (ch == 'D') map[y][x] = 2;
		else if (ch == 'L') map[y][x] = 3;
		else if (ch == 'R' || ch == 'S') map[y][x] = 4;
	}

	void move(int xf, int yf) {
		dead();
		if (countDelay == delay) {
			x = xf;
			y = yf;
			countDelay = 0;
		}
		countDelay++;
		eat();
		getitem();
	}

	bool collision(int fx, int fy) {
		if (map[fy][fx] == 9) return true;
		return false;
	}

	void dead() {
		if (map[y][x] == 5) {
			ch = 'S';
			x = 10;
			y = 15;
			life--;
		}
	}

	void eat() {
		if (map[y][x] == 7) {
			stage[y][x] = ' ';
			score += 10;
			coin();
		}
	}

	void getitem() {
		if (map[y][x] == 6) {
			stage[y][x] = ' ';
			item_time += 20 * 20;
		}
	}

};

class Enemy {
public:
	int x;
	int y;
	int ch;
	int delay;
	int countDelay;

	void draw() {
		if (item_time > 0) map[y][x] = 10;
		else map[y][x] = 5;
	}


	void move() {
		if (countDelay == delay) {
			if (ch == 0 && !collision(x, y - 1)) y--;
			if (ch == 1 && !collision(x, y + 1)) y++;
			if (ch == 2 && !collision(x - 1, y)) x--;
			if (ch == 3 && !collision(x + 1, y)) x++;
			countDelay = 0;
			ch = rand() % 4;
		}
		countDelay++;
	}

	bool collision(int fx, int fy) {
		if (map[fy][fx] == 9) return true;
		return false;
	}
};

player pacman;
Enemy enemy[5];

void setup() {
	srand(time(NULL));
	pacman.x = 10;
	pacman.y = 15;
	pacman.ch = 'S';
	pacman.delay = 5;


	enemy[0].x = 9;
	enemy[0].y = 9;
	enemy[0].delay = 3;

	enemy[1].x = 10;
	enemy[1].y = 9;
	enemy[1].delay = 3;


	enemy[2].x = 11;
	enemy[2].y = 9;
	enemy[2].delay = 3;

	enemy[3].x = 2;
	enemy[3].y = 1;
	enemy[3].delay = 3;

	enemy[4].x = 18;
	enemy[4].y = 1;
	enemy[4].delay = 3;

	coin();
}

void layout() {
	for (int i = 0; i < ScreenHeight; i++) {
		for (int j = 0; j < ScreenWidth; j++) {
			if (stage[i][j] == '#') map[i][j] = 9;
			else if (stage[i][j] == '.') map[i][j] = 8;
			else if (stage[i][j] == '*') map[i][j] = 7;
			else if (stage[i][j] == '@') map[i][j] = 6;
			else map[i][j] = 0;
		}
	}
}


void display() {
	gotoxy(2, 1); cout << "life: " << life;
	gotoxy(35, 1); cout << "score: " << score;
	gotoxy(10, 1); cout << "time : " << (Time--) / 20;
	layout();
	pacman.draw();
	if (item_time > 0) {
		if (item_time == 0) {
			item_time = 0;
		}
		gotoxy(30, 1); cout << (item_time--) / 20;
	}

	for (int i = 0; i < 5; i++) {
		enemy[i].draw();
	}
	for (int i = 0; i < ScreenHeight; i++) {
		for (int j = 0; j < ScreenWidth; j++) {
			setcolor(WHITE, BLACK);
			gotoxy(j + 2, i + 3);
			if (map[i][j] == 9) {
				setcolor(GREEN, BLACK);
				cout << char(178);  //collision
			}
			if (map[i][j] == 8) cout << ' '; // dot
			if (map[i][j] == 1) cout << char(86);
			if (map[i][j] == 2) cout << char(94);
			if (map[i][j] == 3) cout << char(62);
			if (map[i][j] == 4) cout << char(60);
			if (map[i][j] == 5) cout << char(234); // enermy
			if (map[i][j] == 0) cout << ' ';
			if (map[i][j] == 7) {
				setcolor(YELLOW, BLACK);
				cout << char(233);
			}
			if (map[i][j] == 6) cout << char(235);
			if (map[i][j] == 10) cout << char(153);
		}
	}

}

void input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 's':
			if (!pacman.collision(pacman.x, pacman.y + 1)) {
				pacman.ch = 'D';
				pacman.move(pacman.x, pacman.y++);
			}
			break;
		case 'w':
			if (!pacman.collision(pacman.x, pacman.y - 1)) {
				pacman.ch = 'U';
				pacman.move(pacman.x, pacman.y--);
			}
			break;
		case 'a':
			if (!pacman.collision(pacman.x - 1, pacman.y)) {
				pacman.ch = 'L';
				pacman.move(pacman.x--, pacman.y);
			}
			break;
		case 'd':
			if (!pacman.collision(pacman.x + 1, pacman.y)) {
				pacman.ch = 'R';
				pacman.move(pacman.x++, pacman.y);
			}
			break;
		}
		fflush(stdin);

	}
}

void Catch(int y, int x) {
	if (item_time == 0) {
		if (map[y][x] == 1 || map[y][x] == 2 || map[y][x] == 3 || map[y][x] == 4) {
			pacman.ch = 'S';
			pacman.x = 10;
			pacman.y = 15;
			life--;
		}
	}
}

void movement() {
	for (int i = 0; i < 5; i++) {
		enemy[i].move();
		Catch(enemy[i].y, enemy[i].x);
	}
}


void menu() {
	char c = '0';
	int scroll = 2;
	bool exit = false;
	system("CLS");
	while (exit == false) {
		setcolor(BLUE, BLACK);
		gotoxy(55, 14);
		cout << char(24);
		gotoxy(55, 15);
		cout << "W";


		gotoxy(55, 28);
		cout << char(25);
		gotoxy(55, 27);
		cout << "S";

		setcolor(WHITE, BLACK);
		gotoxy(52, 19);
		cout << "START";
		gotoxy(52, 21);
		cout << "OPTIONS";
		gotoxy(52, 23);
		cout << "SCORE";
		gotoxy(52, 25);
		cout << "QUIT ";
		while (c != char(13)) {
			setcolor(GREEN, BLACK);
			gotoxy(49, 19 + scroll);
			cout << "->";
			gotoxy(0, 0);
			c = _getch();
			gotoxy(49, 19 + scroll);
			cout << "  ";
			gotoxy(60, 19 + scroll);
			cout << "  ";
			if (c == 's') {
				if (scroll < 6) {
					scroll += 2;
				}
			}
			if (c == 'w') {
				if (scroll > 0) {
					scroll -= 2;
				}
			}
		}
		if (scroll == 6) {  //press Enter
			exit = true;
		}
		cout << scroll;
		scroll = 2;
		c = ' ';
		system("CLS");
	}
}

int main() {

	SetConsoleSize(800, 700);
	hideCursor(0);
	//menu();
	setup();
	while (true) {
		display();
		input();
		movement();
		/*if (Time == 0) {
			system("cls");
			cout << "Game over";
			Sleep(3000);
			_getch();
			Time = 10 * 19;
		}*/
	}
		//cout << char(13);

	return 0;
}