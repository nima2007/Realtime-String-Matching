#include <iostream>
#include <vector>
//#include <algorithm>
#include "MooreSearch.h"
#include <string>
using namespace std;

//Note: only cstrings is working right now. c++ strings is not working yet.
//To get the result vector use getResult.
//To hide the couts in the class, edit the bool statement in the top of the h file
//to change max size, change that at the top of the header


int main() {

	string textstring = "Yo this is just a test it is not done yet just";
	string keystring = "is";

	const char * text = textstring.c_str();
	const char * key = keystring.c_str();


	int n = textstring.size();//size of text
	int m = keystring.size();//size of pattern

	BoyerMoore bm(key, m);
	//BoyerMoore bm(keystring); //string version (not working)

	bm.search(text, n);
	//bm.search(textstring); //string version

	bm.searchstarter(text, n);
	cout<< bm.searchstep();
	cout<<bm.searchstep();
	cout<<bm.searchstep();
	cout << bm.searchstep();
	cout << bm.searchstep();
	cout << bm.searchstep();
	cout << bm.searchstep();
	cout << bm.searchstep();
	cout << bm.searchstep();


	vector<int> resultfromsearch = bm.getResult(); //this gets the result


	return 0;
}