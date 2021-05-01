#include <iostream>
#include "scheduling.h"

using namespace std;

int main() {
    vector<process*> p;
    vector<gantt*> ganttDiagram;
    /*
    process p1 = {"P1", 0, 0, 5, 5, -1, -1};
    process p2 = {"P2", 0, 1, 4, 4, -1, -1};
    process p3 = {"P3", 0, 2, 3, 3, -1, -1};
    process p4 = {"P4", 0, 3, 2, 2, -1, -1};
    process p5 = {"P5", 0, 4, 4, 4, -1, -1};
    */
    process p1 = {"P1", 0, 0, 12, 12, -1, -1};
    process p2 = {"P2", 0, 2, 4, 4, -1, -1};
    process p3 = {"P3", 0, 3, 6, 6, -1, -1};
    process p4 = {"P4", 0, 8, 5, 5, -1, -1};
    p.push_back(&p1);p.push_back(&p2);p.push_back(&p3);p.push_back(&p4);
    scheduleSJF s(p);
    s.executePreemptive(&ganttDiagram, 0);
    for (auto it = ganttDiagram.begin(); it!=ganttDiagram.end(); it++)
        cout << (*it)->label << " " << (*it)->i << " " << (*it)->f << endl;
    return 0;
}
