#include "../Headers/Data.h"

extern const char* TASK_FILE;
extern const char* TEMP_FILE;

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
