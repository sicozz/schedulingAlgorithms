#include <iostream>
#include "scheduling.h"
#include "gantt.h"

using namespace std;

vector<process*> parser() {
    int nP;
    vector<process*> procVec;
    process* proc = new process;
    cin >> nP;
    while(nP--) {
        cin >> proc->pid >> proc->priority >> proc->arrivalT >> proc->burstT;
        proc->remainingT = proc->burstT;
        proc->responseT = -1;
        proc->completionT = -1;
        procVec.push_back(proc);
        proc = new process;
    }

    return procVec;
}

int main() {
    vector<process*> p = parser();
    vector<gantt*> ganttDiagram;
    scheduleFCFS s(p);
    s.executeNonPreemptive(&ganttDiagram, 0);
    printGannttDiagram(ganttDiagram);
    return 0;
}
