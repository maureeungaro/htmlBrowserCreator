/// \file example.cc

// example on how to use the text progress bar library

// text progress bar
#include "htmlImageBrowserCreator.h"

// c++
#include <iostream>
using namespace std;



// load the Page option
map<string, GOption> defineOptions()
{
	map<string, GOption> optionsMap;
	optionsMap += Page::defineOptions();
	return optionsMap;
}


//! example of main declaring GOptions
int main(int argc, char* argv[])
{
	GOptions *gopts = new GOptions(argc, argv, defineOptions(), 1);

	Page page(gopts);
}

