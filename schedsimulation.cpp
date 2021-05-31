#include <ctype.h>
#include <iostream>
#include "gantt.h"
#include "scheduling.h"
#include <string.h>
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <thread>

#define FCFS    0
#define SJF     1
#define PRIO    2
#define RR      3
#define PRIO_RR 4

using namespace std;

void processSJF(scheduleSJF* s, vector<gantt*>* ganttDiagram, bool isPreemtive,
        int cct) {
    std::cout << "Launched by thread SJF" << std::endl;
    if (!isPreemtive)
        s->executeNonPreemptive(ganttDiagram, cct);
    else
        s->executePreemptive(ganttDiagram, cct);
}
void processFCFS(scheduleFCFS* s, vector<gantt*>* ganttDiagram, int cct) {
    std::cout << "Launched by thread FCFS" << std::endl;
    s->executeNonPreemptive(ganttDiagram, cct);

}
void processPRIO(schedulePrio* s, vector<gantt*>* ganttDiagram, bool
        isPreemtive, int cct) {
    std::cout << "Launched by thread PRIO"<< std::endl;
    if (!isPreemtive)
        s->executeNonPreemptive(ganttDiagram, cct);
    else
        s->executePreemptive(ganttDiagram, cct);
}

void processRR(scheduleRR* s, vector<gantt*>* ganttDiagram, int cct) {
    std::cout << "Launched by thread RR"<< std::endl;
    s->executePreemptive(ganttDiagram, cct);
}

void processPrioRR(schedulePrioRR* s, vector<gantt*>* ganttDiagram, int cct) {
    std::cout << "Launched by thread PrioRR"<< std::endl;
    s->executePreemptive(ganttDiagram, cct);
}


vector<process*> parser() {
    int nP;
    vector<process*> procVec;
    process* proc = new process;
    cin >> nP;
    while(nP--) {
        cin >> proc->pid >> proc->priority >> proc->arrivalT >> proc->burstT;
        proc->remainingT = proc->burstT;
        proc->responseT = -1;
        proc->completionT = -1;
        procVec.push_back(proc);
        proc = new process;
    }
    return procVec;
}

void copyProccess(vector<process*>* dst, vector<process*> src) {

    for (std::vector<process*>::iterator it = src.begin();
            it!=src.end(); it++)  {
            process* proc = new process;
            proc->pid = (*it)->pid;
            proc->priority = (*it)->priority;
            proc->arrivalT = (*it)->arrivalT;
            proc->burstT = (*it)->burstT;
            proc->remainingT = (*it)->remainingT;
            proc->responseT = (*it)->responseT;
            proc->completionT = (*it)->completionT;
            dst->push_back(proc);
    }
}

void
usage(const char progName[])
{
    cout << "usage: " << progName << " [SCHEDULINGS] [OPTIONS]" << endl;
    cout << "SCHEDULINGS:" << endl << 
        "\t-f\t\t\t\tFirst come first serve" << endl <<
        "\t-s pre|nopre\t\t\tShortest job first" << endl <<
        "\t-p pre|nopre\t\t\tPriority" << endl <<
        "\t-r\t\t\t\tRound Robin" << endl <<
        "\t-R\t\t\t\tPriority Round Robin" << endl;
    cout << "OPTIONS:" << endl << 
        "\t-c [n > 0]\t\t\tContext change time" << endl <<
        "\t-q [n > 0]\t\t\tTime quantum" << endl <<
        "\t-o file\t\t\t\tOutput file" << endl;
}

int main(int argc, char *argv[]) {

    // ./main isPreemtive cct
    if (argc < 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    bool schedules[] = {
        false,          // First come first serve
        false,          // Shortest job first
        false,          // Priority
        false,          // Round Robin
        false           // Priority round Robin
    };
    bool sjf_pre, prio_pre;
    char *output_file;
    int argIndex, cct = 0, flag, quantum = 2;

    while ((flag = getopt(argc, argv, "fs:p:rRq:c:o:")) != -1) {
        switch (flag) {
            case 'f':
                schedules[FCFS] = true;
                break;
            case 's':
                schedules[SJF] = true;
                if (strcmp(optarg, "pre") == 0)
                    sjf_pre = true;
                else if (strcmp(optarg, "nopre") == 0)
                    sjf_pre = false;
                else {
                    fprintf(stderr,
                            "Option -%c requires preemptive argument.\n",
                            optopt);
                    usage(argv[0]);
                    return 1;
                }
                break;
            case 'p':
                schedules[PRIO] = true;
                if (strcmp(optarg, "pre") == 0)
                    prio_pre = true;
                else if (strcmp(optarg, "nopre") == 0)
                    prio_pre = false;
                else {
                    fprintf(stderr,
                            "Option -%c requires preemptive argument.\n",
                            optopt);
                    usage(argv[0]);
                    return 1;
                }
                break;
            case 'r':
                schedules[RR] = true;
                break;
            case 'R':
                schedules[PRIO_RR] = true;
                break;
            case 'c':
                cct = atoi(optarg);
                break;
            case 'q':
                quantum = atoi(optarg);
                break;
            case 'o':
                output_file = optarg;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n",
                            optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort ();
        }
    }
    return 0;

    vector<process*> p1 = parser(), p2, p3, p4, p5;
    vector<gantt*> ganttDiagramS, ganttDiagramF, ganttDiagramP, ganttDiagramRR,
        ganttDiagramPrioRR;
    const int num_threads = 5;
    std::thread schedulings[num_threads];

    copyProccess(&p2, p1);
    copyProccess(&p3, p1);
    copyProccess(&p4, p1);
    copyProccess(&p5, p1);

    scheduleSJF s(p1);
    scheduleFCFS f(p2);
    schedulePrio p(p3);
    scheduleRR rr(p4, quantum);
    schedulePrioRR prr(p5, quantum);

    schedulings[0] = std::thread(processSJF,  &s, &ganttDiagramS, sjf_pre, cct); // scheduleSJF
    schedulings[1] = std::thread(processFCFS, &f, &ganttDiagramF, cct); // scheduleFCFS
    schedulings[2] = std::thread(processPRIO, &p, &ganttDiagramP, prio_pre, cct); // schedulePrio
    schedulings[3] = std::thread(processRR, &rr, &ganttDiagramRR, cct); // scheduleRR
    schedulings[4] = std::thread(processPrioRR, &prr, &ganttDiagramPrioRR, cct); // schedulePrioRR

    //Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i)
        schedulings[i].join();

    printTitle("scheduleSJF ");
    printGannttDiagram(ganttDiagramS);
    printAverage(s.getProcesses());

    printTitle("scheduleFCFS");
    printGannttDiagram(ganttDiagramF);
    printAverage(f.getProcesses());

    printTitle("schedulePrio");
    printGannttDiagram(ganttDiagramP);
    printAverage(p.getProcesses());

    printTitle("scheduleRR");
    printGannttDiagram(ganttDiagramRR);
    printAverage(rr.getProcesses());

    printTitle("schedulePrioRR");
    printGannttDiagram(ganttDiagramPrioRR);
    printAverage(prr.getProcesses());
    return 0;
}
