#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <vector>
#include <map>
#include <queue>

#define NOT_COMPLETED -1

struct gantt {
    int i;
    int f;
    std::string label;
};

struct process {
    std::string pid;
    int priority;
    int arrivalT;
    int burstT;
    int remainingT;
    int responseT;
    int completionT;
};

template <class T>
struct schedule {
    public:
        std::vector<process*> processes;
        std::map<int, std::vector<int>> arrival;
        std::priority_queue<process*, std::vector<process*>, T> readyQueue;
        int elapsed;
        bool finished;
};

int turnAroundT(process p);
int waitT(process p);
int responseT(process p);
int avgWaitT(std::vector<process*> procVec);
int avgTurnAroundT(std::vector<process*> procVec);
int avgResponseT(std::vector<process*> procVec);

class priorityFCFS {
    public:
        bool operator() (process* a, process* b);
};
class prioritySJF {
    public:
        bool operator() (process* a, process* b);
};
class priorityP {
    public:
        bool operator() (process* a, process* b);
};

//template <class T>
void caller(schedule<priorityFCFS>* s);

//template <class T>
std::vector<gantt*> executeSchedule(bool isPreemptive, schedule<priorityFCFS> s);
#endif
