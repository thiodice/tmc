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
const unsigned int MAX_LINE_LENGTH = 128;
const char* SPLIT_TOKEN = "%";

int createDataDirectory();
int createTaskFile();
void printTasks();
void getStatusString(bool done, char* resultString);
void addTaskToFile(const char* taskText);
int changeTaskStatus(const unsigned int id, const bool newStatus);
void printStatus();

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
  char line[MAX_LINE_LENGTH];
  int lineNumber = 1;

  FILE* file = fopen(TASK_FILE, "r");
  while (fgets(line, sizeof(line), file) != NULL)
  {
    char* taskText = strtok(line, SPLIT_TOKEN);
    if (taskText == NULL) return;

    char* statusPart = strtok(NULL, SPLIT_TOKEN);
    if (statusPart != NULL)
    {
      int status = atoi(statusPart);
      char statusString[32];
      getStatusString(status, statusString);
      printf("%d) %s %s\n", lineNumber, taskText, statusString);
    }

    lineNumber++;
  }

  if (lineNumber == 1)
  {
    printf("%sYou have no tasks added!%s\n", C_RED, C_RESET);
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

void addTaskToFile(const char* taskText)
{
  FILE* file = fopen(TASK_FILE, "a");

  fprintf(file, "%s%%%d\n", taskText, 0);
  fclose(file);
}

int changeTaskStatus(const unsigned int id, const bool newStatus)
{
  FILE* file = fopen(TASK_FILE, "r+");

  char line[MAX_LINE_LENGTH];
  int lineNumber = 1;
  bool found = false;

  while (fgets(line, sizeof(line), file))
  {
    char* taskText = strtok(line, SPLIT_TOKEN);
    if (taskText == NULL) return 1;

    char* statusPart = strtok(NULL, SPLIT_TOKEN);
    if (statusPart != NULL)
    {
      int status = atoi(statusPart);
      if (id == lineNumber)
      {
        status = newStatus;
        fseek(file, -strlen(statusPart), SEEK_CUR);
        fprintf(file, "%d", status);
        found = true;
      }
    }

    lineNumber++;
  }

  if (!found)
  {
    printf("%sTask with this ID does not exist!%s\n", C_RED, C_RESET);
    return 1;
  }

  fclose(file);
  return 0;
}

void printStatus()
{
  FILE* file = fopen(TASK_FILE, "r");

  char line[MAX_LINE_LENGTH];
  int taskCount = 0;
  int doneCount = 0;
  int undoneCount = 0;

  while (fgets(line, sizeof(line), file))
  {
    char* taskText = strtok(line, SPLIT_TOKEN);
    if (taskText == NULL) return;

    char* statusPart = strtok(NULL, SPLIT_TOKEN);
    if (statusPart != NULL)
    {
      int status = atoi(statusPart);
      status ? doneCount++ : undoneCount++;
    }

    taskCount++;
  }

  printf("Total tasks:   %s%d%s\n", C_YELLOW, taskCount, C_RESET);
  printf("Done tasks:    %s%d%s\n", C_GREEN, doneCount, C_RESET);
  printf("Undone tasks:  %s%d%s\n", C_RED, undoneCount, C_RESET);

  fclose(file);
}
