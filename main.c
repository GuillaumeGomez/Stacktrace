#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "stacktrace.h"

void	crash_function()
{
  int	x = 1;

  x = 0;
  printf("%d\n", 10 / x);
}

static void	useless_function2()
{
  crash_function();  
}

void	useless_function()
{
  useless_function2();
}

int main() {
  STACK_INIT
#ifdef WIN32
  STACK_ADD_SIGNAL(SIGTERM)
#endif
  STACK_NOT_PRINT_FULL
  STACK_CHANGE_OUTPUT_FILE("output.txt")
  useless_function();
  return 1;
}
