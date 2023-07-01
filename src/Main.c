#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

const char* C_BLACK   = "\e[0;30m";
const char* C_RED     = "\e[0;31m";
const char* C_GREEN   = "\e[0;32m";
const char* C_YELLOW  = "\e[0;33m";
const char* C_BLUE    = "\e[0;34m";
const char* C_MAGENTA = "\e[0;35m";
const char* C_CYAN    = "\e[0;36m";
const char* C_WHITE   = "\e[0;37m";
const char* C_RESET   = "\e[0m";

const char* TASKS[] = {
  "Task 1",
  "Task 2",
  "Task 3",
};
const bool TASKS_DONE[] = {
  true,
  true,
  false,
};
const int NUM_TASKS = sizeof(TASKS) / sizeof(TASKS[0]);

const char* TASK_FILE = "./data/tasks.txt";
const char* SPLIT_TOKEN = "%";

int createDataDirectory();
int createTaskFile();
void printTasks();
void getStatusString(bool done, char* resultString);

int main()
{
  printf("%sTask Manager%s\n\n", C_BLUE, C_RESET);

  if (createDataDirectory() == 1)
  {
    printf("Failed to create the data folder!\n");
    return 1;
  }

  if (createTaskFile() == 1)
  {
    printf("Failed to create the task file!\n");
    return 1;
  }

  printTasks();

  return EXIT_SUCCESS;
}

int createDataDirectory()
{
  const char* folderPath = "./data";
  int result = mkdir(folderPath, 0777);
  return result;
}

int createTaskFile()
{
  if (access(TASK_FILE, F_OK) != -1)
  {
    return 0;
  }

  FILE* taskFile = fopen(TASK_FILE, "w");  
  if (taskFile == NULL)
  {
    return 1;
  }

  fclose(taskFile);
  return 0;
}

void printTasks()
{
  const int maxLineLength = 128;
  char line[maxLineLength];
  int lineNumber = 1;

  FILE* file = fopen(TASK_FILE, "r");
  while (fgets(line, maxLineLength, file) != NULL) {
    char* taskText = strtok(line, SPLIT_TOKEN);

    if (taskText != NULL) {
      char* statusPart = strtok(NULL, SPLIT_TOKEN);

      if (statusPart != NULL) {
        int status = atoi(statusPart);
        char statusString[32];
        getStatusString(status, statusString);
        printf("%d) %s %s\n", lineNumber, taskText, statusString);
      }
    }

    lineNumber++;
  }

  fclose(file);
}

void getStatusString(bool done, char* resultString)
{
  const char* colorString = done ? C_GREEN : C_RED;
  const char* statusString = done ? "✓" : "✗";

  strcpy(resultString, colorString);
  strcat(resultString, statusString);
  strcat(resultString, C_RESET);
}
