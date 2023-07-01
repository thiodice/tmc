#include "../Headers/Data.h"

extern const char* DATA_FOLDER;
extern const char* TASK_FILE;
extern const char* TEMP_FILE;

int createDataDirectory()
{
  const char *homeDir = getenv("HOME");
  char path[96];

  snprintf(path, sizeof(path), "%s/%s", homeDir, DATA_FOLDER);

  struct stat st;
  if (stat(path, &st) == 0)
  {
    if (S_ISDIR(st.st_mode))
    {
      return EXIT_SUCCESS;
    }
  }

  int result = mkdir(path, S_IRWXU);
  return result != 0;
}

int createTaskFile()
{
  const char *homeDir = getenv("HOME");
  char path[96];

  snprintf(path, sizeof(path), "%s/%s/%s", homeDir, DATA_FOLDER, TASK_FILE);

  if (access(path, F_OK) == 0)
  {
    return EXIT_SUCCESS;
  }

  FILE* taskFile = fopen(path, "w");  
  if (taskFile == NULL)
  {
    return EXIT_FAILURE;
  }

  fclose(taskFile);
  return EXIT_SUCCESS;
}
