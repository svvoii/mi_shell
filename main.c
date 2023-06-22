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
		printf("\n\tcmd:[%s]\n", cmd->cmd);
		//printf("\tjust_cmd:[%s], built:[%d], redfir:[%d], meta:[%d]\n", cmd->just_cmd, cmd->is_built, cmd->redir, cmd->have_meta);
		printf("\toutput:[%s]\n", cmd->output);
		/*
		*/
		i = -1;
		while (cmd->argv[++i])
			printf("\targv[%d]: [%s]\n", i, cmd->argv[i]);
		printf("\n");
		/*
		i = -1;
		while (cmd->quoted_str && cmd->quoted_str[++i])
			printf("\tquoted_str[%d]: [%s]\n", i, cmd->quoted_str[i]);
		printf("\n");
		i = -1;
		while (cmd->arguments && cmd->arguments[++i])
			printf("\targuments[%d]: [%s]\n", i, cmd->arguments[i]);
		printf("\n");
		*/
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
			init_data_cmd(data);
			splitOrNot(str, data->cmdIndex); // after this f() each t_cmd holds part of the line which was split by '|' pipe

			/* here we split the part of the line from each cmd->cmd (str) into separate single and double quotes string which is stored in cmd->quoted_str.. 
			** see print_cmd with ex: (echo " $USER" ok '..fillow the white' rabbit "$FT_USER ..") */
			extract_quoted_str(data);
			parse_arguments_with_quotes(data);
			replace_envp_key_with_value(data);

			malloc_all(data); // initializes redirection if any, splits cmd into argv, arranges file_fds

			/* DEBUG */
			print_cmd(data);
			/* ***** */

			if (global.last_status == 0)
				exec(data);
		}
		/* debug */
		//printf("\tfree str: '%s'\n", str);
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
	eternal_loop(&data);
	
	printf("\texit. last_status: '%d'\n", global.last_status);

	free_tab(data.m_envp);
	rl_clear_history();
	return (0);
}
