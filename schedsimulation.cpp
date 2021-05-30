#include <iostream>
#include <thread>
#include "scheduling.h"
#include "gantt.h"
#include "stdio.h"

using namespace std;


void processSJF(scheduleSJF* s, vector<gantt*>* ganttDiagram, bool isPreemtive,
        int cct) {
    std::cout << "Launched by thread SJF" << std::endl;
    if (!isPreemtive)
        s->executeNonPreemptive(ganttDiagram, cct);
    else
        s->executePreemptive(ganttDiagram, cct);
}
void processFCFS(scheduleFCFS* s, vector<gantt*>* ganttDiagram, int cct) {
    std::cout << "Launched by thread FCFS" << std::endl;
    s->executeNonPreemptive(ganttDiagram, cct);

}
void processPRIO(schedulePrio* s, vector<gantt*>* ganttDiagram, bool
        isPreemtive, int cct) {
    std::cout << "Launched by thread PRIO"<< std::endl;
    if (!isPreemtive)
        s->executeNonPreemptive(ganttDiagram, cct);
    else
        s->executePreemptive(ganttDiagram, cct);
}

void processRR(scheduleRR* s, vector<gantt*>* ganttDiagram, int cct) {
    std::cout << "Launched by thread RR"<< std::endl;
    s->executePreemptive(ganttDiagram, cct);
}


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

void copyProccess(vector<process*>* dst, vector<process*> src) {

    for (std::vector<process*>::iterator it = src.begin();
            it!=src.end(); it++)  {
            process* proc = new process;
            proc->pid = (*it)->pid;
            proc->priority = (*it)->priority;
            proc->arrivalT = (*it)->arrivalT;
            proc->burstT = (*it)->burstT;
            proc->remainingT = (*it)->remainingT;
            proc->responseT = (*it)->responseT;
            proc->completionT = (*it)->completionT;
            dst->push_back(proc);
    }
}


int main(int argc, char const *argv[]) {

    // ./main isPreemtive cct
    if (argc < 3) {
        std::cout << "Error!" << '\n';
        exit(EXIT_FAILURE);
    }

    bool isPreemtive = atoi(argv[1]);
    int cct = atoi(argv[2]);
    int quantum;
    quantum = argc > 3 ? atoi(argv[3]) : 3;

    vector<process*> p1 = parser(), p2, p3, p4;
    vector<gantt*> ganttDiagramS, ganttDiagramF, ganttDiagramP, ganttDiagramRR;
    const int num_threads = 4;
    std::thread schedulings[num_threads];

    copyProccess(&p2, p1);
    copyProccess(&p3, p1);
    copyProccess(&p4, p1);

    scheduleSJF s(p1);
    scheduleFCFS f(p2);
    schedulePrio p(p3);
    scheduleRR rr(p4, quantum);

    schedulings[0] = std::thread(processSJF,  &s, &ganttDiagramS, isPreemtive, cct); // scheduleSJF
    schedulings[1] = std::thread(processFCFS, &f, &ganttDiagramF, cct); // scheduleFCFS
    schedulings[2] = std::thread(processPRIO, &p, &ganttDiagramP, isPreemtive, cct); // schedulePrio
    schedulings[3] = std::thread(processRR, &rr, &ganttDiagramRR, cct); // schedulePrio

    //Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i)
        schedulings[i].join();

    printTitle("scheduleSJF ");
    printGannttDiagram(ganttDiagramS);
    printAverage(s.getProcesses());

    printTitle("scheduleFCFS");
    printGannttDiagram(ganttDiagramF);
    printAverage(f.getProcesses());

    printTitle("schedulePrio");
    printGannttDiagram(ganttDiagramP);
    printAverage(p.getProcesses());

    printTitle("scheduleRR");
    printGannttDiagram(ganttDiagramRR);
    printAverage(rr.getProcesses());

    return 0;
}
