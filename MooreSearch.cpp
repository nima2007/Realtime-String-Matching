#include <iostream>
#include <vector>
#include <algorithm>
#include "MooreSearch.h"

using namespace std;


BoyerMoore::BoyerMoore(const char* pattern, int m)
	: d(NMAXSIZE), pattern(pattern), m(m) {

	// function 1 : bad character
	if (verbose) { cout << "INFO: Length of search text is " << m << endl << endl; }
	// We define for each possible character are right most occur in the pattern.
		// If it is not in the pattern then : -1
	for (int i = 0; i < d; i++)
		process_1[i] = -1;

	for (int i = 0; i < m; i++)
		process_1[pattern[i]] = i;

	// suffix table
	suffix_tabel.resize(m, 0);
	for (int i = m - 2; i >= 0; i--) {
		int current = suffix_tabel[i + 1];
		while (current > 0 && pattern[m - 1 - current] != pattern[i])
			current = suffix_tabel[m - 1 - current];
		if (pattern[m - 1 - current] == pattern[i])
			current++;
		suffix_tabel[i] = current;
	}

	// shift table, base on suffix table: the same suffix with the
	// minimal shift
	int i;
	offset_table.resize(suffix_tabel.size(), m - suffix_tabel[0] - 1);
	for (int j = suffix_tabel.size() - 1; j >= 0; j--) {
		i = m - suffix_tabel[j] - 1;
		offset_table[i] = offset_table[i] < (i + 1 - j) ? offset_table[i] : i + 1 - j;
	}

}

//BoyerMoore::BoyerMoore(string patt)   //can't do this in c++, so I did it in header
//{
//	const char * patcs = patt.c_str();
//	int payttlen = patt.size();
//	this->BoyerMoore(patcs, payttlen);
//}

int BoyerMoore::function_bad_character(const char* text, int k, int i) {
	// K is the position in the text, i is the position in the pattern.

	// This is the first (simple) function.
	// This searches the rightmost position of the character in text [pos].
	// We determine the offset so that a similar character is text [pos]
	int j = process_1[text[k]];
	int shift = i - j;

	// since j (rightmost index preventing the character) to the right of pos, then we would shift 0 (negative)
	// then shift just one
	if (shift < 1)
		shift = 1;

	return shift;
}

int BoyerMoore::function_good_suffix(int j) {
	// length m-i-1
	// case
	return offset_table[j];
}

void BoyerMoore::search(const char* text, int n) {

	std::vector<int> locaties;
	result.clear();//clear last run
	if (verbose) { cout << "INFO: Length of source text is " << n << endl<<endl; }

	int i = 0;
	while (i < n - m) {
		// From left to right walk the pattern and compare the corresponding characters
		int j;
		for (j = m - 1; j >= 0 && pattern[j] == text[i + j]; j--);
		if (j < 0) {
			// found it
			if (verbose) { cout << "Pattern found at index " << i << endl; }
			result.push_back(i);
			i++;
		}
		else {
			int h1 = function_bad_character(text, i + j, j);
			int h2 = function_good_suffix(j);
			i += max(h1, h2);
		}

	}
}

void BoyerMoore::searchstarter(const char* text, int n) {

	std::vector<int> locaties;
	result.clear();//clear last run
	if (verbose) { cout << "INFO: Length of source text is " << n << endl << endl; }
	gn = n;
	gtext = text;
	gi = 0;
	gj = 0;
	forFlag = true;
}

bool BoyerMoore::searchstep()
{
	if (gi < gn - m) {
		if (verbose) { cout << "while loop stepped" << endl; }
		// From left to right walk the pattern and compare the corresponding characters
		if (forFlag){
			gj = m - 1;
			forFlag = false;
		}
		while(gj >= 0 && pattern[gj] == gtext[gi + gj])
		{
			gj--;
			return false;
		}
		forFlag = true;
		if (gj < 0) {
			// found it
			if (verbose) { cout << "Pattern found at index " << gi << endl; }
			result.push_back(gi);
			gi++;
			return true;
		}
		else {
			int h1 = function_bad_character(gtext, gi + gj, gj);
			int h2 = function_good_suffix(gj);
			gi += max(h1, h2);
			return false;
		}

	}
	return false;
}

void BoyerMoore::search(string text)
{
	const char * textcs = text.c_str();
	int txtlen = text.length();
	search(textcs, txtlen);
}

int BoyerMoore::getIndex()
{
	return gi + m;
}
int BoyerMoore::getPatIndex()
{
	return gi + gj;
}