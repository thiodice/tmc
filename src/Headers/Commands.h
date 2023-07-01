#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void printTasks();
void getStatusString(bool done, char* resultString);
void addTaskToFile(const char* taskText);
int  changeTaskStatus(const unsigned int id, const bool newStatus);
int  deleteTask(const unsigned int id);
void printStatus();

#endif // COMMANDS_H
