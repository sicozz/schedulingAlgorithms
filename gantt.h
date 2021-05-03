#ifndef  GANTT_H
#define GANTT_H
#include <iostream>
#include <vector>
#include <iomanip>
#include <stdio.h>
#include <string>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct gantt {
    std::string label;
    int i;
    int f;

};

void printGannttDiagram(std::vector<gantt*> );
std::string formatString(std::string, int );

#endif
