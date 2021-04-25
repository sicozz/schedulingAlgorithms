#include <iostream>
#include "scheduling.h"

int waitT(process p) {
    if (p.completionT==-1) {
        fprintf(stderr, "Error: Wait Time for non completed process");
        return -1;
    }
    return (p.completionT - p.arrivalT);
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
