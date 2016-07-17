#ifndef SCREEN_H
#define SCREEN_H
#include <curses.h>
#include <string>
using namespace std;
extern char *choices[];

class Screen
{
protected:
	WINDOW *screenWin;
	int screenNum;
	int screenChange;
	int maxX;
	int maxY;
public:
	Screen(const int& numIn);
	int changeScreen();
	string readFile(string fileName);
};

class MainScreen : public Screen
{
private:
	int startx;
	int starty;
	int n_choices;
public:
	MainScreen(const int& numIn);
	void run();
	void print_menu(WINDOW *menu_win, int highlight);
};

class ScreenRK : public Screen
{
private:
	string text;
public:
	ScreenRK(const int& numIn);
	void run();
};

class ScreenKMP : public Screen
{
private:
	string text;
public:
	ScreenKMP(const int& numIn);
	void run();
};

class ScreenBM : public Screen
{
private:
	string text;
public:
	ScreenBM(const int& numIn);
	void run();
};

#endif