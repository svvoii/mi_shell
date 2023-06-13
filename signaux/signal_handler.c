/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:24:30 by vloth             #+#    #+#             */
/*   Updated: 2023/06/13 14:23:08 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
void	newline(void);
void	sigint_handler(int sig);
void 	sigquit_handler(int sig);
void	signal_handler(void);
*/
//GESTION DES SIGNAUX

void	newline(void)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler(int sig)
{
	//(void)sig;
	printf("\tsig_handl: '%d', pid: '%d'", sig, global.pid);
	global.signal = sig;
	write(1, "\n", 1);
	newline();
	if (global.pid == 0)
		rl_redisplay();
}

void sigquit_handler(int sig)
{
	(void)sig;
	/*
	ft_putendl_fd("CTRL-D = Segmentation fault", 2);
	printf("\tsigquit: '%d', SIGSEGV: '%d'\n", sig, SIGSEGV);
	global.signal = sig;
	exit(128);
	*/
}

void	signal_handler(void)
{
	signal(SIGINT, &sigint_handler); // ctrl-C is pressed
	signal(SIGQUIT, SIG_IGN);
	signal(SIGSEGV, &sigquit_handler);
}

/*
SIGINT, SIGQUIT, and SIGSEGV are all signals in UNIX-based operating systems 
that are used to notify a process about certain events or conditions. 
Here's a brief explanation of each signal:

SIGINT: This signal, which stands for "signal interrupt," 
is typically generated by pressing CTRL-C in a terminal window. 
When a process receives a SIGINT signal, it typically terminates gracefully, 
freeing up any resources it was using. In the context of the 
code snippet you provided, the sigint_handler() function is 
called when a SIGINT signal is received, which clears the current 
input line using readline library functions.

SIGQUIT: This signal, which stands for "signal quit," is typically 
generated by pressing CTRL-\ in a terminal window. When a process 
receives a SIGQUIT signal, it usually terminates abruptly, 
leaving behind a core dump file that can be used for debugging 
purposes. In the context of the code snippet you provided, 
the sigquit_handler() function is called when a SIGQUIT signal 
is received, which writes an error message to the standard 
error file descriptor and then exits the program with exit status 128.

SIGSEGV: This signal, which stands for "segmentation violation," 
is generated when a process attempts to access an area of memory 
that is not allowed. This can happen, for example, if a process 
tries to read from or write to a null pointer, or if it tries to 
access memory that has already been freed. 
When a process receives a SIGSEGV signal, it typically terminates 
abruptly, leaving behind a core dump file that can be 
used for debugging purposes. The sigquit_handler() function is called 
when a SIGSEGV signal is received, which writes an error message 
to the standard error file descriptor and then exits the program 
with exit status 128.
*/