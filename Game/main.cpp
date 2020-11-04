#pragma GCC optimize("Ofast")       
#pragma comment(lib, "lwinmm.lib")  //Play music
#include <iostream>
#include <fstream>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

#define PAINT1 char(219)          // Paint 1 vi tri la kí tu 219 trong ASCII 
#define PAINT2 PAINT1 << PAINT1   // Paint 2 vi tri
#define PAINT3 PAINT2 << PAINT1   // Paint 3 vi tri
#define PAINT4 PAINT3 << PAINT1   // Paint 4 vi tri
#define PAINT5 PAINT4 << PAINT1   // Paint 5 vi tri

//Mã ASCII cua cac phim Up, Down, Right, Left, ESC, Enter
#define KB_ENTER 13    
#define KB_ESC 27
#define KB_UP 72
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_DOWN 80

//Vi tri cua cac ô trong Board
#define X (j - 1) * 20 + 1
#define Y (i - 1) * 10
#define endl '\n'

// Function of Color
void colorFont(int n){
	HANDLE color;
	color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, n);
}

// Function of resize Console
void resizeConsole(int width, int height){
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

// Function of Teleport Point
void gotoxy(short x, short y){
	HANDLE color;
	color = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cal = {x, y};
	SetConsoleCursorPosition(color, cal);
}

// Function of Write Score to File text
void writeDataToFile(string file, long long score){
	ofstream coutFile(file);
	if(coutFile.fail()){
		colorFont(12);	cout << "Warning: Can't find file HighScore.txt. Your high score won't be save !";
		colorFont(15);	Sleep(2000);		return;
	}
	coutFile << score;
}

// Function of Reiceve High Score to game
void readDataFromFile(string file, long long &fileScore){
	ifstream cinFile(file);
	if(cinFile.fail()){
		colorFont(12);	cout << "Warning: Can't find file HighScore.txt. Your high score won't be save !";
		colorFont(15);	Sleep(2000);		
		return;
	}
	cinFile >> fileScore;
}

// OOP
class GAME{
	private:
		string filePath = "HighScore.txt";
		long long score;
		int n, _2048[100][100], turn, add, accept, choice, gameOver, select;
	public:
		long long highScore = -1;
		GAME(){
			n = 3;	turn = 1;	score = 0;	gameOver = 0;	select = 1;
			readDataFromFile(filePath, highScore);
		}
		void menu();
		void main();
		void guide();
		void credit();
		void makeBoard();
		void generate();
		void level();
		int random();
		void display();
		void writeScore();
		void move();
		void checkGameOver();
		void endGame();
};

void GAME::menu(){
	MAIN:;
	system("cls");
	PlaySound(TEXT("Music.wav"), NULL, SND_ASYNC);
	credit();
	colorFont(10);
	gotoxy(98, 15);		cout << "  ______    _______       __     _______     ";
	gotoxy(98, 16);		cout << " /  __  \\  /  ___  \\     /  |   /  ___  \\ ";
	gotoxy(98, 17);		cout << " |_|  \\  | | /   \\ |    /   |   | /   \\ | ";
	gotoxy(98, 18);		cout << "      |  | | |   | |   / /| |   | \\___/ |   ";
	gotoxy(98, 19);		cout << "     /  /  | |   | |  / /_| |_   \\     /    ";
	gotoxy(98, 20);		cout << "    /  /   | |   | | |____   _|  / ___ \\    ";
	gotoxy(98, 21);		cout << "   /  /    | |   | |      | |   | /   \\ |   ";
	gotoxy(98, 22);		cout << "  /  /___  | \\___/ |      | |   | \\___/ |  ";
	gotoxy(98, 23);		cout << " |_______| \\_______/      |_|   \\_______/  ";
	gotoxy(99, 25);		cout << "             _*-*_MENU_*-*_                  ";
	gotoxy(99, 27);		cout << "               Play Game                     ";
	gotoxy(99, 29);		cout << "               Quit                          ";
	colorFont(15);
	MENU:;
	switch(select){
		case 1:
			gotoxy(111, 27);	colorFont(14);		cout << ">> Play Game";		colorFont(10);		gotoxy(0, 0);
			switch(_getch()){
				case KB_ENTER:	colorFont(14);
					for (auto i = 1; i <= 13; i ++){
						gotoxy(106 + i, 27);	cout << "     >>";
						gotoxy(0, 0);		Sleep(50);
					}
					gotoxy(114, 27);	colorFont(10);		cout << "Play Game     ";
					colorFont(10);		gameOver = 0;		PlaySound(NULL , NULL, SND_ASYNC);		main();			goto MAIN;
				case KB_DOWN :		select = 2;		break;
				default:		goto MENU;
			}
			gotoxy(109, 27);		cout << "     Play Game     ";		goto MENU;
		case 2:
			gotoxy(111, 29);	colorFont(14);		cout << ">> Quit";			colorFont(10);		gotoxy(0, 0);
			switch(_getch()){
				case KB_ENTER:	colorFont(14);
					for (auto i = 1; i <= 13; i ++){
						gotoxy(106 + i, 29);	cout << "     >>";
						gotoxy(0, 0);		Sleep(50);
					}
					gotoxy(111, 29);		cout << ">> Quit          ";		colorFont(10);		gotoxy(0, 0);	goto END;
				case KB_UP:			select = 1;		break;
				default:		goto MENU;
			}
			gotoxy(109, 29);		cout << "     Quit     ";			goto MENU;
	}
	END:;
}

//	Function Main Game
void GAME::main(){
	PlaySound(TEXT("Music2048.wav"), NULL, SND_LOOP | SND_ASYNC);
	makeBoard();
//	_2048[1][1] = 2;	_2048[1][2] = 4;	_2048[1][3] = 8;	_2048[1][4] = 16;	_2048[2][1] = 32;	_2048[2][2] = 64;	_2048[2][3] = 128;	_2048[2][4] = 256;	_2048[3][1] = 512;
	generate();
	guide();
	TURN:;
		generate();
		display();
		writeScore();
		checkGameOver();
		if(gameOver){	
		    endGame();
			goto END;
		}
		REINPUT:;  // Play Game
			accept = 0;
			switch(_getch()){
				case KB_UP:		choice = 1;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_RIGHT:	choice = 2;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_DOWN:	choice = 3;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_LEFT:	choice = 4;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_ESC:	score = 0;		goto END;
				default:		goto REINPUT;
			}
	END:;
	PlaySound(NULL , NULL, SND_ASYNC);
}

void GAME::guide(){
	gotoxy(83, 4);		cout << "ESC: Exit Game";
	gotoxy(0, 0);
}

//	Function of Credit
void GAME::credit(){
	colorFont(14);
	gotoxy(80, 1)  ;    cout << "Subject : Project I - 20201 ";
	colorFont(15);
	gotoxy(139, 39);	cout << "2048 - Made by Elephant";
	gotoxy(118, 40);	cout << "      Best for you " << '\xa9' << " 2020 Oct. All Rights Reserved";
	gotoxy(0, 0);
}

//	Function of Make Board
void GAME::makeBoard(){
	colorFont(8);		gotoxy(0, 0);
	for(auto i = 1; i <= n; i ++)
		for(auto j = 1; j <= n; j ++)
			_2048[i][j] = 0;
	for(auto i = 1; i <= n * 10 + 1; i ++){
		for(auto j = 1; j <= n * 20 + 1; j ++)
			if(!((i - 1) % 10) || ((i - 1) % 10 && !((j - 1) % 20)))
				cout << PAINT1;
			else cout << ' ';
		cout << endl;
	}
}

//	Function : Random ô 2 và 4 diem
void GAME::generate(){
	int seed, i ,j;
	!(turn % 5) ? add = 4 : add = 2;
	turn ++;
	while(true){
		seed = random();
		i = seed/n + 1;	j = seed%n + 1;
		if(!(_2048[i][j])){		_2048[i][j] = add;		break;		}
	}
}

// Function of Level in Game
void GAME::level() {
}

//	Function of Random Seed, choose 1 cell in n*n cell
int GAME::random(){
	return rand() % (n * n); 
}

//	Function of point on Board
void GAME::display(){
	for(auto i = 1; i <= n; i ++)
		for(auto j = 1; j <= n; j ++)
			switch(_2048[i][j]){
				case 0:
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "                   ";
					gotoxy(X, Y + 4);		cout << "                   ";
					gotoxy(X, Y + 5);		cout << "                   ";
					gotoxy(X, Y + 6);		cout << "                   ";
					gotoxy(X, Y + 7);		cout << "                   ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					break;
				case 2:
					colorFont(240);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 4);		cout << "           " << PAINT1 << "       ";
					gotoxy(X, Y + 5);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 6);		cout << "       " << PAINT1 << "           ";
					gotoxy(X, Y + 7);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 4:
					colorFont(112);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 4);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 5);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 6);		cout << "           " << PAINT1 << "       ";
					gotoxy(X, Y + 7);		cout << "           " << PAINT1 << "       ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 8:
					colorFont(48);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 4);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 5);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 6);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 7);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 16:
					colorFont(176);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "      " << PAINT1 << " " << PAINT5 << "      ";
					gotoxy(X, Y + 4);		cout << "      " << PAINT1 << " " << PAINT1 << "          ";
					gotoxy(X, Y + 5);		cout << "      " << PAINT1 << " " << PAINT5 << "      ";
					gotoxy(X, Y + 6);		cout << "      " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << "      ";
					gotoxy(X, Y + 7);		cout << "      " << PAINT1 << " " << PAINT5 << "      ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 32:
					colorFont(32);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 4);		cout << "        " << PAINT1 << "     " << PAINT1 << "    ";
					gotoxy(X, Y + 5);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 6);		cout << "        " << PAINT1 << " " << PAINT1 << "        ";
					gotoxy(X, Y + 7);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 64:
					colorFont(160);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "    " << PAINT5 << " " << PAINT1 << "   " << PAINT1 << "    ";
					gotoxy(X, Y + 4);		cout << "    " << PAINT1 << "     " << PAINT1 << "   " << PAINT1 << "    ";
					gotoxy(X, Y + 5);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 6);		cout << "    " << PAINT1 << "   " << PAINT1 << "     " << PAINT1 << "    ";
					gotoxy(X, Y + 7);		cout << "    " << PAINT5 << "     " << PAINT1 << "    ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 128:
					colorFont(16);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 4);		cout << "   " << PAINT1 << "     " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << "   ";
					gotoxy(X, Y + 5);		cout << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 6);		cout << "   " << PAINT1 << " " << PAINT1 << "     " << PAINT1 << "   " << PAINT1 << "   ";
					gotoxy(X, Y + 7);		cout << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 256:
					colorFont(144);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << " " << PAINT5 << " " << PAINT5 << " " << PAINT5 << " ";
					gotoxy(X, Y + 4);		cout << "     " << PAINT1 << " " << PAINT1 << "     " << PAINT1 << "     ";
					gotoxy(X, Y + 5);		cout << " " << PAINT5 << " " << PAINT5 << " " << PAINT5 << " ";
					gotoxy(X, Y + 6);		cout << " " << PAINT1 << "         " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << " ";
					gotoxy(X, Y + 7);		cout << " " << PAINT5 << " " << PAINT5 << " " << PAINT5 << " ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 512:
					colorFont(96);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "   " << PAINT5 << " " << PAINT1 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 4);		cout << "   " << PAINT1 << "     " << PAINT1 << "     " << PAINT1 << "   ";
					gotoxy(X, Y + 5);		cout << "   " << PAINT5 << " " << PAINT1 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 6);		cout << "       " << PAINT1 << " " << PAINT1 << " " << PAINT1 << "       ";
					gotoxy(X, Y + 7);		cout << "   " << PAINT5 << " " << PAINT1 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 1024:      break;				
				case 2048:		break;			
				case 4096:		break; 		
				case 8192:		break;			
				case 16384:		break;
				case 32768:		break;
				default:		return;
			}
}

//	Function write Score after 1 turn
void GAME::writeScore(){
	gotoxy(105, 5);		cout << "High Score: " << highScore;
	gotoxy(110, 7);		cout << "Score: " << score;
}

//	Function of move
void GAME::move(){
	int location, checkSpace;
	switch(choice){
		case 1:		// Up
			for(auto j = 1; j <= n; j ++){
				checkSpace = 0;
				for(auto i = 1; i <= n; i ++){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto j = 1; j <= n; j ++){
				location = 1;
				for(auto i = 1; i <= n; i ++){
					if(!_2048[i][j])	continue;
					for(auto k = i + 1; k <= n; k ++){
						if(!_2048[k][j])	continue;
						if(_2048[i][j] == _2048[k][j]){		_2048[i][j] *= 2;	_2048[k][j] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != i){	_2048[location][j] = _2048[i][j];	_2048[i][j] = 0;	}
					location ++;
				}
			}
			break;
		case 2:		//	Right
			for(auto i = 1; i <= n; i ++){
				checkSpace = 0;
				for(auto j = n; j >= 1; j --){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto i = 1; i <= n; i ++){
				location = n;
				for(auto j = n; j >= 1; j --){
					if(!_2048[i][j])	continue;
					for(auto k = j - 1; k >= 1; k --){
						if(!_2048[i][k])	continue;
						if(_2048[i][j] == _2048[i][k]){		_2048[i][j] *= 2;	_2048[i][k] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != j){	_2048[i][location] = _2048[i][j];	_2048[i][j] = 0;	}
					location --;
				}
			}
			break;
		case 3:		//	Down
			for(auto j = 1; j <= n; j ++){
				checkSpace = 0;
				for(auto i = n; i >= 1; i --){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto j = 1; j <= n; j ++){
				location = n;
				for(auto i = n; i >= 1; i --){
					if(!_2048[i][j])	continue;
					for(auto k = i - 1; k >= 1; k --){
						if(!_2048[k][j])	continue;
						if(_2048[i][j] == _2048[k][j]){		_2048[i][j] *= 2;	_2048[k][j] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != i){	_2048[location][j] = _2048[i][j];	_2048[i][j] = 0;	}
					location --;
				}
			}
			break;
		case 4:		//	Left
			for(auto i = 1; i <= n; i ++){
				checkSpace = 0;
				for(auto j = 1; j <= n; j ++){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto i = 1; i <= n; i ++){
				location = 1;
				for(auto j = 1; j <= n; j ++){
					if(!_2048[i][j])	continue;
					for(auto k = j + 1; k <= n; k ++){
						if(!_2048[i][k])	continue;
						if(_2048[i][j] == _2048[i][k]){		_2048[i][j] *= 2;	_2048[i][k] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != j){	_2048[i][location] = _2048[i][j];	_2048[i][j] = 0;	}
					location ++;
				}
			}
			break;
		default:	break;
	}
}

//	Function of check game over
void GAME::checkGameOver(){
	
}

//	Function of write High Score
void GAME::endGame(){
	if(score > highScore){
		gotoxy(97, 10);		cout << "Congratulation ! You achieve new high score !";
		writeDataToFile(filePath, score);
	}
	gotoxy(105, 12);	cout << "Game over. Your score is: " << score;
	gotoxy(100, 13);	cout << "Press any key to return to main menu...";
	score = 0;			
	_getch();
}

//	main function code start running
int main(int argc, char *argv[]){
	cin.tie(NULL);		cout.tie(NULL);	//	Fast input and output
	srand(time(0));						//	Random Number
	cout.setf(ios_base::boolalpha);		//	Output boolean 'true' and 'false' instead of '1' and '0'
	resizeConsole(1280,720);			//	Resize of Console
	GAME play;			play.menu();
	return 0;
}
