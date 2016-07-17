#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

const int NMAXSIZE = 256;
const bool shouldPrintTextToScreen = false; //set this to false to hide couts made by the program for debugging

//Nima Header File

//USAGE: First add the search pattern(what you're looking for)inside the constructor parameter when making the object.
//       Then call search, giving it the text to search in
//       You can use cstring or c++ string but if you use the cstring you have to give it a length too.


class BoyerMoore {
public:
	BoyerMoore(const char* pattern, int m); //uses cstring
	BoyerMoore(string patt) : BoyerMoore(patt.c_str(), patt.length()) {}               //uses cpp string
	void search(const char* text, int n);   //uses cstring
	void searchstarter(const char* text, int n);
	bool searchstep();
	void search(string text);               //uses cppstring

	vector<int> getResult() const { return result; } //result is stored in a vector that is returned
	void clearResult() { result.clear(); }
	int getIndex();
	int getPatIndex();
private:
	static const bool verbose = shouldPrintTextToScreen;
	vector<int> result;
	int d; // ASCII
	int m;
	int gn;
	const char* gtext;
	const char* pattern;

	int process_1[NMAXSIZE];
	vector<int> suffix_tabel;
	vector<int> offset_table;

	int function_bad_character(const char* text, int k, int i);
	int function_good_suffix(int j);

	int gi;
	int gj;
	bool forFlag;
	//int calculate_suffix(const char* str, int length, int j);
};