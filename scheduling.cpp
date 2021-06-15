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

int
gcd(int a,int b) {

    if (b == 0)
        return a;
    return gcd(b , a % b);
}

int
setLeastCommonMultiple(std::vector<int> periods) {

    /*
     * Tomado de : https://www.geeksforgeeks.org/lcm-of-given-array-elements/
     */
    int n, i, ans;

    ans = periods[0];
    for (i = 1; i < periods.size(); i++) {
        ans = ( (periods[i] * ans)) / gcd(periods[i], ans);
    }
    return ans;
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
scheduleRR::updateReadyQ(int low, int hi)
{
    std::vector<process*>::iterator it;
    for (it = processes.begin(); it != processes.end(); it++) {
        if (low <= (*it)->arrivalT && (*it)->arrivalT <= hi)
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
        updateReadyQ(elapsed, elapsed);
        if (aProc == NULL || aQuantum == 0) {
            if (aProc != NULL) {
                readyQ.push(aProc);
                if (aProc != readyQ.front() && cct != 0) {
                    updateReadyQ(elapsed + 1, elapsed + cct);
                    elapsed += cct;
                }
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
schedulePrioRR::updateReadyQ(int low, int hi)
{
    std::vector<process*>::iterator it;
    for (it = processes.begin(); it != processes.end(); it++) {
        if (low <= (*it)->arrivalT && (*it)->arrivalT <= hi)
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
        updateReadyQ(elapsed, elapsed);
        if (aProc == NULL || aQuantum == 0) {
            if (aProc != NULL) {
                readyQ.push(aProc);
                if (aProc != readyQ.top() && cct != 0) {
                    updateReadyQ(elapsed + 1, elapsed + cct);
                    elapsed += cct;
                }
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

/* Earliest Deadline First  */

scheduleEDF::scheduleEDF(std::vector<process*> processes) {
    this->processes = processes;
}


void
scheduleEDF::setIntervalos() {

    int i, k, period, limit, start, end;

    for (i = 0; i < processes.size(); i++) {
        period = processes[i]->period;
        limit = LCM / period;
        start = 0;
        end = period;
        for (k = 0; k < limit; k++) {
            interval* e;
            e = (interval*) malloc(sizeof(interval));
            e->start = start;
            e->end = end;
            e->count = 0;
            e->deadline = start + processes[i]->deadline;
            processes[i]->intervalos.push_back(e);
            start = end;
            end += period;
        }
    }
}

interval*
scheduleEDF::getInterval(process* t, int timeStart, int timeEnd) {

    for (int i = 0; i < t->intervalos.size(); i++) {
        interval* e = t->intervalos[i];
        if (timeStart >= e->start && timeEnd < e->end && e->count < t->capacity) {
            return e;
        }
    }

    return NULL;
}

process*
scheduleEDF::fetch(int timeStart, int timeEnd) {
    int i, k;

    if (processes.size() > 0) {
        process* process_min = processes[0];
        interval* interval_min = getInterval(process_min, timeStart, timeEnd);

        for (i = 1; i < processes.size(); i++) {
            process* current_process = processes[i];
            interval* current_interval = getInterval(current_process, timeStart, timeEnd);

            if (interval_min == NULL) {
                interval_min = current_interval;
                process_min = current_process;
            }
            else if (current_interval != NULL) {
                if (current_interval->deadline < interval_min->deadline) {
                    interval_min = current_interval;
                    process_min = current_process;
                }
            }

        }
        if (process_min != NULL && interval_min != NULL) {
            interval_min->count += 1;
            return process_min;
        }
    }

    return NULL;
}

void
scheduleEDF::executePreemptive(std::vector<gantt*>* g, int cct) {

    std::vector<int> periods;
    for (int i = 0; i < processes.size(); i++)
        periods.push_back(processes[i]->period);
    LCM = setLeastCommonMultiple(periods);
    setIntervalos();

    int timeStart = 0, timeEnd = 1;
    gantt* aGantt;

    while (timeStart < LCM) {

        string pid = "X";
        process* e = fetch(timeStart, timeEnd);
        if (e != NULL)
            pid = e->pid;

        aGantt = new gantt {pid, timeStart, timeEnd};
        g->push_back(aGantt);
        timeStart = timeEnd;
        timeEnd = timeEnd + 1;
    }
}

std::vector<process*>
scheduleEDF::getProcesses() {
    return processes;
}
/* Rate monotonic */

scheduleRM::scheduleRM(std::vector<process*> processes) {
    this->processes = processes;
}

void
scheduleRM::setIntervalos() {

    int i, k, period, limit, start, end;

    for (i = 0; i < processes.size(); i++) {
        period = processes[i]->period;
        limit = LCM / period;
        start = 0;
        end = period;
        for (k = 0; k < limit; k++) {
            interval* e;
            e = (interval*) malloc(sizeof(interval));
            e->start = start;
            e->end = end;
            e->count = 0;
            processes[i]->intervalos.push_back(e);
            start = end;
            end += period;
        }
    }
}

interval*
scheduleRM::getInterval(process* t, int timeStart, int timeEnd) {

    for (int i = 0; i < t->intervalos.size(); i++) {
        interval* e = t->intervalos[i];
        if (timeStart >= e->start && timeEnd < e->end && e->count < t->capacity) {
            e->count = e->count + 1;
            return e;
        }
    }

    return NULL;
}

process*
scheduleRM::fetch(int timeStart, int timeEnd) {
    int i, k;
    for (i = 0; i < processes.size(); i++) {
        process* p = processes[i];
        if (getInterval(p, timeStart, timeEnd) != NULL)
            return p;
    }
    return NULL;
}

void
scheduleRM::executePreemptive(std::vector<gantt*>* g, int cct) {

    std::vector<int> periods;
    for (int i = 0; i < processes.size(); i++)
        periods.push_back(processes[i]->period);

    LCM = setLeastCommonMultiple(periods);
    setIntervalos();
    sort(processes.begin(), processes.end(), scheduleRM::compare);

    int timeStart = 0, timeEnd = 1;
    gantt* aGantt;

    while (timeStart < LCM) {

        string pid = "X";
        process* e = fetch(timeStart, timeEnd);
        if (e != NULL)
            pid = e->pid;

        aGantt = new gantt {pid, timeStart, timeEnd};
         g->push_back(aGantt);
        timeStart = timeEnd;
        timeEnd = timeEnd + 1;
    }
}

bool
scheduleRM::compare(process* a, process* b) {
    return a->period < b->period;
}

std::vector<process*>
scheduleRM::getProcesses() {
    return processes;
}
