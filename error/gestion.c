/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:06 by vloth             #+#    #+#             */
/*   Updated: 2023/06/14 22:23:55 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Gestion des arguments
void the_arg(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		exit(EXIT_FAILURE);
}

// Gestion des erreur de malloc
int	malloc_error(char *str)
{
	if (!str)
	{
		ft_putendl_fd("Malloc failure", 2);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//fork error
pid_t fork_error(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd("Fork failure", 2);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	mini_error_file(char *file)
{
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(file, STDERR);
	ft_putendl_fd(": No such file or directory", STDERR);
}
