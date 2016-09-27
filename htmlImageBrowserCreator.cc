// htmlImageBrowserCreator
#include "htmlImageBrowserCreator.h"
// c++
#include <fstream>

HtmlVariable::HtmlVariable(string tit, string filename)
{
	title = tit;
	ifstream in(filename.c_str());
	if(!in) {
		cerr << " !! Error: can't open input file " << filename << ". Check your spelling. Exiting. " << endl;
		exit(1);
	} else {
		cout << " > Scanning file " << filename << " for variable >" << tit << "<" << endl;
		while (!in.eof()) {
			string imageName;
			in >> imageName ;
			// checking if variable exist
			if(imageName.find(tit) != string::npos) {
				string thisFieldStart = imageName.substr(imageName.find(tit));
				// getting upper limit: underscore or .png
				string thisFieldEnd = "";
				if(thisFieldStart.find("_") != string::npos) {
					thisFieldEnd   = thisFieldStart.substr(0, thisFieldStart.find("_"));
				} else if(thisFieldStart.find(".png") != string::npos) {
					thisFieldEnd   = thisFieldStart.substr(0, thisFieldStart.find(".png"));
				}
				// getting lower limit: dash. This won't pass if thisFieldEnd is empty
				if(thisFieldEnd.find("-") != string::npos) {
					string newField = thisFieldEnd.substr(thisFieldEnd.find("-")+1);

					// inserting it only if it's not present already
					auto notPresent = find(present.begin(), present.end(), newField);
					if(notPresent == present.cend()) {
						present.push_back(newField);
						cout << " found " << tit << ": " << newField << endl;
					}
				}
			}
		}
		in.close();
	}
}

// these needs more documenting
map<string, GOption> Page::defineOptions()
{
	map<string, GOption> optionsMap;
	optionsMap["addRowTableVariable"]    = GOption("Row Variable Description",     "mauri",          "html", true);
	optionsMap["addColumnTableVariable"] = GOption("Column Variable Description",  "mauri",          "html", true);
	optionsMap["addSelectableVariable"]  = GOption("QuantityVariable Description", "mauri",          "html", true);
	optionsMap["f"]                      = GOption("Filename",                     "imageslist.txt");
	optionsMap["imagesSize"]             = GOption("(w, h) image size",            "1000 800");
	optionsMap["defaultPlot"]            = GOption("Default Plot at page loading", "default.png");
	optionsMap["pageTitle"]              = GOption("Page Title",                   "Mauri");
	optionsMap["tdGrouping"]             = GOption("Table scheme",                  0 );   // 0: rows and columns are independent. Any other number will print the value on td and group the tds
	return optionsMap;
}

Page::Page(GOptions* gopt)
{
	string filename = gopt->getString("f");

	for(auto &tv: gopt->getStrings("addRowTableVariable")) {
		rows.push_back(HtmlVariable(tv, filename));
	}
	for(auto &tv: gopt->getStrings("addColumnTableVariable")) {
		columns.push_back(HtmlVariable(tv, filename));
	}
	for(auto &tv: gopt->getStrings("addSelectableVariable")) {
		selections.push_back(HtmlVariable(tv, filename));
	}
	tdGrouping = gopt->getInt("tdGrouping");
	pageTitle = gopt->getString("pageTitle");

	hf.open("page.html");
	writeTopHtml();
	writeBody();
	writeBottomHtml();
	hf.close();

	cout << " tdGrouping set to " << tdGrouping << endl;

}

void Page::writeTopHtml()
{
	hf << "<html>" << endl;
	hf << "\t <head>" << endl;
	// css style
	hf << "\t\t <style type=\"text/css\">" << endl;
	hf << "\t\t\t #indextable { " << endl;
	hf << "\t\t\t\t font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif; " << endl;
	hf << "\t\t\t\t border-collapse: collapse; " << endl;
	hf << "\t\t\t } " << endl;
	hf << "\t\t\t #indextable td { " << endl;
	hf << "\t\t\t\t border: 1px solid #ddd; " << endl;
	hf << "\t\t\t\t padding: 8px; " << endl;
	hf << "\t\t\t } " << endl;
	hf << "\t\t\t #indextable tr:nth-child(even){background-color: #f2f2f2;} " << endl;
	hf << "\t\t\t #indextable tr:hover {background-color: #ddd;}" << endl;
	hf << "\t\t\t #indextable td:hover {background-color: #d55;}" << endl;

	hf << "\t\t </style>" << endl;
	// script
	hf << "\t\t <script>" << endl;
	
	// showPic function
	hf << "\t\t\t function popPic() {picDisplayed = window.open(document.DIST.src, document.DIST.src,";
	hf << "'statusbar=no, scrollbars=auto, status=no, resizable=no, width=1000, height=800');}" << endl;
	
	// selectable variables
	for(auto &sv: selections) {
		hf << "\t\t\t var " << sv.title << " = \"" << sv.present[0] << "\";" << endl;
	}
	
	// changeBin function
	hf << "\t\t\t function showPic(row, col)" << endl;
	hf << "\t\t\t {" << endl;
	hf << "\t\t\t    var dist = \"img/\"" << endl;
	hf << "\t\t\t    dist += \"" << rows[0].title << "-\"" << endl;
	hf << "\t\t\t    dist += row"  << endl;
	if(tdGrouping == 0) {
		hf << "\t\t\t    dist += \"_\"" << endl;
		hf << "\t\t\t    dist += \"" << columns[0].title << "-\"" << endl;
		hf << "\t\t\t    dist += col" << endl;
	}
	hf << "\t\t\t    dist += \".png\"" << endl;
	hf << "\t\t\t    document.DIST.src = dist" << endl;
	hf << "\t\t\t }" << endl;


	hf << "\t\t </script>" << endl;
	hf << "\t </head>" << endl;
}


void Page::writeBody()
{
	hf << "\t <body>" << endl;
	hf << "\t\t <center>" << endl;
	
	// title
	hf << "\t\t\t <h2> " << pageTitle << " </h2>" << endl;

	// table. Left: choices tables. Right: plot
	hf << "\t\t\t <table> " << endl;
	hf << "\t\t\t\t <tr> " << endl;

	// Left: choices tables
	hf << "\t\t\t\t\t <td> " << endl;
	hf << "\t\t\t\t\t\t <table id=\"indextable\" class=\"indextable\"> " << endl;

	// data rows
	if(tdGrouping == 0) {
		writeSingleTDs();
	} else if(tdGrouping > 0) {
		writeGroupedTDs();
	}
	
	hf << "\t\t\t\t\t\t </table> " << endl;
	hf << "\t\t\t\t\t </td> " << endl;



	// Right: plot
	hf << "\t\t\t\t\t <td> " << endl;
	hf << "\t\t\t\t\t\t <img name = \"DIST\" width=\"650px\" /> " << endl;
	hf << "\t\t\t\t\t </td> " << endl;

	
	
	
	hf << "\t\t\t\t </tr> " << endl;
	hf << "\t\t\t </table> " << endl;
	hf << "\t\t </center>" << endl;
	hf << "\t </body>" << endl;
}

void Page::writeBottomHtml()
{
	hf << "</html>" << endl;
}

void Page::writeGroupedTDs()
{
	// tds are grouped
	int gIndex = 0;

	for(auto &ri: rows[0].present) {


		if(gIndex == 0) hf << "\t\t\t\t\t\t\t <tr> " << endl;

		hf << "\t\t\t\t\t\t\t\t <td onclick=\"popPic()\" onmouseover=\"showPic('" << ri << "', 'na')\" >" << ri << "</td>" << endl;

		gIndex++;

		if(gIndex == tdGrouping) {
			hf << "\t\t\t\t\t\t\t </tr> " << endl;
			gIndex = 0;
		}

	}
}


void Page::writeSingleTDs()
{
	// first row: columns titles
	hf << "\t\t\t\t\t\t\t <tr> " << endl;
	hf << "\t\t\t\t\t\t\t\t <td> <sub> " << rows[0].title << "</sub> <sup> " << columns[0].title << "</sup> </td>" << endl;
	for(auto &ct : columns[0].present) {
		hf << "\t\t\t\t\t\t\t\t <td>" <<  ct << "</td>" << endl;
	}
	hf << "\t\t\t\t\t\t\t </tr> " << endl;

}














































