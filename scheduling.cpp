#include <iostream>
#include "scheduling.h"

int turnAroundT(process p) {

    if (p.completionT == NOT_COMPLETED) {
        fprintf(stderr, "Error: Turn Around Time for non completed process");
        return NOT_COMPLETED;
    }
    return p.completionT - p.arrivalT;
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

int waitT(process p) {
    if (p.completionT == NOT_COMPLETED) {
        fprintf(stderr, "Error: Wait Time for non completed process");
        return NOT_COMPLETED;
    }
    return (p.responseT - p.burstT);
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
void caller(schedule<T>* s) {

    std::vector<int> indices = s->arrival[s->elapsed];
    int i, index;

    for (i = 0; i < indices.size(); i++) {
        index = indices[i];
        if (index >= 0 && index < s->processes.size())
            s->readyQueue.push( (s->processes)[index] );
    }
}
