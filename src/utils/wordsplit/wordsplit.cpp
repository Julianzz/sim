#include "wordsplit.h"
#include "token.h"
#include "dict.h"
#include "algor.h"
#include <iostream>

int WordSplit::load(const vector<string> &dicList) {
	vector<string>::const_iterator it = dicList.begin();
	for (; it != dicList.end(); ++it) {
		if (!rmmseg::dict::load_words((*it).c_str()))
			return -1;
	}
	return 0;
}

int WordSplit::split(const string &text, vector<string> &result) {
	result.clear();
	if (text.length() <= 2) {
		result.push_back(text);
		return 0;
	}

//	rmmseg::Algorithm* __alg = new rmmseg::Algorithm(text.c_str(), text.size());
	rmmseg::Algorithm __alg(text.c_str(), text.size());
	Token tk_last = mmseg_next_token(__alg);

	while (1) {
		Token tk_cur = mmseg_next_token(__alg);
		if (tk_cur.length == 0) {
			string temp = tk_last.text;
//			string temp(tk_last.text,
//					strlen(__alg.get_text()) - tk_last.offset);
			result.push_back(temp);
			break;
		}
		string temp(tk_last.text, tk_cur.offset - tk_last.offset);
		result.push_back(temp);
		tk_last = tk_cur;
	}
	//delete __alg;

//	result.push_back(text);
	return 0;
}

//Token WordSplit::mmseg_next_token(rmmseg::Algorithm * &__alg) {
//	rmmseg::Token rtk = __alg->next_token();
//	Token tk;
//	tk.text = rtk.text;
//	tk.offset = rtk.text - __alg->get_text();
//	tk.length = rtk.length;
//	return tk;
//}

Token WordSplit::mmseg_next_token(rmmseg::Algorithm &__alg) {
	rmmseg::Token rtk = __alg.next_token();
	Token tk;
	tk.text = rtk.text;
	tk.offset = rtk.text - __alg.get_text();
	tk.length = rtk.length;
	return tk;
}

WordSplit::~WordSplit() {
}
