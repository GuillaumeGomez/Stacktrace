#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <tchar.h>
#include <dbghelp.h>
#else
#include <execinfo.h>
#include <unistd.h>
#endif
#include "stacktrace.h"

unsigned int	g_stack_display_size = 20;
int		g_print_full_stack = 0;
char		*g_stack_output_file = NULL;

void	change_stack_display_size(unsigned int x)
{
  g_stack_display_size = (x > STACK_MAX_DISPLAY_SIZE ? STACK_MAX_DISPLAY_SIZE : x);
}

void	set_output_file(const char *s)
{
  if (!s || !strlen(s))
    return;
  if (g_stack_output_file)
    free(g_stack_output_file);
  g_stack_output_file = strdup(s);
}

void	signal_handler(int sig)
{
  void		**array;
  size_t	size;
  FILE		*fd = stderr;
  int		need_close = 0;
  int		i;
  char		**strings;

  if (!(array = malloc(sizeof(*array) * (g_stack_display_size + 1)))) {
    fprintf(stderr, "Cannot allocate enough memory to stacktrace...\n");
    exit(EXIT_FAILURE);
  }
  size = backtrace(array, g_stack_display_size);

  if (g_stack_output_file && strlen(g_stack_output_file)) {
    if (!(fd = fopen(g_stack_output_file, "w")))
      fd = stderr;
    else {
      need_close = 1;
    }
  }
  if (!(strings = backtrace_symbols(array, size)))
    perror("backtrace_symbols");
  else {
    for (i = (g_print_full_stack ? 0 : 2); i < size; ++i)
      fprintf(fd, "%d -> %s\n", (int)(size - i), strings[i]);
    free(strings);
  }
  if (need_close) {
    fclose(fd);
    fprintf(stderr, "Data wrote in %s\n", g_stack_output_file);
  }
  free(array);
  exit(EXIT_SUCCESS);
}

void	set_print_full_stack(unsigned int i)
{
  g_print_full_stack = (!i ? 0 : 1);
}

static void	clearMemory()
{
  if (g_stack_output_file)
    free(g_stack_output_file);
}

void	stacktrace_add_signal(int sig)
{
  signal(sig, signal_handler);
}

void	init_stacktrace()
{
#ifdef WIN32
  SymSetOptions(SYMOPT_LOAD_LINES);
  if (!SymInitialize(GetCurrentProcess(), NULL, TRUE)) {
    fprintf(stderr, _T("SymInitialize failed with error %d"), GetLastError());
    return;
  }
#endif
  stacktrace_add_signal(SIGFPE);
  stacktrace_add_signal(SIGSEGV);
  atexit(clearMemory);
}
