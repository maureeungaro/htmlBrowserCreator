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
	} else if(tit != "mauri") {
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
	optionsMap["addRowTableVariable"]    = GOption("Row Variable Description",     "mauri");
	optionsMap["addColumnTableVariable"] = GOption("Column Variable Description",  "mauri");
	optionsMap["addSelectableVariable"]  = GOption("QuantityVariable Description", "mauri",  "general", true);  // can be repeated
	optionsMap["f"]                      = GOption("Filename",                     "imageslist.txt");
	optionsMap["d"]                      = GOption("Image Directory",              "img");
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
		if(tv != "mauri")
			rows.push_back(HtmlVariable(tv, filename));
	}
	for(auto &tv: gopt->getStrings("addColumnTableVariable")) {
		if(tv != "mauri")
			columns.push_back(HtmlVariable(tv, filename));
	}
	for(auto &tv: gopt->getStrings("addSelectableVariable")) {
		if(tv != "mauri")
			selections.push_back(HtmlVariable(tv, filename));
	}
	tdGrouping  = gopt->getInt("tdGrouping");
	pageTitle   = gopt->getString("pageTitle");
	defaultPlot = gopt->getString("defaultPlot");
	imgDir      = gopt->getString("d");

	vector<string> ress = gopt->getStringVectorValue("imagesSize");
	if(ress.size() == 2) {
		// not sure why I have to add 50. 
		xres = stod(ress[0]) + 50;
		yres = stod(ress[1]) + 50;
	}

	hf.open("page.html");
	writeTopHtml();
	writeBody();
	writeBottomHtml();
	hf.close();

	cout << " tdGrouping set to " << tdGrouping << endl;

}

void Page::writeTopHtml()
{
	hf << "<html lang=\"en\">" << endl;
	hf << "\t <head>" << endl;
	// css style: table
	hf << "\t\t <style>" << endl;
	hf << "\t\t\t #indextable { " << endl;
	hf << "\t\t\t\t font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif; " << endl;
	hf << "\t\t\t\t border-collapse: collapse; " << endl;
	hf << "\t\t\t } " << endl;
	hf << "\t\t\t #indextable td { " << endl;
	hf << "\t\t\t\t border: 1px solid #ddd; " << endl;
	hf << "\t\t\t\t padding: 6px; " << endl;
	hf << "\t\t\t } " << endl;
	hf << "\t\t\t #indextable tr:nth-child(even){background-color: #f2f2f2;} " << endl;
	hf << "\t\t\t #indextable tr:hover {background-color: #ddd;}" << endl;
	hf << "\t\t\t #indextable td:hover {background-color: #119; color: white;}" << endl;
	hf << "\t\t\t .button { " << endl;
    hf << "\t\t\t\t background-color: white;" << endl;
    hf << "\t\t\t\t     color: black;" << endl;
    hf << "\t\t\t\t     border: 2px solid #555555;" << endl;
    hf << "\t\t\t\t     border-radius: 10%;" << endl;
    hf << "\t\t\t\t     padding: 15px 32px;         /* Button size */" << endl;
    hf << "\t\t\t\t     text-align: center;         /* Center text */" << endl;
    hf << "\t\t\t\t     text-decoration: none;      /* No underline */" << endl;
    hf << "\t\t\t\t     display: inline-block;      /* Inline block */" << endl;
    hf << "\t\t\t\t     font-size: 16px;            /* Text size */" << endl;
    hf << "\t\t\t\t     margin: 4px 2px;            /* Small margins */" << endl;
    hf << "\t\t\t\t     cursor: pointer;            /* Pointer cursor */" << endl;
    hf << "\t\t\t\t     transition: background-color 0.3s ease; /* Smooth background transition */" << endl;
    hf << "\t\t\t}" << endl;
    hf << "\t\t\t " << endl;

    hf << "\t\t\t/* Hover effect */" << endl;
    hf << "\t\t\tbutton:hover {" << endl;
    hf << "\t\t\t\t background-color: #45a049; /* Darker green on hover */" << endl;
    hf << "\t\t\t}" << endl;

    hf << "\t\t\t/* Active state (when clicked) */" << endl;
    hf << "\t\t\tbutton:active {" << endl;
    hf << "\t\t\t\t transform: scale(0.98);    /* Slight shrink effect on click */" << endl;
    hf << "\t\t\t}" << endl;

    hf << "\t\t\t/* Selected (stay highlighted) */" << endl;
    hf << "\t\t\tbutton.selected {" << endl;
    hf << "\t\t\t\t background-color: #2e7031; /* Green when selected */" << endl;
    hf << "\t\t\t\t color: white;               /* White text when selected */" << endl;
    hf << "\t\t\t\t box-shadow: 0 5px #666;    /* Shadow for depth */" << endl;
    hf << "\t\t\t}" << endl;

    hf << "\t\t\t/* Optional: Focus state (when keyboard focused) */" << endl;
    hf << "\t\t\tbutton:focus {" << endl;
    hf << "\t\t\t\t outline: none;             /* Remove default focus outline */" << endl;
    hf << "\t\t\t\t box-shadow: 0 0 10px #719ECE; /* Custom blue glow */" << endl;
    hf << "\t\t\t}" << endl;

	hf << "\t\t </style>" << endl;
    hf << "\t\t <title>Mauri's plots</title>" << endl;
    // script
	hf << "\t\t <script>" << endl;
	
	// showPic function
	hf << "\t\t\t function popPic() {picDisplayed = window.open(document.DIST.src, document.DIST.src,";
	hf << "'statusbar=no, scrollbars=auto, status=no, resizable=no, width=" << xres << ", height=" << yres << "');}" << endl;
	
	// selectable variables
	for(auto &sv: selections) {
		hf << "\t\t\t var " << sv.title << " = \"" << sv.present[0] << "\";" << endl;
	}
	
	// changeBin function
	hf << "\t\t\t function showPic(row, col)" << endl;
	hf << "\t\t\t {" << endl;

	// row
	hf << "\t\t\t    var dist = \"" << imgDir << "/\"" << endl;
	hf << "\t\t\t    dist += \"" << rows[0].title << "-\"" << endl;
	hf << "\t\t\t    dist += row;"  << endl;

	// column if necessary
	if(tdGrouping == 0) {
		hf << "\t\t\t    dist += \"_\"" << endl;
		hf << "\t\t\t    dist += \"" << columns[0].title << "-\"" << endl;
		hf << "\t\t\t    dist += col;" << endl;
	}

	// selecteables
	for(auto &sv: selections) {
		hf << "\t\t\t    dist += \"_" << sv.title << "-\";" << endl;
		hf << "\t\t\t    dist += "    << sv.title << ";" << endl;
	}

	// format
	hf << "\t\t\t    dist += \".png\"" << endl;
	hf << "\t\t\t    document.DIST.src = dist" << endl;
	hf << "\t\t\t }" << endl;

	// for each selectables create the function that change the title
	for(auto &sv: selections) {
		string functionTitle = "change" + sv.title;
		string btitle        = sv.title + "Buttons";
		string ctitle        = sv.title + "Title";
		hf << "\t\t\t function " << functionTitle << "(selectedButton)" << endl;
		hf << "\t\t\t {" << endl;
		hf << "\t\t\t\t var " << ctitle << " = \"" << sv.title << " \"  + " << sv.title << ";" << endl;
		hf << "\t\t\t\t document.getElementById(\"" << btitle << "\").innerHTML = " << ctitle << ";" << endl;

        // Remove 'selected' class from all buttons in the weight row
        hf << "\t\t\t\t var buttons = selectedButton.closest('tr').querySelectorAll('button');" << endl;
        hf << "\t\t\t\t buttons.forEach(button => button.classList.remove(\"selected\"));" << endl;

        // Add 'selected' class to the clicked button
        hf << "\t\t\t\t selectedButton.classList.add(\"selected\");" << endl;
        hf << "\t\t\t }" << endl;

	}

	hf << "\t\t </script>" << endl;
	hf << "\t </head>" << endl;
}


void Page::writeBody()
{
	hf << "\t <body>" << endl;
	hf << "\t\t <center>" << endl;
	
	// title
	if(pageTitle != "Mauri") hf << "\t\t\t <h2> " << pageTitle << " </h2>" << endl;


	// table. Left: data tables. Right: plot
	hf << "\t\t\t <table> " << endl;

	// selectables quantities
	writeSelectables();

	hf << "\t\t\t\t <tr> " << endl;
	// Left: data tables
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
	hf << "\t\t\t\t\t\t <img name = \"DIST\" width=\"650px\" src=\"" << defaultPlot << "\"/> " << endl;
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

	// all remaining rows: row content
	for(auto &ri: rows[0].present) {
		hf << "\t\t\t\t\t\t\t <tr> " << endl;

		hf << "\t\t\t\t\t\t\t\t <td>" << ri << "</td>" << endl;

		for(auto &ct : columns[0].present) {
			hf << "\t\t\t\t\t\t\t\t <td onclick=\"popPic()\" onmouseover=\"showPic('" << ri << "', '" << ct << "')\" />"  << endl;
		}



		hf << "\t\t\t\t\t\t\t </tr> " << endl;
	}

}


void Page::writeSelectables()
{
	for(auto &sv: selections) {
		// selectable quantities - onlt left TD
		hf << "\t\t\t\t <tr> " << endl;

		// colspan 2 so buttons include the plot space too
		hf << "\t\t\t\t\t <td colspan=2> " << endl;

		hf << "\t\t\t\t\t\t <table> <tr>" << endl;
		string functionTitle = "change" + sv.title;
		string btitle        = sv.title + "Buttons";

		hf << "\t\t\t\t\t\t\t <td id=\"" << btitle << "\" align=center colspan=" << sv.present.size() << ">" << sv.title << " " << sv.present[0] << "</td></tr><tr>" << endl;

		for(auto &ps: sv.present) {
			hf << "\t\t\t\t\t\t\t <td><button class=\"button\" onclick=\"javascript:" ;
			hf << sv.title << "=\'" << ps << "\' ; " << functionTitle << "(this); \">" << ps <<"</button></td>" << endl;
		}
		hf << "\t\t\t\t\t\t </tr></table>" << endl;
		hf << "\t\t\t\t\t </td> " << endl;

		hf << "\t\t\t\t </tr> " << endl;
	}
}
