#ifndef  HTMLIMAGEBROWSERCREATOR_H
#define  HTMLIMAGEBROWSERCREATOR_H

// options
#include "goptions.h"

// c++
#include <string>
#include <vector>
using namespace std;


class HtmlVariable {
	
public:
	HtmlVariable(string tit, string filename);
	
	string title;
	vector<string> present;
	
};

class Page {
	
public:
	Page(int argc, char *argv[]);
	
	
private:
	GOptions *gopts;
	map<string, GOption> defineOptions();
	

	vector<HtmlVariable> rows;
	vector<HtmlVariable> coumns;
	vector<HtmlVariable> selections;
	
};

#endif
