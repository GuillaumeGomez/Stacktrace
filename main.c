#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#ifdef WIN32
#else
#include <unistd.h>
#endif
#include "stacktrace.h"

void	crash_function()
{
#ifdef WIN32
	abort();
#else
	int	x = 1;

	x = 0;
	printf("%d\n", 10 / x);
#endif
}

static void	useless_function2()
{
	crash_function();
}

void	useless_function()
{
	printf("Hello people !\n");
	useless_function2();
}

int main() {
	if (!STACK_INIT) {
		fprintf(stderr, "Couldn't init stack...\n");
		return EXIT_FAILURE;
	}
#ifdef WIN32
	STACK_ADD_SIGNAL(SIGTERM);
#endif
	STACK_NOT_PRINT_FULL;
	STACK_CHANGE_OUTPUT_FILE("output.txt");
	useless_function();
	return EXIT_SUCCESS;
}
