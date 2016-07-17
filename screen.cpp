#include "screen.h"
#include <curses.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include "rk.h"
#include "kmp.h"
#include "MooreSearch.h"
using namespace std;

int CHECKSPEED = 150;

char *choices[] = {
	"Rabin-Karp",
	"Knuth-Morris-Pratt",
	"Booyre-Moore",
	"Exit",
};

/********* colorString Class *********/

class colorString
{
private:
	char* cString;
	int* formatArray;
	size_t searchPos;
	size_t patPos;
	size_t length;
	size_t patternLength;
public:
	colorString(char* stringIn, size_t lengthIn, size_t patternLengthIn)
	{
		length = lengthIn+1;
		cString = new char[length];
		strcpy(cString, stringIn);
		patternLength = patternLengthIn;
		formatArray = new int[length] {0};
		searchPos = -1;
		patPos = -1;
	}
	~colorString()
	{
		delete[] cString;
		delete[] formatArray;
	}
	void print(WINDOW *winIn, int posY, int posX)
	{
		wmove(winIn, posY, posX);
		for (size_t i = 0; i < length; i++)
		{
			if (i == searchPos && cString[i] != '\n')
				waddch(winIn, '_' | A_STANDOUT);
			else if (i >= patPos && i < patPos + patternLength && cString[i] != '\n')
				waddch(winIn, '_');
			else if (formatArray[i] == 0)
				waddch(winIn, cString[i]);
			else
				waddch(winIn, cString[i] | A_BOLD | A_UNDERLINE);
		}
	}
	void printBM(WINDOW *winIn, int posY, int posX)
	{
		wmove(winIn, posY, posX);
		for (size_t i = 0; i < length; i++)
		{
			if (i == patPos && cString[i] != '\n')
				waddch(winIn, '_' | A_STANDOUT);
			else if (i >= searchPos - patternLength && i < searchPos && cString[i] != '\n')
				waddch(winIn, '_');
			else if (formatArray[i] == 0)
				waddch(winIn, cString[i]);
			else
				waddch(winIn, cString[i] | A_BOLD | A_UNDERLINE);
		}
	}
	void setColor(int elementIn, bool state)
	{
		if (state)
			formatArray[elementIn] = 1;
		else
			formatArray[elementIn] = 0;
	}
	void setSearchPos(int posIn)
	{
		searchPos = posIn;
	}
	void setPatPos(int patPosIn)
	{
		patPos = patPosIn;
	}
};

/********* Parent Screen Class *********/

Screen::Screen(const int& numIn)
{
	screenNum = numIn;
	screenChange = numIn;
}
int Screen::changeScreen()
{
	int temp = screenChange;
	screenChange = screenNum;
	return temp;
}
string Screen::readFile(string fileName)
{
	ifstream file(fileName);
	ostringstream ss;
	if (file.is_open())
	{
		ss << file.rdbuf();
	}
	else
	{
		cerr << "Unable to open file: " << fileName << endl;
		return "";
	}
	file.close();
	return ss.str();
}

/********* Main Screen Class *********/

MainScreen::MainScreen(const int& numIn) : Screen(numIn)
{
	n_choices = sizeof(choices) / sizeof(char *);
}
void MainScreen::run()
{
	int WIDTH = 25;
	int HEIGHT = 6;
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	string printBuffer;
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);
	menu_win = newwin(HEIGHT, WIDTH, maxY/2 - HEIGHT/2, maxX/2 - WIDTH/2);
	keypad(menu_win, TRUE);

	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	curs_set(0);

	// Print Title
	printBuffer = "|| | | || BARCODES ||| || ||";
	mvprintw(maxY / 2 - HEIGHT / 2 - 3, maxX / 2 - printBuffer.length() / 2, printBuffer.c_str());
	printBuffer = "String Matching Presentation";
	mvprintw(maxY / 2 - HEIGHT / 2 - 2, maxX / 2 - printBuffer.length() / 2, printBuffer.c_str());
	// Print Footer
	printBuffer = "Nima Vasseghi | Xince Shi | David Park | Damon Birtola";
	mvprintw(maxY / 2 + n_choices / 2 + 2, maxX / 2 - printBuffer.length() / 2, printBuffer.c_str());
	refresh();
	// Print Interactive Menu
	print_menu(menu_win, highlight);
	// Keyboard Interaction
	while (1)
	{
		c = wgetch(menu_win);
		switch (c)
		{
		case KEY_UP:
			if (highlight == 1)
				highlight = n_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case '\r': case '\n': case KEY_ENTER:
			choice = highlight;
			screenChange = choice;
			break;
		default:
			break;
		}
		print_menu(menu_win, highlight);
		if (screenChange != screenNum)	/* User did a choice come out of the infinite loop */
			break;
	}
	delwin(menu_win);
}
void MainScreen::print_menu(WINDOW *menu_win, int highlight)
{
	int maxY, maxX, i;
	getmaxyx(menu_win, maxY, maxX);
	box(menu_win, 0, 0);

	// Print Options
	int y = maxY / 2 - n_choices / 2;
	int x;
	for (i = 0; i < n_choices; ++i)
	{
		x = maxX / 2 - strlen(choices[i]) / 2;
		if (highlight == i + 1) /* High light the present choice */
		{
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

/********* ScreenRK Class *********/

ScreenRK::ScreenRK(const int& numIn) : Screen(numIn)
{
}
void ScreenRK::run()
{
	//Pad Creation
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);
	WINDOW *rkWin;
	rkWin = newpad(1000, maxX);
	//Read file to string
	text = readFile("Speech.txt");
	//count lines to be displayed from text
	int textLines = 0;
	int linePos = 0;
	for (char& c : text) {
		if (c == '\n')
		{
			textLines++;
			linePos = 0;
		}
		else
		{
			if (linePos < maxX)
				linePos++;
			else
			{
				textLines++;
				linePos = 0;
			}
		}
	}
	if (textLines > maxY)
		textLines -= maxY;
	textLines += 5;
	// Rabin Karp Algorithm Object
	char* txt = _strdup(text.c_str());
	char* pat = "the";
	size_t M = strlen(pat);
	size_t N = strlen(txt);
	RabinKarp<char> rk;
	rk.add(txt, pat, M, N);
	// Create 
	colorString colorText(txt, N, M);
	colorText.setSearchPos(0);
	colorText.print(rkWin, 0, 0);
	prefresh(rkWin, 0, 0, 0, 0, maxY-1, maxX-1);
	//Input Loop
	keypad(rkWin, TRUE);
	nodelay(rkWin, TRUE);
	int padPos = 0;
	size_t textIndex = 0;
	int c;
	bool exitFlag = false;
	while (1)
	{
		c = wgetch(rkWin);
		switch (c)
		{
		case KEY_UP:
			if (padPos >0)
				padPos -= 1;
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		case KEY_DOWN:
			if (padPos < textLines)
				padPos += 1;
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		case '\r': case '\n': case KEY_ENTER:
			screenChange = 0;
			exitFlag = true;
		default:
			textIndex = rk.getIndex();
			colorText.setSearchPos(textIndex);
			if (rk.searchNext())
			{
				for (size_t i = textIndex; i < M + textIndex; i++)
					colorText.setColor(i, true);
			}
			colorText.print(rkWin, 0, 0);
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		}
		if (exitFlag)
			break;
		Sleep(CHECKSPEED);
	}
	delete[] txt;
	delwin(rkWin);
}

/********* ScreenKMP Class *********/

ScreenKMP::ScreenKMP(const int& numIn) : Screen(numIn)
{
}
void ScreenKMP::run()
{
	//Pad Creation
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);
	WINDOW *rkWin;
	rkWin = newpad(1000, maxX);
	//Read file to string
	text = readFile("Speech.txt");
	//count lines to be displayed from text
	int textLines = 0;
	int linePos = 0;
	for (char& c : text) {
		if (c == '\n')
		{
			textLines++;
			linePos = 0;
		}
		else
		{
			if (linePos < maxX)
				linePos++;
			else
			{
				textLines++;
				linePos = 0;
			}
		}
	}
	if (textLines > maxY)
		textLines -= maxY;
	textLines += 5;
	//KMP object
	char* txt = _strdup(text.c_str());
	char* pat = "temporary";
	size_t M = strlen(pat);
	size_t N = strlen(txt);
	KMP kmp(pat, strlen(pat), txt, text.length());
	// Create 
	colorString colorText(txt, N, M);
	colorText.setSearchPos(0);
	colorText.print(rkWin, 0, 0);
	prefresh(rkWin, 0, 0, 0, 0, maxY - 1, maxX - 1);
	//Input Loop
	keypad(rkWin, TRUE);
	nodelay(rkWin, TRUE);
	int padPos = 0;
	size_t textIndex = 0;
	size_t patIndex = 0;
	int c;
	bool exitFlag = false;
	while (1)
	{
		c = wgetch(rkWin);
		switch (c)
		{
		case KEY_UP:
			if (padPos >0)
				padPos -= 1;
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		case KEY_DOWN:
			if (padPos < textLines)
				padPos += 1;
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		case '\r': case '\n': case KEY_ENTER:
			screenChange = 0;
			exitFlag = true;
		default:
			textIndex = kmp.getIndex();
			patIndex = kmp.getPatIndex();
			colorText.setSearchPos(textIndex);
			colorText.setPatPos(patIndex);
			if (kmp.findNext())
			{
				for (size_t i = textIndex; i < M + textIndex+1; i++)
					colorText.setColor(i-M, true);
			}
			colorText.print(rkWin, 0, 0);
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		}
		if (exitFlag)
			break;
		Sleep(CHECKSPEED);
	}
	delete[] txt;
	delwin(rkWin);
}

/********* ScreenBM Class *********/

ScreenBM::ScreenBM(const int& numIn) : Screen(numIn)
{
}
void ScreenBM::run()
{
	//Pad Creation
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);
	WINDOW *rkWin;
	rkWin = newpad(1000, maxX);
	//Read file to string
	text = readFile("Speech.txt");
	//count lines to be displayed from text
	int textLines = 0;
	int linePos = 0;
	for (char& c : text) {
		if (c == '\n')
		{
			textLines++;
			linePos = 0;
		}
		else
		{
			if (linePos < maxX)
				linePos++;
			else
			{
				textLines++;
				linePos = 0;
			}
		}
	}
	if (textLines > maxY)
		textLines -= maxY;
	textLines += 5;
	//BM object
	string patString = "the";
	const char * cText = text.c_str();
	const char * cPat = patString.c_str();
	int n = text.size();//size of text
	int m = patString.size();//size of pattern
	BoyerMoore bm(cPat, m);
	bm.searchstarter(cText, n);
	char *txt;
	txt = new char[n+1];
	strcpy(txt, cText);
	// Create 
	colorString colorText(txt, n, m);
	colorText.setSearchPos(0);
	colorText.printBM(rkWin, 0, 0);
	prefresh(rkWin, 0, 0, 0, 0, maxY - 1, maxX - 1);
	//Input Loop
	keypad(rkWin, TRUE);
	nodelay(rkWin, TRUE);
	int padPos = 0;
	size_t textIndex = 0;
	size_t patIndex = 0;
	int c;
	bool exitFlag = false;
	while (1)
	{
		c = wgetch(rkWin);
		switch (c)
		{
		case KEY_UP:
			if (padPos >0)
				padPos -= 1;
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		case KEY_DOWN:
			if (padPos < textLines)
				padPos += 1;
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		case '\r': case '\n': case KEY_ENTER:
			screenChange = 0;
			exitFlag = true;
		default:
			textIndex = bm.getIndex();
			patIndex = bm.getPatIndex();
			colorText.setSearchPos(textIndex);
			colorText.setPatPos(patIndex);
			if (bm.searchstep())
			{
				for (size_t i = textIndex - m; i < textIndex; i++)
					colorText.setColor(i, true);
			}
			colorText.printBM(rkWin, 0, 0);
			prefresh(rkWin, padPos, 0, 0, 0, maxY - 1, maxX - 1);
			break;
		}
		if (exitFlag)
			break;
		Sleep(CHECKSPEED);
	}
	delwin(rkWin);
	delete[] txt;
}
