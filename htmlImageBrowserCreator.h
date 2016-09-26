#ifndef  HTMLIMAGEBROWSERCREATOR_H
#define  HTMLIMAGEBROWSERCREATOR_H

// options
#include "goptions.h"

// c++
#include <string>
#include <vector>
#include <fstream>
using namespace std;


class HtmlVariable {
	
public:
	HtmlVariable(string tit, string filename);
	
	string title;
	vector<string> present;
	
};

class Page {
	
public:
	Page(GOptions* gopt);
	
	// Page options
	static map<string, GOption> defineOptions();

private:

	vector<HtmlVariable> rows;
	vector<HtmlVariable> columns;
	vector<HtmlVariable> selections;

	// html
	ofstream hf;

	void writeTopHtml();
	void writeBottomHtml();

};

#endif
