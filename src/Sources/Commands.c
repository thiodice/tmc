#include "../Headers/Commands.h"

extern const char*        DATA_FOLDER;
extern const char*        TASK_FILE;
extern const char*        TEMP_FILE;
extern const unsigned int MAX_LINE_LENGTH;
extern const char*        SPLIT_TOKEN;

extern const char* C_RED;
extern const char* C_GREEN;
extern const char* C_YELLOW;
extern const char* C_BLUE;
extern const char* C_RESET;

int printTasks()
{
  const char *homeDir = getenv("HOME");
  char taskPath[96];

  snprintf(taskPath, sizeof(taskPath), "%s/%s/%s", homeDir, DATA_FOLDER, TASK_FILE);

  char line[MAX_LINE_LENGTH];
  int lineNumber = 1;

  FILE* file = fopen(taskPath, "r+");
  while (fgets(line, sizeof(line), file) != NULL)
  {
    char* taskText = strtok(line, SPLIT_TOKEN);
    if (taskText == NULL) return EXIT_FAILURE;

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
  return EXIT_SUCCESS;
}

void getStatusString(bool done, char* resultString)
{
  const char* colorString = done ? C_GREEN : C_RED;
  const char* statusString = done ? "✓" : "✗";

  strcpy(resultString, colorString);
  strcat(resultString, statusString);
  strcat(resultString, C_RESET);
}

int addTaskToFile(const char* taskText)
{
  const char *homeDir = getenv("HOME");
  char taskPath[96];

  snprintf(taskPath, sizeof(taskPath), "%s/%s/%s", homeDir, DATA_FOLDER, TASK_FILE);

  FILE* file = fopen(taskPath, "a");
  fprintf(file, "%s%%%d\n", taskText, 0);
  fclose(file);
  
  return EXIT_SUCCESS;
}

int changeTaskStatus(const unsigned int id, const bool newStatus)
{
  const char *homeDir = getenv("HOME");
  char taskPath[96];

  snprintf(taskPath, sizeof(taskPath), "%s/%s/%s", homeDir, DATA_FOLDER, TASK_FILE);

  FILE* file = fopen(taskPath, "r+");

  char line[MAX_LINE_LENGTH];
  int lineNumber = 1;
  bool found = false;

  while (fgets(line, sizeof(line), file))
  {
    char* taskText = strtok(line, SPLIT_TOKEN);
    if (taskText == NULL) return EXIT_FAILURE;

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
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int deleteTask(const unsigned int id)
{
  const char *homeDir = getenv("HOME");
  char taskPath[96];
  char tempPath[96];

  snprintf(taskPath, sizeof(taskPath), "%s/%s/%s", homeDir, DATA_FOLDER, TASK_FILE);
  snprintf(tempPath, sizeof(tempPath), "%s/%s/%s", homeDir, DATA_FOLDER, TEMP_FILE);

  FILE* file = fopen(taskPath, "r");
  FILE* tempFile = fopen(tempPath, "w");

  if (tempFile == NULL)
  {
    printf("%sFailed to create temporary file!%s\n", C_RED, C_RESET);
    fclose(file);
    return EXIT_FAILURE;
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
    if (remove(tempPath) != 0)
    {
      printf("%sFailed to remove the temporary file!%s\n", C_RED, C_RESET);
    }
    return EXIT_FAILURE;
  }

  if (remove(taskPath) != 0)
  {
    printf("%sFailed to remove the file!%s\n", C_RED, C_RESET);
    return EXIT_FAILURE;
  }

  if (rename(tempPath, taskPath) != 0)
  {
    printf("%sFailed to rename the file!%s\n", C_RED, C_RESET);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int printStatus()
{
  const char *homeDir = getenv("HOME");
  char taskPath[96];

  snprintf(taskPath, sizeof(taskPath), "%s/%s/%s", homeDir, DATA_FOLDER, TASK_FILE);

  FILE* file = fopen(taskPath, "r");

  char line[MAX_LINE_LENGTH];
  int taskCount = 0;
  int doneCount = 0;
  int undoneCount = 0;

  while (fgets(line, sizeof(line), file))
  {
    char* taskText = strtok(line, SPLIT_TOKEN);
    if (taskText == NULL) return EXIT_FAILURE;

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
  return EXIT_SUCCESS;
}

int handleCommand(int argc, char* argv[])
{
  if (argc == 1)
  {
    printf("%sTask Manager%s\n\n", C_BLUE, C_RESET);
    return printTasks();
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

    int success = addTaskToFile(taskText);
    free(taskText);
    return success;
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
    return printStatus();
  }
  else
  {
    printf("%sInvalid command: %s!%s\n", C_RED, argv[1], C_RESET);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
