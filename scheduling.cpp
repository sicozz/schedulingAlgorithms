#include <iostream>
#include "scheduling.h"

using namespace std;

void printAverage(std::vector<process*> procVec) {

    int turnAroundT, waitT, responseT;
    turnAroundT = avgTurnAroundT(procVec);
    waitT = avgWaitT(procVec);
    responseT = avgResponseT(procVec);

    cout << ANSI_COLOR_CYAN << "\t\t+----------------------------------+" << endl;
    cout << "\t\t|" << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_MAGENTA << "             AVERAGE              " << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_CYAN << "|" << endl;
    cout << "\t\t+----------------------------------+" << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_GREEN << setw( 62 ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET << endl;

    cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << "Turnaround Time" << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << "Waiting Time" << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << "Response Time" << ANSI_COLOR_RESET;


    cout << ANSI_COLOR_GREEN << "|"<< setw( 62 ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << turnAroundT << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << waitT << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_GREEN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << responseT << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_GREEN << "|"<< setw( 62 ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET << endl << endl;
}


int turnAroundT(process p) {

    if (p.completionT == -1) {
        fprintf(stderr, "Error: Turn Around Time for non completed process");
        return -1;
    }
    return p.completionT - p.arrivalT;
}

int waitT(process p) {
    if (p.completionT == -1) {
        fprintf(stderr, "Error: Wait Time for non completed process");
        return -1;
    }
    return (turnAroundT(p) - p.burstT);
}

int responseT(process p) {

    if (p.responseT == -1) {
        fprintf(stderr, "Error: Response Time for non completed process");
        return -1;
    }

    return p.responseT;
}

int avgWaitT(std::vector<process*> procVec) {
    int total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++)  total += waitT(*(*it));
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

std::vector<process*> scheduleSJF::getProcesses() {
    return processes;
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
        if (topProc==NULL && (*it)->arrivalT<=elapsed && (*it)->remainingT!=0)
            topProc = (*it);
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

void scheduleSJF::executeNonPreemptive(std::vector<gantt*>* g, int cct) {
    process* aProc = NULL;
    gantt* aGantt = new gantt {"X", 0, -1};
    while(!this->finished()){
        if (aProc==NULL) {
            aProc = this->fetch();
            if (aProc!=NULL) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                elapsed += cct;
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
                elapsed += cct;
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

void scheduleSJF::executePreemptive(std::vector<gantt*>* g, int cct) {
    process* aProc = NULL;
    gantt* aGantt = new gantt {"X", 0, -1};
    aGantt->i = 0;
    while(!this->finished()){
        aProc = this->fetch();
        if (aProc!=NULL) {
            if (aGantt->label!=aProc->pid) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                elapsed += cct;
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
                elapsed += cct;
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

scheduleFCFS::scheduleFCFS(std::vector<process*> processes) {
    this->processes = processes;
    this->elapsed = 0;
}

std::vector<process*> scheduleFCFS::getProcesses() {
    return processes;
}

bool scheduleFCFS::finished() {
    bool ans = true;
    std::vector<process*>::iterator it = processes.begin();
    while(it!=processes.end() && ans) {
        if ((*it)->remainingT!=0) ans = false;
        it++;
    }
    return ans;
}

process* scheduleFCFS::fetch() {
    process* topProc = NULL;
    for (std::vector<process*>::iterator it = processes.begin();
            it!=processes.end() && topProc == NULL; it++) {

        if (topProc==NULL && (*it)->arrivalT<=elapsed && (*it)->remainingT!=0)
            topProc = (*it);
        else {
            if ((*it)->arrivalT<=elapsed && (*it)->remainingT!=0) {
                if ((*it)->arrivalT<topProc->arrivalT) topProc = (*it);
            }
        }

    }
    return topProc;
}

void scheduleFCFS::executeNonPreemptive(std::vector<gantt*>* g, int cct) {
    process* aProc = NULL;
    gantt* aGantt = new gantt {"X", 0, -1};
    while(!this->finished()){
        if (aProc==NULL) {
            aProc = this->fetch();
            if (aProc!=NULL) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                elapsed += cct;
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
                elapsed += cct;
                aGantt = new gantt {"X", elapsed, -1};
                aProc = this->fetch();
                if (aProc!=NULL) {
                    aGantt->label = aProc->pid;
                    elapsed = elapsed - 1;
                }
            }
        }
        elapsed++;
    }
    aProc->completionT = elapsed;
    aGantt->f = elapsed;
    if (aGantt->i!=aGantt->f) g->push_back(aGantt);
}

schedulePrio::schedulePrio(std::vector<process*> processes) {
    this->processes = processes;
    this->elapsed = 0;
}

std::vector<process*> schedulePrio::getProcesses() {
    return processes;
}
bool schedulePrio::finished() {
    bool ans = true;
    std::vector<process*>::iterator it = processes.begin();
    while(it!=processes.end() && ans) {
        if ((*it)->remainingT!=0) ans = false;
        it++;
    }
    return ans;
}

process* schedulePrio::fetch() {
    process* topProc = NULL;
    for (std::vector<process*>::iterator it = processes.begin();
            it!=processes.end(); it++) {
        if (topProc==NULL && (*it)->arrivalT<=elapsed && (*it)->remainingT!=0)
            topProc = (*it);
        else {
            if ((*it)->arrivalT<=elapsed && (*it)->remainingT!=0) {
                if ((*it)->priority==topProc->priority) {
                    if ((*it)->arrivalT<topProc->arrivalT) topProc = (*it);
                }
                else {
                    if ((*it)->priority<topProc->priority) topProc = (*it);
                }
            }
        }
    }
    return topProc;
}

void schedulePrio::executeNonPreemptive(std::vector<gantt*>* g, int cct) {
    process* aProc = NULL;
    gantt* aGantt = new gantt {"X", 0, -1};
    while(!this->finished()){
        if (aProc==NULL) {
            aProc = this->fetch();
            if (aProc!=NULL) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                elapsed += cct;
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
                elapsed += cct;
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

void schedulePrio::executePreemptive(std::vector<gantt*>* g, int cct) {
    process* aProc = NULL;
    gantt* aGantt = new gantt {"X", 0, -1};
    aGantt->i = 0;
    while(!this->finished()){
        aProc = this->fetch();
        if (aProc!=NULL) {
            if (aGantt->label!=aProc->pid) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f) g->push_back(aGantt);
                elapsed += cct;
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
                elapsed += cct;
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
