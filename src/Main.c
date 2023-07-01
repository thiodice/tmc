#include <stdio.h>
#include <stdlib.h>

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

int main()
{
  printf("%sTask Manager%s\n\n", C_BLUE, C_RESET);
  for (int i = 0; i < 3; i++)
  {
    printf("%d) %s %s✗%s\n", i + 1, TASKS[i], C_RED, C_RESET);
  }

  return EXIT_SUCCESS;
}
