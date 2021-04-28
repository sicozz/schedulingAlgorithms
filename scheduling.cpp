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

void caller(schedule<priorityFCFS>* s) {
    std::vector<int> indices = s->arrival[s->elapsed];

    for (std::vector<int>::iterator it = indices.begin();
            it!=indices.end(); it++) {
        s->readyQueue.push((s->processes)[*it]);
    }
    if (indices.size()) {
        if (indices[indices.size()-1]==s->processes.size()-1) s->finished = true;
    }
}

void logger(schedule<priorityFCFS> s, process* p) {
    std::cout << "---------------" << std::endl; 
    std::cout << "Top" << std::endl; 
    if (s.readyQueue.top()!=NULL) std::cout << s.readyQueue.top()->pid << std::endl; 
    else std::cout << "NULL top" << std::endl; 
    std::cout << "aProcess" << std::endl; 
    if (p!=NULL) std::cout << p->pid << std::endl; 
    else std::cout << "NULL aProcess" << std::endl; 
    std::cout << "---------------" << std::endl; 
}
std::vector<gantt*> executeSchedule(bool isPreemptive, schedule<priorityFCFS> s) {
    std::vector<gantt*> ganttDiagram;
    process* aProcess = NULL;
    bool first = true;
    bool dead = false;
    gantt* g = NULL;
    int start = 0;
    s.elapsed = 0;
    while(!s.readyQueue.empty() || !s.finished) {
        caller(&s);
        if (aProcess==NULL || (isPreemptive && aProcess!=s.readyQueue.top())) {
            if (aProcess!=NULL) {
                g = new gantt;
                g->i = start;
                g->f = s.elapsed;
                g->label = aProcess->pid;
                ganttDiagram.push_back(g);
            }
            aProcess = s.readyQueue.top();
            if (aProcess!=NULL) {
                if (aProcess->pid=="P1") std::cout << "Start " << start << " Elapsed " << s.elapsed << std::endl;
                if (!first && !dead) aProcess->remainingT--;
            }
            if (dead) {
               start = s.elapsed;
               dead = false;
            }
        }
        if (aProcess!=NULL) {
            if (aProcess->responseT==NOT_COMPLETED)
                aProcess->responseT = s.elapsed;
            if (aProcess->remainingT) aProcess->remainingT--;
            else {
                aProcess->completionT = s.elapsed;
                g = new gantt;
                g->i = start;
                g->f = s.elapsed;
                g->label = aProcess->pid;
                ganttDiagram.push_back(g);
                aProcess = NULL;
                s.readyQueue.pop();
                start = s.elapsed;
                first = false;
                if (s.readyQueue.empty()) dead = true;
            }
        }
        s.elapsed++;
    }
    return ganttDiagram;
}
