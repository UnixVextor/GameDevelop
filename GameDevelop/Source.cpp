#include <Windows.h>
#include <conio.h>
#include <time.h>
#include<iostream>
#include <cwchar>
#include <time.h>
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include<cmath>

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

void menu();

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

int life = 2;
int score = 0;
int Time = 200;
int SetTime = 200;
unsigned long pastTime = 0;
unsigned long pastTimeItem = 0;
int item_time = 0;
int const ScreenHeight = 22;
int const ScreenWidth = 22;
COORD dir;
string NameWin;
bool is_exit = false;
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
		if (stage[x][y] == '.' && is_exit == false) {
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
	bool ismove = false;
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
			score += 30;
			coin();
		}
	}

	void getitem() {
		if (map[y][x] == 6) {
			stage[y][x] = ' ';
			item_time += 20;
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
	int count = 0;
	int fx_;
	int fy_;
	int dir = 0;
	unsigned long pastTimeEnemy = 0;
	unsigned long delayEnemy = 0;
	unsigned long pastEnemy = 0;
	int limit;

	void draw() {
		if (item_time > 0) map[y][x] = 10;
		else map[y][x] = 5;
	}


	void move() {
		if (clock() - pastEnemy >= 400) {
			if (countDelay == delay) {
				if (ch == 0 && !collision(x, y - 1)) y--;
				if (ch == 1 && !collision(x, y + 1)) y++;
				if (ch == 2 && !collision(x - 1, y)) x--;
				if (ch == 3 && !collision(x + 1, y)) x++;
				ch = rand() % 4;
			}
			pastEnemy = clock();
		}
	}


	bool collision(int fx, int fy) {
		if (map[fy][fx] == 9) return true;
		return false;
	}
};

player pacman;
Enemy enemy[6];

void setup() {
	srand(time(NULL));
	pacman.x = 10;
	pacman.y = 15;
	pacman.ch = 'S';
	pacman.delay = 5;


	enemy[0].x = 2;
	enemy[0].y = 2;
	enemy[0].delayEnemy = 600;
	enemy[0].limit = 9;

	enemy[1].x = 15;
	enemy[1].y = 1;
	enemy[1].delayEnemy = 600;
	enemy[1].limit = 9;

	enemy[2].x = 10;
	enemy[2].y = 9;
	enemy[2].delayEnemy = 600;
	enemy[2].limit = 13;

	enemy[3].x = 5;
	enemy[3].y = 19;
	enemy[3].delayEnemy = 600;
	enemy[3].limit = 13;

	enemy[4].x = 18;
	enemy[4].y = 13;
	enemy[4].delayEnemy = 600;
	enemy[4].limit = 13;

	int x = rand() % 20;
	int y = rand() % 20;
	while (1) {
		if (stage[x][y] == '.' && is_exit == false) {
			stage[x][y] = '*';
			break;
		}
		x = rand() % 20;
		y = rand() % 20;
	}
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
	
	setcolor(RED,BLACK);
	gotoxy(56, 6); cout << "Life: " << life;
	setcolor(WHITE, BLACK);
	gotoxy(40, 6); cout << "Score: " << score;
	if (clock() - pastTime >= 1000) {
		gotoxy(47, 4); cout << "           ";
		setcolor(GREEN,BLACK);
		gotoxy(47, 4); cout << "Time : " << --Time;
		pastTime = clock();
	}
	
	setcolor(AQUA,BLACK);
	gotoxy(39, 9); cout << "-------------------------";
	setcolor(WHITE, BLACK);
	gotoxy(42, 11); cout << "V -> You";
	setcolor(RED,BLACK);
	gotoxy(42, 13); cout << char(234) << " -> Enemy";
	setcolor(YELLOW, BLACK);
	gotoxy(42, 15); cout << char(233) << " -> Coin";
	setcolor(PURPLE, BLACK);
	gotoxy(42, 17); cout << char(235) << " -> Item ";
	setcolor(WHITE, BLACK);
	gotoxy(42, 19); cout << "ESC -> Quit the game";
	layout();
	pacman.draw();
	if (item_time > 0) {
		if (item_time == 0) {
			item_time = 0;
		}
		if (clock() - pastTimeItem >= 1000 && item_time != 0) {
			gotoxy(40, 8); cout << "                  ";
			setcolor(PURPLE,BLACK);
			gotoxy(40, 8); cout << "Item Time : " << --item_time;
			pastTimeItem = clock();
		}
	}

	for (int i = 0; i < 5; i++) {
		enemy[i].draw();
	}
	for (int i = 0; i < ScreenHeight; i++) {
		for (int j = 0; j < ScreenWidth; j++) {
			setcolor(WHITE, BLACK);
			gotoxy(j + 15, i + 4);
			if (map[i][j] == 9) {
				setcolor(GREEN, BLACK);
				cout << char(178);  //wall
			}
			if (map[i][j] == 8) cout << ' '; // dot
			if (map[i][j] == 1) cout << char(86);
			if (map[i][j] == 2) cout << char(94);
			if (map[i][j] == 3) cout << char(62);
			if (map[i][j] == 4) cout << char(60);
			setcolor (RED, BLACK);  if (map[i][j] == 5) cout << char(234); // enermy
			if (map[i][j] == 0) cout << ' ';
			if (map[i][j] == 7) {
				setcolor(YELLOW, BLACK);
				cout << char(233);
			}
			setcolor(PURPLE, BLACK); if (map[i][j] == 6) cout << char(235);
			setcolor(GRAY, BLACK); if (map[i][j] == 10) cout << char(153);
			setcolor(WHITE, BLACK);
		}
	}

}

void input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 's':
			if (!pacman.collision(pacman.x, pacman.y + 1)) {
				pacman.ch = 'D';
				pacman.move(pacman.x, ++pacman.y);
				pacman.ismove = true;
			}
			break;
		case 'w':
			if (!pacman.collision(pacman.x, pacman.y - 1)) {
				pacman.ch = 'U';
				pacman.move(pacman.x, --pacman.y);
				pacman.ismove = true;
			}
			break;
		case 'a':
			if (!pacman.collision(pacman.x - 1, pacman.y)) {
				pacman.ch = 'L';
				pacman.move(--pacman.x, pacman.y);
				pacman.ismove = true;
			}
			break;
		case 'd':
			if (!pacman.collision(pacman.x + 1, pacman.y)) {
				pacman.ch = 'R';
				pacman.move(++pacman.x, pacman.y);
				pacman.ismove = true;
			}
			break;
		case char(27):
			is_exit = true;
			break;
		}
		fflush(stdin);
	}
	else {
		pacman.ismove = false;
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
	
	//ghostmove();
	for (int i = 0; i < 5; i++) {
		
		int difx = abs(pacman.x - enemy[i].x);
		int dify = abs(pacman.y - enemy[i].y);
		enemy[i].fx_ = enemy[i].x;
		enemy[i].fy_ = enemy[i].y;
			if (clock() - enemy[i].pastTimeEnemy >= enemy[i].delayEnemy) {
				if (difx > dify && difx != dify) {
					if (pacman.x > enemy[i].x) {
						if (!enemy[i].collision(enemy[i].fx_ + 1, enemy[i].fy_)) {
							enemy[i].x++;
						}
						else if (!enemy[i].collision(enemy[i].fx_, enemy[i].fy_ - 1) && enemy[i].dir == 2) {
							enemy[i].y--;
						}
						else if (!enemy[i].collision(enemy[i].fx_, enemy[i].fy_ + 1) && enemy[i].dir == 3) {
							enemy[i].y++;
						}
						else {
							enemy[i].dir = (rand() % 4) + 2;
						}
					}
					else if (pacman.x < enemy[i].x) {
						if (!enemy[i].collision(enemy[i].fx_ - 1, enemy[i].fy_)) {
							enemy[i].x--;
						}
						else if (!enemy[i].collision(enemy[i].fx_, enemy[i].fy_ - 1) && enemy[i].dir == 2) {
							enemy[i].y--;
						}
						else if (!enemy[i].collision(enemy[i].fx_, enemy[i].fy_ + 1) && enemy[i].dir == 3) {
							enemy[i].y++;
						}
						else {
							enemy[i].dir = (rand() % 4) + 2;
						}
					}
				}
				else if (difx < dify && difx != dify) {
					if (pacman.y > enemy[i].y) {
						if (!enemy[i].collision(enemy[i].fx_, enemy[i].fy_ + 1)) {
							enemy[i].y++;
						}
						else if (!enemy[i].collision(enemy[i].fx_ - 1, enemy[i].fy_) && enemy[i].dir == 0) {
							enemy[i].x--;
						}
						else if (!enemy[i].collision(enemy[i].fx_ + 1, enemy[i].fy_) && enemy[i].dir == 1) {
							enemy[i].x++;
						}
						else {
							enemy[i].dir = rand() % 2;
						}
					}
					else if (pacman.y < enemy[i].y) {
						if (!enemy[i].collision(enemy[i].fx_, enemy[i].fy_ - 1)) {
							enemy[i].y--;
						}
						else if (!enemy[i].collision(enemy[i].fx_ - 1, enemy[i].fy_) && enemy[i].dir == 0) {
							enemy[i].x--;
						}
						else if (!enemy[i].collision(enemy[i].fx_ + 1, enemy[i].fy_) && enemy[i].dir == 1) {
							enemy[i].x++;
						}
						else {
							enemy[i].dir = rand() % 2;
						}
					}
					
				}
				else {
					enemy[i].move();
				}
				Catch(enemy[i].y, enemy[i].x);
				enemy[i].pastTimeEnemy = clock();

			}
	}
}

void win() {
	hideCursor(1);
	gotoxy(14, 4); cout << "*******************************************";
	gotoxy(14, 5); cout << "*                                         *";
	gotoxy(14, 6); cout << "*               YOU WIN!!!                *";
	gotoxy(14, 7); cout << "*                                         *";
	gotoxy(14, 8); cout << "*  Your Score : " << score;
	gotoxy(50, 8); cout << "      *";
	gotoxy(14, 9); cout << "*                                         *";
	gotoxy(50, 10); cout << "      *";
	gotoxy(14, 11); cout << "*                                         *";
	gotoxy(14, 12); cout << "*                                         *";
	gotoxy(14, 13); cout << "*******************************************";
	gotoxy(14, 10); cout << "*  Enter your name : "; cin >> NameWin; 
	hideCursor(0);
	vector<pair<int, string>> userScore;
	fstream file("score.txt", ios::in);
	int scorefile;
	string name;

	while (file >> name >> scorefile) {
		userScore.push_back(make_pair(scorefile, name));
	}

	file.close();
	userScore.push_back(make_pair(score, NameWin)); // push back data in vector
	for (int i = userScore.size() - 2; i >= 0; i--) {
		if (userScore[i].second == userScore[userScore.size() - 1].second && userScore[i].first == userScore[userScore.size() - 1].first) {
			userScore.erase(userScore.begin() + i);
		}
	}
	sort(userScore.begin(), userScore.end());

	file.open("score.txt", ios::out);

	for (int i = userScore.size() - 1; i >= 0; i--) {
		file << userScore[i].second << " ";
		file << userScore[i].first << endl;
	}

	file.close();
	score = 0;
	gotoxy(14, 17); cout << "       PRESS ANY KEY TO GO BACK MENU       ";
	_getch();
	menu();
}


void lose() {
	gotoxy(14, 4); cout << "*******************************************";
	gotoxy(14, 5); cout << "*                                         *";
	gotoxy(14, 6); cout << "*                                         *";
	gotoxy(14, 7); cout << "*                                         *";
	gotoxy(14, 8); cout << "*              YOU LOSE !!!               *";
	gotoxy(14, 9); cout << "*                                         *";
	gotoxy(14, 10); cout << "*                                         *";
	gotoxy(14, 11); cout << "*                                         *";
	gotoxy(14, 12); cout << "*                                         *";
	gotoxy(14, 13); cout << "*******************************************";
	score = 0;
	gotoxy(14, 17); cout << "        PRESS  ESC  TO GO BACK MENU       ";
	if (_getch() == char(27)) {
		menu();
	}
	else {
		lose();
	}
	
}

void Score() {
	int n = 0;
	setcolor(WHITE, BLACK);
	vector<pair<int, string>> userScore;
	fstream file("score.txt", ios::in);
	int score;
	string name;

	while (file >> name >> score) {
		userScore.push_back(make_pair(score, name));
	}
	file.close();
	gotoxy(6, 4); cout << "********************** SCORE TABLE ***********************"<<endl;
	gotoxy(6, 6); cout << "         Name                              SCORE         "<< endl;
	for (int i = 0; i < 5; i++) {
		gotoxy(10, 8+n); cout << i + 1<<".";
		gotoxy(15, 8+n); cout << userScore[i].second;
		gotoxy(49, 8 + n); cout << userScore[i].first;
		n += 2;
	}
	gotoxy(6, 23); cout << "               PRESS ESC TO GO BACK THE  MENU               ";
	if (_getch() == char(27)) {
		menu();
	}
	else {
		Score();
	}

}

void playGame() {
	setup();
	int state = 1;
	while (state) {
		display();
		input();
		movement();
		if (is_exit == true) {
			for (int i = 0; i < ScreenHeight; i++) {
				for (int j = 0; j < ScreenWidth; j++) {
					if (stage[i][j] == '*') stage[i][j] = '.';
				}
			}
			is_exit = false;
			Time = SetTime;
			score = 0;
			menu();
			break;
		}
		if (Time == 0) {
			Time = SetTime;
			enemy[0].count = 0;
			enemy[1].count = 0;
			enemy[2].count = 0;
			enemy[3].count = 0;
			enemy[4].count = 0;
			for (int i = 0; i < ScreenHeight; i++) {
				for (int j = 0; j < ScreenWidth; j++) {
					if (stage[i][j] == '*') stage[i][j] = '.';
				}
			}
			system("CLS");
			win();
			state = 0;
		}

		if (life == 0) {
			life = 2;
			Time = SetTime;
			enemy[0].count = 0;
			enemy[1].count = 0;
			enemy[2].count = 0;
			enemy[3].count = 0;
			enemy[4].count = 0;
			for (int i = 0; i < ScreenHeight; i++) {
				for (int j = 0; j < ScreenWidth; j++) {
					if (stage[i][j] == '*') stage[i][j] = '.';
				}
			}
			system("CLS");
			lose();
			state = 0;
		}
	}
}

void menu() {
	char c = '0';
	int scroll = 0;
	bool exit = false;
	system("CLS");
	while (exit == false) {

		gotoxy(24,5);
		cout << "CATCH ME IF YOU CAN";

		setcolor(BLUE, BLACK);
		gotoxy(32, 10);
		cout << char(24);
		gotoxy(32, 11);
		cout << "W";

		gotoxy(32, 19);
		cout << char(25);
		gotoxy(32, 20);
		cout << "S";

		setcolor(WHITE, BLACK);
		gotoxy(30, 13);
		cout << "START";
		gotoxy(30, 15);
		cout << "SCORE";
		gotoxy(30, 17);
		cout << "QUIT ";
		while (c != char(13)) {
			setcolor(GREEN, BLACK);
			gotoxy(27, 13 + scroll);
			cout << "->";
			gotoxy(0, 0);
			c = _getch();
			gotoxy(27, 13 + scroll);
			cout << " ";
			gotoxy(27, 13 + scroll);
			cout << " ";
			cout << "  ";
			if (c == 's') {
				if (scroll < 4) {
					scroll += 2;
				}
			}
			if (c == 'w') {
				if (scroll > 0) {
					scroll -= 2;
				}
			}
		}
		if (scroll == 4) {  //press Enter
			exit = true; // exit
		}
		if (scroll == 2) {
			exit = true;
			system("CLS");
			Score();// score
		}
		if (scroll == 0) {
			exit = true;
			system("CLS");
			playGame();
			//paly game
		}
		scroll = 2;
		c = ' ';
		system("CLS");
	}
}



int main() {
	SetConsoleTitle(L"CATCH ME IF YOU CAN");
	SetConsoleSize(800, 700);
	hideCursor(0);
	SetConsoleFontSize(24,24);
	menu();
	//win();
	//Score();
	return 0;
}