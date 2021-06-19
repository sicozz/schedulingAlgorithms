#ifndef  GANTT_H
#define GANTT_H
#include <iostream>
#include <vector>
#include <iomanip>
#include <stdio.h>
#include <string>
#define _RED     "\x1b[31m"
#define _GREEN   "\x1b[32m"
#define _YELLOW  "\x1b[33m"
#define _BLUE    "\x1b[34m"
#define _MAGENTA "\x1b[35m"
#define _CYAN    "\x1b[36m"
#define _RESET   "\x1b[0m"

struct gantt {
    std::string label;
    int i;
    int f;

};

void printGannttDiagram(std::vector<gantt*> );
std::string formatString(std::string, int );
void comparativaRealTime(std::vector<gantt*> ganttA, std::vector<gantt*> ganttB);

#endif
