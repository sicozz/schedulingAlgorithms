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
void printGannttDiagram(vector<gantt*> ganttDiagram) {


    int size = ganttDiagram.size(), first = 1, startp, endp;

    cout << endl;
    cout << ANSI_COLOR_CYAN << "\t+-----------------------------------------+" << endl;
    cout << "\t|" << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_MAGENTA << "             GANTT  DIAGRAM              " << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_CYAN << "|" << endl;
    cout << "\t+-----------------------------------------+" << ANSI_COLOR_RESET << endl;

    cout << ANSI_COLOR_GREEN << setw( 12 + ( (size-1) * 10) ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET;

    for (auto it = ganttDiagram.begin(); it!=ganttDiagram.end(); it++)
        cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
        << left <<  ANSI_COLOR_BLUE << setw( 8 )
        << formatString((*it)->label, 6) << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_GREEN << "|" << setw( 12 + ( (size-1) * 10)  ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET << endl;

    for (auto it = ganttDiagram.begin(); it!=ganttDiagram.end(); it++) {
        if (first-- > 0)
            cout << left << ANSI_COLOR_YELLOW  << setw( 10 ) << (*it)->i;
        cout << setw( 10 ) << (*it)->f;
    }
    cout << ANSI_COLOR_RESET << endl << endl;
}
