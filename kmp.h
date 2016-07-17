#ifndef KMP_H
#define KMP_H

class KMP {
private:
	int *lps;
	char *pat;
	char *txt;
	int M, N;
	int i, j;
	void computeLPSArray()
	{
		int len = 0;  // length of the previous longest prefix suffix
		int i;

		lps[0] = 0; // lps[0] is always 0
		i = 1;

		// the loop calculates lps[i] for i = 1 to M-1
		while (i < M)
		{
			if (pat[i] == pat[len])
			{
				len++;
				lps[i] = len;
				i++;
			}
			else // (pat[i] != pat[len])
			{
				if (len != 0)
				{
					// This is tricky. Consider the example 
					// AAACAAAA and i = 7.
					len = lps[len - 1];

					// Also, note that we do not increment i here
				}
				else // if (len == 0)
				{
					lps[i] = 0;
					i++;
				}
			}
		}
	}
	bool matchFlag;
public:
	KMP(char *patIn, int mIn, char *txtIn, int nIn) : i(0), j(0)
	{
		M = mIn;
		N = nIn;
		pat = new char[M];
		txt = new char[N];
		strncpy(pat, patIn, mIn);
		strncpy(txt, txtIn, nIn);
		lps = new int[M];
		computeLPSArray();
		matchFlag = false;
	}
	~KMP()
	{
		delete lps;
		delete pat;
		delete txt;
	}
	bool findNext() {
		bool foundFlag = false;
		if (i < N)
		{
			if (matchFlag)
			{
				j = lps[j - 1];
				matchFlag = false;
			}
			if (pat[j] == txt[i])
			{
				j++;
				i++;
			}

			if (j == M)
			{
				foundFlag = true;
				j = lps[j - 1];
			}

			// mismatch after j matches
			else if (i < N && pat[j] != txt[i])
			{
				// Do not match lps[0..lps[j-1]] characters,
				// they will match anyway
				if (j != 0)
				{
					matchFlag = true;
					return false;
				}
				else
					i = i + 1;
			}
		}
		return foundFlag;
	}
	size_t getIndex()
	{
		return i;
	}
	size_t getPatIndex()
	{
		return i - j;
	}
};

#endif