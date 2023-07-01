#include <stdio.h>
#include <stdlib.h>

#include "Headers/Data.h"
#include "Headers/Commands.h"

const char* C_BLACK   = "\e[0;30m";
const char* C_RED     = "\e[0;31m";
const char* C_GREEN   = "\e[0;32m";
const char* C_YELLOW  = "\e[0;33m";
const char* C_BLUE    = "\e[0;34m";
const char* C_MAGENTA = "\e[0;35m";
const char* C_CYAN    = "\e[0;36m";
const char* C_WHITE   = "\e[0;37m";
const char* C_RESET   = "\e[0m";

const char*        TASK_FILE       = "./data/tasks.txt";
const char*        TEMP_FILE       = "./data/temp.txt";
const unsigned int MAX_LINE_LENGTH = 128;
const char*        SPLIT_TOKEN     = "%";

int main(int argc, char* argv[])
{
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

  if (argc == 1)
  {
    printf("%sTask Manager%s\n\n", C_BLUE, C_RESET);
    printTasks();
    return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "add") == 0)
  {
    if (argc < 3)
    {
      printf("%sMissing task text!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }
    char* taskText = malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(taskText, argv[2]);

    for (int i = 3; i < argc; i++)
    {
      strcat(taskText, " ");
      strcat(taskText, argv[i]);
    }

    addTaskToFile(taskText);
    free(taskText);
  }
  else if (strcmp(argv[1], "done") == 0)
  {
    if (argc < 3)
    {
      printf("%sMissing task ID!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }

    int taskId = atoi(argv[2]);
    if (taskId <= 0) {
      printf("%sInvalid task ID!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }

    return changeTaskStatus(taskId, 1);
  }
  else if (strcmp(argv[1], "undone") == 0)
  {
    if (argc < 3)
    {
      printf("%sMissing task ID!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }

    int taskId = atoi(argv[2]);
    if (taskId <= 0) {
      printf("%sInvalid task ID!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }

    return changeTaskStatus(taskId, 0);
  }
  else if (strcmp(argv[1], "delete") == 0)
  {
    if (argc < 3)
    {
      printf("%sMissing task ID!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }

    int taskId = atoi(argv[2]);
    if (taskId <= 0) {
      printf("%sInvalid task ID!%s\n", C_RED, C_RESET);
      return EXIT_FAILURE;
    }

    return deleteTask(taskId);
  }
  else if (strcmp(argv[1], "status") == 0)
  {
    printf("%sTask Manager%s\n\n", C_BLUE, C_RESET);
    printStatus();
  }
  else
  {
    printf("%sInvalid command: %s!%s\n", C_RED, argv[1], C_RESET);
  }

  return EXIT_SUCCESS;
}
