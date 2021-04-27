#include <iostream>
#include "scheduling.h"

using namespace std;

int main() {
    process p1 = {"P1", 0, 4, 5, -1, -1, -1};
    process p2 = {"P2", 0, 6, 4, -1, -1, -1};
    process p3 = {"P3", 0, 0, 3, -1, -1, -1};
    process p4 = {"P4", 0, 6, 2, -1, -1, -1};
    process p5 = {"P5", 0, 5, 4, -1, -1, -1};
    vector<process*> p;
    map<int, vector<int>> m;
    p.push_back(&p1);p.push_back(&p2);p.push_back(&p3);p.push_back(&p4);p.push_back(&p5);
    m[0] = vector<int> (1,2);
    m[1] = vector<int> (0);
    m[2] = vector<int> (0);
    m[3] = vector<int> (0);
    m[4] = vector<int> (1,0);
    m[5] = vector<int> (1,4);
    m[6] = vector<int> (2);m[6][0] = 1;m[6][1] = 3;
    priority_queue<process*, vector<process*>, priorityFCFS> rQ;
    schedule<priorityFCFS> sch = {p, m, rQ, 0};
    vector<gantt*> ganttDiagram;
    ganttDiagram = executeSchedule(false, sch);
    for (int i=0; i<ganttDiagram.size(); i++)
        cout << ganttDiagram[i]->label << " " << ganttDiagram[i]->i << " "  <<
            ganttDiagram[i]->f << endl;
    return 0;
}
