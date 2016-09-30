#ifndef  HTMLIMAGEBROWSERCREATOR_H
#define  HTMLIMAGEBROWSERCREATOR_H

// we assume the first two variables are row and column
// respectively. The rest are the selectable variables


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

	int tdGrouping;
	string pageTitle;
	string defaultPlot;
	string imgDir;

	int xres, yres;

	// html
	ofstream hf;

	void writeTopHtml();
	void writeBottomHtml();
	void writeBody();

	void writeGroupedTDs();
	void writeSingleTDs();

	void writeSelectables();

};

#endif
