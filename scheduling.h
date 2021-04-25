#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <vector>
#include <map>
#include <queue>

struct process {
    std::string pid;
    int priority;
    int arrivalT;
    int burstT;
    int remainingT;
    int responseT;
    int completionT;
};

int turnAroundT(process p);
int waitT(process p);
int responseT(process p);
int avgTurnAroundT(std::vector<process*> s);
int avgResponseT(std::vector<process*> s);

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

template <class T>
struct schedule {
    std::vector<process*> processes;
    std::map<int, int> arrival;
    std::priority_queue<process*, std::vector<process*>, T> readyQueue;
    int elapsed;
};

#endif
