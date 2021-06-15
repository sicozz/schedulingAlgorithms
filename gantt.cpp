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
    cout << _CYAN << "\t+-----------------------------------------+" << endl;
    cout << "\t|" << _RESET;
    cout << _YELLOW  << "               "  <<  str << "              " << _RESET;
    cout << _CYAN << "|" << endl;

}

void printGannttDiagram(vector<gantt*> ganttDiagram) {

    int size = ganttDiagram.size(), first = 1, startp, endp;

    cout << _CYAN << "\t+-----------------------------------------+" << endl;
    cout << "\t|" << _RESET;
    cout << _MAGENTA << "             GANTT  DIAGRAM              " << _RESET;
    cout << _CYAN << "|" << endl;
    cout << "\t+-----------------------------------------+" << _RESET << endl;

    cout << "\tLABEL\t\tSTART\t\tEND" << endl;
    for (auto it = ganttDiagram.begin(); it!=ganttDiagram.end(); it++)
        cout << "\t" <<  _GREEN  << formatString((*it)->label, 6) <<  "\t\t" <<  _YELLOW
         << formatString( to_string((*it)->i), 5) << _RED
         << "\t\t" << formatString( to_string((*it)->f), 5) <<  _RESET << endl;
    cout << endl;
}
