#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#include <DbgHelp.h>
#include <signal.h>
#else
#include <execinfo.h>
#include <unistd.h>
#endif
#include "stacktrace.h"

unsigned int	g_stack_display_size = 20;
int				g_print_full_stack = 0;
char			*g_stack_output_file = NULL;

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
	FILE		*fd = stderr;
	int			need_close = 0;

	if (g_stack_output_file && strlen(g_stack_output_file)) {
		if (!(fd = fopen(g_stack_output_file, "w")))
			fd = stderr;
		else {
			need_close = 1;
		}
	}
#ifdef WIN32
	void				**frames;
	int					frameCount;
	char				symbolName[MAX_SYM_NAME + 1];
	char				buffer[sizeof(IMAGEHLP_SYMBOL64) + MAX_SYM_NAME * sizeof(TCHAR)] = { 0 };
	IMAGEHLP_LINE64		line;
	DWORD64				dis64 = 0;
	DWORD				dis = 0;
	IMAGEHLP_SYMBOL64	*pSym = NULL;
	BOOL				res;
	int					count = 0;

	if (!(frames = malloc(sizeof(*frames) * g_stack_display_size))) {
		fprintf(stderr, "Cannot allocate enough memory to stacktrace...\n");
		exit(EXIT_FAILURE);
	}
	frameCount = CaptureStackBackTrace(1, g_stack_display_size, frames, NULL);
	pSym = (IMAGEHLP_SYMBOL64 *)buffer;
	pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
	pSym->MaxNameLength = MAX_PATH;

	while (frameCount-- > 0) {
		if ((res = SymGetSymFromAddr64(GetCurrentProcess(), frames[frameCount], &dis64, pSym))) {
			strcpy(symbolName, pSym->Name);
		}

		memset(&line, 0, sizeof(line));
		line.SizeOfStruct = sizeof(line);
		
		if (!(res = SymGetLineFromAddr64(GetCurrentProcess(), frames[frameCount], &dis, &line))) {
			continue;
		} else {
			fprintf(fd, "%d -> %s (%s, %d)\n", ++count, symbolName, line.FileName, line.LineNumber);
		}
	}
	free(frames);
#else
	void		**array;
	size_t		size;
	int			i;
	char		**strings;

	if (!(array = malloc(sizeof(*array) * (g_stack_display_size + 1)))) {
		fprintf(stderr, "Cannot allocate enough memory to stacktrace...\n");
		exit(EXIT_FAILURE);
	}
	size = backtrace(array, g_stack_display_size);
	if (!(strings = backtrace_symbols(array, size)))
		perror("backtrace_symbols");
	else {
		for (i = (g_print_full_stack ? 0 : 2); i < size; ++i)
			fprintf(fd, "%d -> %s\n", (int)(size - i), strings[i]);
		free(strings);
	}
	free(array);
#endif
	if (need_close) {
		fclose(fd);
		fprintf(stderr, "Data wrote in %s\n", g_stack_output_file);
	}
	exit(EXIT_SUCCESS);
}

void	set_print_full_stack(unsigned int i)
{
	g_print_full_stack = (!i ? 0 : 1);
}

static void	clearMemory(void)
{
	if (g_stack_output_file)
		free(g_stack_output_file);
#ifdef WIN32
#endif
}

void	stacktrace_add_signal(int sig)
{
	signal(sig, signal_handler);
}

int	init_stacktrace()
{
#ifdef WIN32
	BOOL initRes;

	SymSetOptions(SYMOPT_LOAD_LINES);
	initRes = SymInitialize(GetCurrentProcess(), NULL, TRUE);

	if (!initRes) {
		fprintf(stderr, "SymInitialize failed with error %d", GetLastError());
		return 0;
	}

#endif
	stacktrace_add_signal(SIGFPE);  // divide by 0
	stacktrace_add_signal(SIGSEGV); // segmentation fault
	stacktrace_add_signal(SIGABRT);
	atexit(clearMemory);
	return 1;
}