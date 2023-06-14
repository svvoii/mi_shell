/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:23:57 by vloth             #+#    #+#             */
/*   Updated: 2023/06/13 16:42:37by sbocanci         ###   ########.fr       */
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
	char	*str;

	while(1)
	{
		global.last_status = 0;

		str = readline("MS#🤖: ");

		/* add history only if it is not empty */
		if (!str)
		{
			printf("\tbreak");
			break ;
		}
		if (empty_line(str))
			continue ;
		else
		{
			add_history(str);
			//init_data_cmd(data, str);
			init_data_cmd(data);
			splitOrNot(str, data->cmdIndex);
			malloc_all(data);

			if (global.last_status == 0)
				exec(data);
		}
		/* debug */
		//printf("\tstr: '%s'\n", str);
		/********/

		free(str);
		//printf("\tloop. last_status:'%d'\n", global.last_status);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data data;

	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);

	the_arg(argc, argv);
	init_data(&data, envp);
	/* debug from here */
	eternal_loop(&data);
	
	printf("\texit. last_status: '%d'\n", global.last_status);
	//ft_free_env(data.env);
	free_tab(data.m_envp);
	rl_clear_history();
	return (0);
}
