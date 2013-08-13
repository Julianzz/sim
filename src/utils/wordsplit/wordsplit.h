#ifndef WORDSPLIT_H_
#define WORDSPLIT_H_

#include <string>
#include <vector>
#include <pthread.h>
#include "algor.h"

using namespace std;

struct Token
{
	const char *text;
	int offset;
	int length;
};

class WordSplit
{
public:
	int load(const vector<string> &dicList);
	int split(const string &text, vector<string> &result);
	
	~WordSplit();

private:
//	rmmseg::Algorithm * __alg;
	Token mmseg_next_token(rmmseg::Algorithm &__alg);
};

#endif 
