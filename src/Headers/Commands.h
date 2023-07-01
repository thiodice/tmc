#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "Data.h"

int  printTasks();
void getStatusString(bool done, char* resultString);
int  addTaskToFile(const char* taskText);
int  changeTaskStatus(const unsigned int id, const bool newStatus);
int  deleteTask(const unsigned int id);
int  printStatus();
int  handleCommand(int argc, char* argv[]);

#endif // COMMANDS_H
