#ifndef __STACKTRACE_H__
#define __STACKTRACE_H__

#define STACK_CHANGE_DISPLAY_SIZE(x)	change_stack_display_size(x)
#define STACK_CHANGE_OUTPUT_FILE(x)	set_output_file(x)
#define	STACK_PRINT_FULL		set_print_full_stack(1)
#define	STACK_NOT_PRINT_FULL		set_print_full_stack(0)
#define STACK_ADD_SIGNAL(x)		stacktrace_add_signal(x)
#define STACK_INIT			init_stacktrace()
#define STACK_MAX_DISPLAY_SIZE		1000

int		init_stacktrace();
void	set_output_file(const char *filename);
void	stacktrace_add_signal(int sig);
void	set_print_full_stack(unsigned int option);
void	change_stack_display_size(unsigned int new_size);

#endif
