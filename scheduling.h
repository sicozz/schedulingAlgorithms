#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
#include "gantt.h"
#include <stdio.h>
struct process {
    std::string pid;
    int priority;
    int arrivalT;
    int burstT;
    int remainingT;
    int responseT;
    int completionT;
};

class processCompare {
    public:
        bool operator ()(const process* pa, const process* pb);
};

void printTitle(std::string str);
void printAverage(std::vector<process*> procVec);
int turnAroundT(process p);
int waitT(process p);
int responseT(process p);
int avgWaitT(std::vector<process*> procVec);
int avgTurnAroundT(std::vector<process*> procVec);
int avgResponseT(std::vector<process*> procVec);

class scheduleSJF {
    private:
        std::vector<process*> processes;
        int elapsed;
        process* fetch();
        bool finished();
    public:
        scheduleSJF(std::vector<process*> processes);
        std::vector<process*> getProcesses();
        void executeNonPreemptive(std::vector<gantt*>* g, int cct);
        void executePreemptive(std::vector<gantt*>* g, int cct);
};


class scheduleFCFS {
    private:
        std::vector<process*> processes;
        int elapsed;
        process* fetch();
        bool finished();
    public:
        scheduleFCFS(std::vector<process*> processes);
        std::vector<process*> getProcesses();
        void executeNonPreemptive(std::vector<gantt*>* g, int cct);
};

class schedulePrio {
    private:
        std::vector<process*> processes;
        int elapsed;
        process* fetch();
        bool finished();
    public:
        schedulePrio(std::vector<process*> processes);
        std::vector<process*> getProcesses();
        void executeNonPreemptive(std::vector<gantt*>* g, int cct);
        void executePreemptive(std::vector<gantt*>* g, int cct);
};

class scheduleRR {
    private:
        std::vector<process*> processes;
        int elapsed;
        int quantum;
        std::queue<process*> readyQ;
        bool finished();
        void workOn(process*);
        void updateReadyQ();
    public:
        scheduleRR(std::vector<process*> processes, int quantum);
        std::vector<process*> getProcesses();
        void executePreemptive(std::vector<gantt*>* g, int cct);
};

class schedulePrioRR {
    private:
        std::vector<process*> processes;
        int elapsed;
        int quantum;
        std::priority_queue<process*, std::vector<process*>, processCompare>
            readyQ;
        bool finished();
        void workOn(process*);
        void updateReadyQ();
    public:
        schedulePrioRR(std::vector<process*> processes, int quantum);
        std::vector<process*> getProcesses();
        void executePreemptive(std::vector<gantt*>* g, int cct);
};
#endif
