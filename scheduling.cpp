#include <iostream>
#include "scheduling.h"

using namespace std;

void printAverage(std::vector<process*> procVec) {

    float turnAroundT, waitT, responseT;
    turnAroundT = avgTurnAroundT(procVec);
    waitT = avgWaitT(procVec);
    responseT = avgResponseT(procVec);

    cout << ANSI_COLOR_CYAN << "\t\t+----------------------------------+" << endl;
    cout << "\t\t|" << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_MAGENTA << "             AVERAGE              " << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_CYAN << "|" << endl;
    cout << "\t\t+----------------------------------+" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_CYAN << setw( 62 ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << endl;

    cout << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_YELLOW << setw( 18 )
    << "Turnaround Time" << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_CYAN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_YELLOW << setw( 18 )
    << "Waiting Time" << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_CYAN << "| " << ANSI_COLOR_RESET
    << left << ANSI_COLOR_YELLOW << setw( 18 )
    << "Response Time" << ANSI_COLOR_RESET;


    cout << ANSI_COLOR_CYAN << "|"<< setw( 62 ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_CYAN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << turnAroundT << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_CYAN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << waitT << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_CYAN << "| " << ANSI_COLOR_RESET
    << left <<  ANSI_COLOR_BLUE << setw( 18 )
    << responseT << ANSI_COLOR_RESET;
    cout << ANSI_COLOR_CYAN<< "|"<< setw( 62 ) <<
    setfill( '-' ) << '\n' << setfill( ' ' ) << ANSI_COLOR_RESET << endl << endl;
}

bool processCompare::operator ()(const process* pa, const process* pb)
{
    return pa->priority > pb->priority;
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

float
avgWaitT(std::vector<process*> procVec) {
    float total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++)  total += float(waitT(*(*it)));
    return (total/float(procVec.size()));
}

float
avgTurnAroundT(std::vector<process*> procVec) {
    float total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++) total += float(turnAroundT(*(*it)));
    return (total/float(procVec.size()));
}

float
avgResponseT(std::vector<process*> procVec) {
    float total = 0;
    for (std::vector<process*>::iterator it = procVec.begin();
            it!=procVec.end(); it++) total += float(responseT(*(*it)));
    return (total/float(procVec.size()));
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
        if (topProc==NULL && (*it)->arrivalT<=elapsed && (*it)->completionT==-1)
            topProc = (*it);
        else {
            if ((*it)->arrivalT<=elapsed && (*it)->completionT==-1) {
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
                elapsed--;
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
            /*
            !!!! No entra al else porque cuando tiene 1, solo lo resta
                volviendolo 0, entonces cuando hace fetch ese ya no se toma en
                cuenta y nunca se le asigno completionT
                */
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
                elapsed--;
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
            it!=processes.end(); it++) {

        if (topProc==NULL && (*it)->arrivalT<=elapsed &&
                (*it)->completionT==-1)
            topProc = (*it);
        else {
            if ((*it)->arrivalT<=elapsed && (*it)->completionT==-1) {
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
        if (topProc==NULL && (*it)->arrivalT<=elapsed &&
                (*it)->completionT==-1)
            topProc = (*it);
        else {
            if ((*it)->arrivalT<=elapsed && (*it)->completionT==-1) {
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
                elapsed--;
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
                elapsed--;
            }
        }
        elapsed++;
    }
    aProc->completionT = elapsed;
    aGantt->f = elapsed;
    if (aGantt->i!=aGantt->f) g->push_back(aGantt);
}

/* Proyecto */


scheduleRR::scheduleRR(std::vector<process*> processes, int quantum)
{
    this->processes = processes;
    this->elapsed = 0;
    this->quantum = quantum;
    this->readyQ = *(new std::queue<process*>);
}

std::vector<process*>
scheduleRR::getProcesses()
{
    return processes;
}

bool
scheduleRR::finished()
{
    bool ans = true;
    std::vector<process*>::iterator it = processes.begin();
    while(it!=processes.end() && ans) {
        if ((*it)->remainingT!=0) ans = false;
        it++;
    }
    return ans;
}

void
scheduleRR::workOn(process* p)
{
    if (p->responseT == -1)
        p->responseT = elapsed;
    if (p->remainingT > 0)
        p->remainingT--;
    if (p->remainingT == 0)
        p->completionT = elapsed + 1;
}

void
scheduleRR::updateReadyQ()
{
    std::vector<process*>::iterator it;
    for (it = processes.begin(); it != processes.end(); it++) {
        if ((*it)->arrivalT == this->elapsed)
            this->readyQ.push(*it);
    }
}

void
scheduleRR::executePreemptive(std::vector<gantt*>* g, int cct)
{
    gantt* aGantt;
    process* aProc;
    int aQuantum;

    aGantt = new gantt {"X", 0, -1};
    aProc = NULL;
    aQuantum = quantum;

    while (!finished()) {
        updateReadyQ();
        if (aProc == NULL || aQuantum == 0) {
            if (aProc != NULL) {
                readyQ.push(aProc);
                elapsed += cct;
            }

            aProc = readyQ.front();
            readyQ.pop();

            if (aProc != NULL) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f)
                    g->push_back(aGantt);

                aGantt = new gantt {aProc->pid, elapsed, -1};
            }
            aQuantum = quantum;     // Se restablece el quantum
        }

        if (aProc != NULL) {
            workOn(aProc);
            if (aProc->completionT != -1) {
                aProc = NULL;
                aGantt->f = elapsed + 1;
                g->push_back(aGantt);
                aGantt = new gantt {"X", elapsed + 1, -1};
            }
        }
        elapsed++;
        aQuantum--;
    }
}

/* Priority Round Robin */

schedulePrioRR::schedulePrioRR(std::vector<process*> processes, int quantum)
{
    this->processes = processes;
    this->elapsed = 0;
    this->quantum = quantum;
    this->readyQ = *(new std::priority_queue<process*, std::vector<process*>,
            processCompare>);
}

std::vector<process*>
schedulePrioRR::getProcesses()
{
    return processes;
}

bool
schedulePrioRR::finished()
{
    bool ans = true;
    std::vector<process*>::iterator it = processes.begin();
    while(it!=processes.end() && ans) {
        if ((*it)->remainingT!=0) ans = false;
        it++;
    }
    return ans;
}

void
schedulePrioRR::workOn(process* p)
{
    if (p->responseT == -1)
        p->responseT = elapsed;
    if (p->remainingT > 0)
        p->remainingT--;
    if (p->remainingT == 0)
        p->completionT = elapsed + 1;
}

void
schedulePrioRR::updateReadyQ()
{
    std::vector<process*>::iterator it;
    for (it = processes.begin(); it != processes.end(); it++) {
        if ((*it)->arrivalT == this->elapsed)
            this->readyQ.push(*it);
    }
}

void
schedulePrioRR::executePreemptive(std::vector<gantt*>* g, int cct)
{
    gantt* aGantt;
    process* aProc;
    int aQuantum;

    aGantt = new gantt {"X", 0, -1};
    aProc = NULL;
    aQuantum = quantum;

    while (!finished()) {
        updateReadyQ();
        if (aProc == NULL || aQuantum == 0) {
            if (aProc != NULL) {
                if (aProc != readyQ.top()) {
                    elapsed += cct;
                }
                readyQ.push(aProc);
            }

            aProc = readyQ.top();
            readyQ.pop();

            if (aProc != NULL && aGantt->label != aProc->pid) {
                aGantt->f = elapsed;
                if (aGantt->i!=aGantt->f)
                    g->push_back(aGantt);

                aGantt = new gantt {aProc->pid, elapsed, -1};
            }
            aQuantum = quantum;     // Se restablece el quantum
        }

        if (aProc != NULL) {
            workOn(aProc);
            if (aProc->completionT != -1) {
                aProc = NULL;
                aGantt->f = elapsed + 1;
                g->push_back(aGantt);
                aGantt = new gantt {"X", elapsed + 1, -1};
            }
        }
        elapsed++;
        aQuantum--;
    }
}
