/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:23:57 by vloth             #+#    #+#             */
/*   Updated: 2023/06/08 13:49:25 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
t_global global;

/* this is for debug */
void	print_cmd(t_data *data)
{
	t_cmd	*cmd = data->cmdIndex->begin;
	int		i;

	while (cmd)
	{
		printf("\tcmd:'%s'\tjust_cmd:'%s'\n", cmd->cmd, cmd->just_cmd);
		i = -1;
		while (cmd->argv[++i])
			printf("\targv[%d]: '%s'\n", i, cmd->argv[i]);
		cmd = cmd->next;
	}
}
/*********************/

//tout est dans le titre de ma fonction
/* Added 'empty_line' which returns true if the line is empty or only spaces. See 'spaces' in utils_nd */
void	eternal_loop(t_data *data)
{
	char *str;

	while(1)
	{
		/* if enter is pressed on an empty line segfault occurs and program exit with an error from sigquit_handler.. */
		signal_handler();

		str = readline("MS#🤖: ");

		/* add history only if it is not empty */
		if (!empty_line(str))
			add_history(str);
		else
			continue ;
		
		/* debug */
		printf("\tstr: '%s'\n", str);
		/********/

		init_data_cmd(data, str);

		splitOrNot(str, data->cmdIndex);
		malloc_all(data);

		//printf("OK so far\n");

		exec(data);
		free(str);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data data;

	the_arg(argc, argv);
	init_data(&data, envp);
	eternal_loop(&data);
	rl_clear_history();
	return (0);
}
