#include <stdio.h>
#include <stdlib.h>

#include "Headers/Data.h"
#include "Headers/Commands.h"

const char* C_RED    = "\e[0;31m";
const char* C_GREEN  = "\e[0;32m";
const char* C_YELLOW = "\e[0;33m";
const char* C_BLUE   = "\e[0;34m";
const char* C_RESET  = "\e[0m";

const char*        DATA_FOLDER     = "tmc_data";
const char*        TASK_FILE       = "tasks.txt";
const char*        TEMP_FILE       = "temp.txt";
const unsigned int MAX_LINE_LENGTH = 128;
const char*        SPLIT_TOKEN     = "%";

int main(int argc, char* argv[])
{
  if (createDataDirectory() == EXIT_FAILURE)
  {
    printf("%sFailed to create the data folder!%s\n", C_RED, C_RESET);
    return EXIT_FAILURE;
  }

  if (createTaskFile() == EXIT_FAILURE)
  {
    printf("%sFailed to create the task file!%s\n", C_RED, C_RESET);
    return EXIT_FAILURE;
  }

  return handleCommand(argc, argv);
}
