#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <unistd.h>

#include "gantt.h"
#include "scheduling.h"

#define FCFS    0
#define SJF     1
#define PRIO    2
#define RR      3
#define PRIO_RR 4
#define RM      5
#define EDF     6
#define NSCHDS  7

using namespace std;

struct sch_answer {
    vector<process*> initial_processes;
    vector<gantt*> ganttDiagram;
    vector<process*> final_processes;
};


void processSJF(scheduleSJF* s, sch_answer* sch , bool isPreemtive, int cct) {
    std::cout << "Launched by thread SJF" << std::endl;
    if (!isPreemtive)
        s->executeNonPreemptive(&(sch->ganttDiagram), cct);
    else
        s->executePreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}
void processFCFS(scheduleFCFS* s, sch_answer* sch, int cct) {
    std::cout << "Launched by thread FCFS" << std::endl;
    s->executeNonPreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}
void processPRIO(schedulePrio* s, sch_answer* sch, bool isPreemtive, int cct) {
    std::cout << "Launched by thread PRIO"<< std::endl;
    if (!isPreemtive)
        s->executeNonPreemptive(&(sch->ganttDiagram), cct);
    else
        s->executePreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}

void processRR(scheduleRR* s, sch_answer* sch, int cct) {
    std::cout << "Launched by thread RR"<< std::endl;
    s->executePreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}

void processPrioRR(schedulePrioRR* s, sch_answer* sch, int cct) {
    std::cout << "Launched by thread PrioRR"<< std::endl;
    s->executePreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}

void processRM(scheduleRM* s, sch_answer* sch, int cct) {
    std::cout << "Launched by thread RM"<< std::endl;
    s->executePreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}

void processEDF(scheduleEDF* s, sch_answer* sch, int cct) {
    std::cout << "Launched by thread EDF"<< std::endl;
    s->executePreemptive(&(sch->ganttDiagram), cct);
    sch->final_processes = s->getProcesses();
}

vector<process*> parser() {
    int nP;
    vector<process*> procVec;
    process* proc = new process;
    cin >> nP;
    while(nP--) {
        cin >> proc->pid >> proc->priority >> proc->arrivalT >> proc->burstT;
        proc->capacity = proc->priority;
        proc->deadline = proc->arrivalT;
        proc->period = proc->burstT;
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
        "\t-R\t\t\t\tPriority Round Robin" << endl <<
        "\t-m\t\t\t\tRate Monotonic" << endl <<
        "\t-e\t\t\t\tEarliest Deadline First" << endl;
    cout << "OPTIONS:" << endl <<
        "\t-c [n > 0]\t\t\tContext change time" << endl <<
        "\t-q [n > 0]\t\t\tTime quantum" << endl <<
        "\t-o file\t\t\t\tOutput file" << endl;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    bool schedules[] = {
        false,          // First come first serve
        false,          // Shortest job first
        false,          // Priority
        false,          // Round Robin
        false,          // Priority round Robin
        false,          // Rate monotonic
        false           // Earliest Deadline First
    };
    bool sjf_pre, prio_pre;
    char *output_file = NULL;
    int argIndex, cct = 0, flag, num_sch = 0, quantum = 2;
    ofstream filestr;

    while ((flag = getopt(argc, argv, "fs:p:rRq:c:o:mM:eE")) != -1) {
        switch (flag) {
            case 'f':
                schedules[FCFS] = true;
                num_sch++;
                break;
            case 's':
                schedules[SJF] = true;
                num_sch++;
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
                num_sch++;
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
                num_sch++;
                break;
            case 'R':
                schedules[PRIO_RR] = true;
                num_sch++;
                break;
            case 'm':
                schedules[RM] = true;
                num_sch++;
                break;
            case 'e':
                schedules[EDF] = true;
                num_sch++;
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
                if (optopt == 's' || optopt == 'p')
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

    if (output_file != NULL) {
        filestr.open (output_file);
        cout.rdbuf(filestr.rdbuf());        // assign streambuf to cout
    }

    vector<sch_answer> containers(num_sch);
    vector<sch_answer>::iterator contIt;
    containers[0].initial_processes = parser();
    for (contIt = containers.begin() + 1; contIt != containers.end(); contIt++)
        copyProccess(&(contIt->initial_processes),
                containers[0].initial_processes);

    thread schedulings[num_sch];

    for (int i = 0, nt = 0; i < NSCHDS; i++) {
        if (schedules[i]) {
            switch (i) {
                case FCFS:
                    schedulings[nt] = thread(processFCFS,
                            new scheduleFCFS(containers[nt].initial_processes),
                            &(containers[nt]), cct);
                    break;
                case SJF:
                    schedulings[nt] = thread(processSJF,
                            new scheduleSJF(containers[nt].initial_processes),
                            &(containers[nt]), sjf_pre, cct);
                    break;
                case PRIO:
                    schedulings[nt] = thread(processPRIO,
                            new schedulePrio(containers[nt].initial_processes),
                            &(containers[nt]), prio_pre, cct);
                    break;
                case RR:
                    schedulings[nt] = thread(processRR,
                            new scheduleRR(containers[nt].initial_processes, quantum),
                            &(containers[nt]), cct);
                    break;
                case PRIO_RR:
                    schedulings[nt] = thread(processPrioRR,
                            new schedulePrioRR(containers[nt].initial_processes, quantum),
                            &(containers[nt]), cct);
                    break;
                case RM:
                    schedulings[nt] = thread(processRM,
                            new scheduleRM(containers[nt].initial_processes),
                            &(containers[nt]), cct);
                    break;
                case EDF:
                    schedulings[nt] = thread(processEDF,
                            new scheduleEDF(containers[nt].initial_processes),
                            &(containers[nt]), cct);
                    break;
                default:
                    fprintf(stderr, "[ERROR] Unknown scheduling number %d", i);
                    abort();
            }
            nt++;
        }
    }

    //Join the threads with the main thread
    for (int i = 0; i < num_sch; ++i)
        schedulings[i].join();


    for (int i = 0, nt = 0; i < NSCHDS; i++) {
        if (schedules[i]) {
            switch (i) {
                case FCFS:
                    printTitle("scheduleSJF ");
                    break;
                case SJF:
                    printTitle("scheduleFCFS");
                    break;
                case PRIO:
                    printTitle("schedulePrio");
                    break;
                case RR:
                    printTitle("scheduleRR");
                    break;
                case PRIO_RR:
                    printTitle("schedulePrioRR");
                    break;
                case RM:
                    printTitle("scheduleRM");
                    break;
                case EDF:
                    printTitle("scheduleEDF");
                    break;
                default:
                    fprintf(stderr, "[ERROR] Unknown scheduling number %d", i);
                    abort();
            }
            printGannttDiagram(containers[nt].ganttDiagram);
            printAverage(containers[nt].final_processes);
            nt++;
        }
    }

    if (output_file != NULL)
        filestr.close();
    return 0;
}
