#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
#include "gantt.h"
#include <stdio.h>
#include <algorithm>

struct process {
    std::string pid;
    int priority;
    int arrivalT;
    int burstT;
    int remainingT;
    int responseT;
    int completionT;
};

struct interval {
    int start;
    int end;
    int count;
    int deadline;
};


struct procces_rt {
    std::string pid;
    int capacity;
    int period;
    int deadline;
    std::vector<interval*> intervalos;
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
float avgWaitT(std::vector<process*> procVec);
float avgTurnAroundT(std::vector<process*> procVec);
float avgResponseT(std::vector<process*> procVec);
int setLeastCommonMultiple(std::vector<int> v);
int gcd(int a, int b);

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
        void updateReadyQ(int low, int hi);
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
        void updateReadyQ(int low, int hi);
    public:
        schedulePrioRR(std::vector<process*> processes, int quantum);
        std::vector<process*> getProcesses();
        void executePreemptive(std::vector<gantt*>* g, int cct);
};

class scheduleRM {
    private:
        int LCM;
        std::vector<procces_rt*> processes;
        void setIntervalos();
        static bool compare(procces_rt* a, procces_rt* b);
        procces_rt* fetch(int timeStart, int timeEnd);
        interval* getInterval(procces_rt* t, int timeStart, int timeEnd);
    public:
        void executePreemptive(std::vector<gantt*>* g, int cct);
        scheduleRM (std::vector<procces_rt*> processes);
};

class scheduleEDF {
    private:
        int LCM;
        std::vector<procces_rt*> processes;
        void setIntervalos();
        procces_rt* fetch(int timeStart, int timeEnd);
        interval* getInterval(procces_rt* t, int timeStart, int timeEnd);
    public:
        void executePreemptive(std::vector<gantt*>* g, int cct);
        scheduleEDF (std::vector<procces_rt*> processes);
};

#endif
