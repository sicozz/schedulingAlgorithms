#include "gantt.h"

using namespace std;

string formatString(string s, int max) {

    string output = "";
    for (int i = 0; i < max && i < s.size(); i++)
        output += s[i];

    if (s.size() > max)
        output += "..";

    return output;
}

void printTitle(string str) {

    cout << endl;
    cout << ANSI_COLOR_CYAN << "\t+-----------------------------------------+" << endl;
    cout << "\t|" << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_YELLOW  << "               "  <<  str << "              " << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_CYAN << "|" << endl;

}

void printGannttDiagram(vector<gantt*> ganttDiagram) {

    int size = ganttDiagram.size(), first = 1, startp, endp;

    cout << ANSI_COLOR_CYAN << "\t+-----------------------------------------+" << endl;
    cout << "\t|" << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_MAGENTA << "             GANTT  DIAGRAM              " << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_CYAN << "|" << endl;
    cout << "\t+-----------------------------------------+" << ANSI_COLOR_RESET << endl;

    cout << "\tLABEL\t\tSTART\t\tEND" << endl;
    for (auto it = ganttDiagram.begin(); it!=ganttDiagram.end(); it++)
        cout << "\t" <<  ANSI_COLOR_GREEN  << formatString((*it)->label, 6) <<  "\t\t" <<  ANSI_COLOR_YELLOW
         << formatString( to_string((*it)->i), 5) << ANSI_COLOR_RED
         << "\t\t" << formatString( to_string((*it)->f), 5) <<  ANSI_COLOR_RESET << endl;
    cout << endl;
}
