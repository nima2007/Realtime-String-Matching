#ifndef RK_H
#define RK_H

using namespace std;

template <typename T>
class RabinKarp
{
private:
	size_t M; //the size of pattern
	size_t N; //the size of text
	T* txt;   //text
	T* pat;   //search pattern
	int searchFound;
	size_t i, j;
	size_t p; // hash value for pattern
	size_t t; // hash value for txt
public:
	RabinKarp() : txt(nullptr), pat(nullptr), M(0), N(0), searchFound(-1), i(0), j(0), p(0), t(0)
	{
	}
	~RabinKarp()
	{
		delete[] txt;
		delete[] pat;
	}
	void add(T* txtIn, T* patIn, size_t m, size_t n)
	{
		pat = new char[m];
		txt = new char[n];
		strncpy(txt, txtIn, n);
		strncpy(pat, patIn, m);
		M = m;
		N = n;
		// Calculate the hash value of pattern and first window of text
		for (j = 0; j < M; j++)
		{
			p = hash(pat[0]);
			t = hash(txt[0]);
		}
	}
	size_t table_size()
	{
		return 101;
	}
	//Calculate the hash value of each charater
	size_t hash(T S)
	{
		return S % table_size();
	}
	void search()
	{
		size_t i, j;
		size_t p = 0; // hash value for pattern
		size_t t = 0; // hash value for txt
					  // Calculate the hash value of pattern and first window of text
		for (i = 0; i < M; i++)
		{
			p = hash(pat[i]);
			t = hash(txt[i]);
		}
		// Slide the pattern over text one by one
		for (i = 0; i <= N; i++)
		{
			// Check the hash values of current window of text
			// and pattern. If the hash values match then only
			// check for characters on by one
			if (p == t)
			{
				/* Check for characters one by one */
				for (j = 0; j < M; j++)
				{
					if (txt[i - M + j + 1] != pat[j])
						break;
				}

				// if p == t and pat[0...M-1] = txt[i - M + 1, ...i]
				if (j == M)
					cout << "Pattern found at index " << (i - M + 1) << endl;
			}
			// Calculate hash value for next window of text
			if (i < N)
			{
				t = hash(txt[i + 1]);
			}
		}
	}
	int searchNextFound()
	{
		size_t i, j;
		size_t p = 0; // hash value for pattern
		size_t t = 0; // hash value for txt
		// Calculate the hash value of pattern and first window of text
		for (i = 0; i < M; i++)
		{
			p = hash(pat[i]);
			t = hash(txt[i]);
		}
		// Slide the pattern over text one by one
		for (i = searchFound; i <= N; i++)
		{
			// Check the hash values of current window of text
			// and pattern. If the hash values match then only
			// check for characters on by one
			if (p == t)
			{
				/* Check for characters one by one */
				for (j = 0; j < M; j++)
				{
					if (txt[i - M + j + 1] != pat[j])
						break;
				}

				// if p == t and pat[0...M-1] = txt[i - M + 1, ...i]
				if (j == M)
				{
					cout << "Pattern found at index " << (i - M + 1) << endl;
					searchFound = i - M + 2;
					return i - M + 1;
				}
			}
			// Calculate hash value for next window of text
			if (i < N)
			{
				t = hash(txt[i + 1]);
			}
		}
	}
	int searchNext()
	{
		bool foundFlag = false;
		// Slide the pattern over text one by one
		if (i <= N)
		{
			// Check the hash values of current window of text
			// and pattern. If the hash values match then only
			// check for characters on by one
			if (p == t)
			{
				/* Check for characters one by one */
				for (j = 0; j < M; j++)
				{
					if (txt[i+j] != pat[j])
						break;
				}
				// if p == t and pat[0...M-1] = txt[i - M + 1, ...i]
				if (j == M)
					foundFlag = true;
			}
			// Calculate hash value for next window of text
			if (i < N)
			{
				t = hash(txt[i+1]);
			}
			i++;
			return foundFlag;
		}
		else
			return foundFlag;
	}
	size_t getIndex()
	{
		return i;
	}
};

#endif

//int main()
//{
//	char* txt = "Yo this is just a test it is not done yet just";
//	char* pat = "is";
//	RabinKarp<char> rk;
//	size_t M = strlen(pat);
//	size_t N = strlen(txt);
//	cout << "Length of source text is " << N << endl;
//	cout << "Length of search text is " << M << endl;
//	rk.add(txt, pat, M, N);
//	rk.search();
//	system("pause");
//	return 0;
//}