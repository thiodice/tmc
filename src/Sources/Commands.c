#include "../Headers/Commands.h"

extern const char*        TASK_FILE;
extern const char*        TEMP_FILE;
extern const unsigned int MAX_LINE_LENGTH;
extern const char*        SPLIT_TOKEN;

extern const char* C_BLACK;
extern const char* C_RED;
extern const char* C_GREEN;
extern const char* C_YELLOW;
extern const char* C_BLUE;
extern const char* C_MAGENTA;
extern const char* C_CYAN;
extern const char* C_WHITE;
extern const char* C_RESET;

void printTasks()
{
  char line[MAX_LINE_LENGTH];
  int lineNumber = 1;

  FILE* file = fopen(TASK_FILE, "r+");
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

int deleteTask(const unsigned int id)
{
  FILE* file = fopen(TASK_FILE, "r");
  FILE* tempFile = fopen(TEMP_FILE, "w");

  if (tempFile == NULL)
  {
    printf("%sFailed to create temporary file!%s\n", C_RED, C_RESET);
    fclose(file);
    return 1;
  }

  int taskId = 1;
  char line[MAX_LINE_LENGTH];
  bool found = false;

  while (fgets(line, sizeof(line), file))
  {
    if (taskId != id)
    {
        fputs(line, tempFile);
    }
    else
    {
      found = true;
    }
    taskId++;
  }

  fclose(file);
  fclose(tempFile);

  if (!found)
  {
    printf("%sTask with this ID does not exist!%s\n", C_RED, C_RESET);
    if (remove(TEMP_FILE) != 0)
    {
      printf("%sFailed to remove the temporary file!%s\n", C_RED, C_RESET);
    }
    return 1;
  }

  if (remove(TASK_FILE) != 0)
  {
    printf("%sFailed to remove the file!%s\n", C_RED, C_RESET);
    return 1;
  }

  if (rename(TEMP_FILE, TASK_FILE) != 0)
  {
    printf("%sFailed to rename the file!%s\n", C_RED, C_RESET);
    return 1;
  }

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
