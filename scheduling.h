#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <vector>
#include <map>
#include <queue>

#define NOT_COMPLETED -1

struct gantt {
    std::string label;
    int i;
    int f;
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
    public:
        bool finished();
        scheduleSJF(std::vector<process*> processes);
        void executeNonPreemptive(std::vector<gantt*>* g);
        void executePreemptive(std::vector<gantt*>* g);
};
#endif
