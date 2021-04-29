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

scheduleSJF::scheduleSJF(std::vector<process*> processes) {
    this->processes = processes;
    this->elapsed = 0;
}

bool scheduleSJF::finished() {
    bool ans = true;
    std::vector<process*>::iterator it = processes.begin();
    while(it!=processes.end() && ans) {
        if ((*it)->remainingT!=0) ans = false;
        it++;
    }
    return ans;
}

process* scheduleSJF::fetch() {
    process* topProc = NULL;
    for (std::vector<process*>::iterator it = processes.begin();
            it!=processes.end(); it++) {
        if (topProc==NULL && (*it)->arrivalT<=elapsed && (*it)->remainingT!=0) topProc = (*it);
        else {
            if ((*it)->arrivalT<=elapsed && (*it)->remainingT!=0) {
                if ((*it)->remainingT==topProc->remainingT) {
                    if ((*it)->arrivalT<topProc->arrivalT) topProc = (*it);
                }
                else {
                    if ((*it)->remainingT<topProc->remainingT) topProc = (*it);
                }
            }
        }
    }
    return topProc;
}
void scheduleSJF::execute(std::vector<gantt*>* g) {
    process* aProc = NULL;
    gantt* aGantt = new gantt {"X", 0, -1};
    aGantt->i = 0;
    while(!this->finished()){
        if (aProc==NULL) {
            aProc = this->fetch();
            if (aProc!=NULL) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                aGantt = new gantt {aProc->pid, elapsed, -1};
            }
        }
        if (aProc!=NULL) {
            if (aProc->responseT==-1) aProc->responseT=elapsed;
            if (aProc->remainingT!=0) aProc->remainingT--;
            else {
                aProc->completionT = elapsed;
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                aGantt = new gantt {"X", elapsed, -1};
                aProc = this->fetch();
                if (aProc!=NULL) aGantt->label = aProc->pid;
            }
        }
        elapsed++;
    }
    aProc->completionT = elapsed;
    aGantt->f = elapsed;
    if (aGantt->i!=aGantt->f) g->push_back(aGantt);
}

