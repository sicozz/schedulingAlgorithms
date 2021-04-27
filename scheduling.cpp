#include <iostream>
#include "scheduling.h"

int turnAroundT(process p) {

    if (p.completionT == NOT_COMPLETED) {
        fprintf(stderr, "Error: Turn Around Time for non completed process");
        return NOT_COMPLETED;
    }
    return p.completionT - p.arrivalT;
}

int waitT(process p) {
    if (p.completionT == NOT_COMPLETED) {
        fprintf(stderr, "Error: Wait Time for non completed process");
        return NOT_COMPLETED;
    }
    return (p.responseT - p.burstT);
}

int responseT(process p) {

    if (p.responseT == NOT_COMPLETED) {
        fprintf(stderr, "Error: Response Time for non completed process");
        return NOT_COMPLETED;
    }

    return p.responseT;
}

int avgWaitT(std::vector<process*> procVec) {
    int total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++) total += waitT(*(*it));
    return (total/procVec.size());
}

int avgTurnAroundT(std::vector<process*> procVec) {
    int total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++) total += turnAroundT(*(*it));
    return (total/procVec.size());
}

int avgResponseT(std::vector<process*> procVec) {
    int total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++) total += responseT(*(*it));
    return (total/procVec.size());
}

bool priorityFCFS::operator() (process* a, process* b) {
    return (a->arrivalT > b->arrivalT);
}

bool prioritySJF::operator() (process* a, process* b) {
    return (a->remainingT > b->remainingT);
}

bool priorityP::operator() (process* a, process* b) {
    return (a->priority > b->priority);
}

template <class T>
int caller(schedule<T>* s) {

    std::vector<int> indices = s->arrival[s->elapsed];

    for (std::vector<int>::iterator it = indices.begin();
            it!=indices.end(); it++) {
        s->readyQueue.push((s->processes)[*it]);
    }
    return indices[indices.size()-1];
}

template <class T>
std::vector<gantt*> executeSchedule(bool isPreemptive, schedule<T> s) {
    std::vector<gantt*> ganttDiagram;
    process* aProcess = NULL;
    gantt* g = NULL;
    int start = 0;
    s.elapsed = 0;
    while(!s.readyQueue.empty() || (caller(&s)!=s.processes.size()-1)) {
        if (aProcess==NULL || (isPreemptive && aProcess!=s.readyQueue.top())) {
            if (aProcess!=NULL) {
                g = (gantt*) malloc(sizeof(gantt));
                g->i = start;
                g->f = s.elapsed;
                g->label = aProcess->pid;
            }
            aProcess = s.readyQueue.top();
        }
        if (aProcess!=NULL) {
            if (aProcess->responseT==NOT_COMPLETED)
                aProcess->responseT = s.elapsed;
            if (aProcess->remainingT) aProcess->remainingT--;
            else {
                aProcess->completionT = s.elapsed;
                s.readyQueue.pop();
                g = (gantt*) malloc(sizeof(gantt));
                g->i = start;
                g->f = s.elapsed;
                g->label = aProcess->pid;
                aProcess = NULL;
            }
        }
        ganttDiagram.push_back(g);
        s.elapsed++;
    }
    return ganttDiagram;
}
