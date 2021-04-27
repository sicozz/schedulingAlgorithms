#include <iostream>
#include "scheduling.h"

using namespace std;

int main() {
    vector<process*> p;
    map<int, vector<int>> m;
    priority_queue<process*, vector<process*>, priorityFCFS> rQ;
    vector<gantt*> ganttDiagram;
    process p1 = {"P1", 0, 0, 5, 5, -1, -1};
    process p2 = {"P2", 0, 1, 4, 4, -1, -1};
    process p3 = {"P3", 0, 2, 3, 3, -1, -1};
    process p4 = {"P4", 0, 3, 2, 2, -1, -1};
    process p5 = {"P5", 0, 4, 4, 4, -1, -1};
    m[0] = vector<int> (1,0);
    m[1] = vector<int> (1,1);
    m[2] = vector<int> (1,2);
    m[3] = vector<int> (1,3);
    m[4] = vector<int> (1,4);
    /*
    process p1 = {"P1", 0, 4, 5, 5, -1, -1};
    process p2 = {"P2", 0, 6, 4, 4, -1, -1};
    process p3 = {"P3", 0, 0, 3, 3, -1, -1};
    process p4 = {"P4", 0, 6, 2, 2, -1, -1};
    process p5 = {"P5", 0, 5, 4, 4, -1, -1};
    m[0] = vector<int> (1,2);
    m[1] = vector<int> (0);
    m[2] = vector<int> (0);
    m[3] = vector<int> (0);
    m[4] = vector<int> (1,0);
    m[5] = vector<int> (1,4);
    m[6] = vector<int> (2);m[6][0] = 3;m[6][1] = 1;
    */
    p.push_back(&p1);p.push_back(&p2);p.push_back(&p3);p.push_back(&p4);p.push_back(&p5);
    schedule<priorityFCFS> sch = {p, m, rQ, 0, false};
    ganttDiagram = executeSchedule(false, sch);
    for (int i=0; i<ganttDiagram.size(); i++)
        cout << ganttDiagram[i]->label << " " << ganttDiagram[i]->i << " "  <<
            ganttDiagram[i]->f << endl;

    /*
    process* aProcess;
    caller(&sch);
    while(!sch.readyQueue.empty()) {
        caller(&sch);
        aProcess = sch.readyQueue.top();
        cout << aProcess->pid << endl; 
        if (aProcess->remainingT) aProcess->remainingT--;
        else {
            aProcess = NULL;
            sch.readyQueue.pop();
        }
        sch.elapsed++;
    }
            */
    return 0;
}
