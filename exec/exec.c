/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:49 by vloth             #+#    #+#             */
/*   Updated: 2023/06/16 09:53:36 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* DEBUG */
void	ft_print_envp(char **envp)
{
	int	i = 0;
	while (envp[i])
	{
		printf("[%d]: '%s'\n", i, envp[i]);
		i++;
	}
}
/*********/

// fonction d'execution
void ft_execve(t_cmd *cmd, t_data *data)
{
	int y;
	char *exec;

	//no_str(cmd->cmd);
	y = 0;
	while (data->path_dirs[++y])
	{
		exec = ft_strjoin(ft_strjoin(data->path_dirs[y], "/"), cmd->argv[0]);
		//printf("execve, path'%s', pid:'%d'\n", exec, global.pid);
		data->exit_return = execve(exec, cmd->argv, data->envp);
		//printf("exit_return: '%d'\n", data->exit_return);
		free(exec);
	}
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	data->exit_return = errno;
	exit(data->exit_return);
}

//regarde quel execution faire si une ou plusieur cmd
//void exec(t_cmdIndex *cmd, char **envp, t_envSom *envp_nodes)
void exec(t_data *data)
{
	t_cmdIndex *index;

	index = data->cmdIndex;
	spec_built_first(data);
	if (index->nb_cmd)
	{
		ft_ft_exec(data);
	}
	//ft_print_envp(data->m_envp);
}

void spec_built(t_cmd *cmd, t_data * data)
{
	if (cmd->spec_built == 1)
		ft_exit(cmd, data);
	else if (cmd->spec_built == 2)
		ft_cd(cmd, data);
	else if (cmd->spec_built == 3)
		ft_export(cmd, data);
	else if (cmd->spec_built == 4)
		ft_unset(cmd, data);
	else
		return ;
}

void spec_built_first(t_data *data)
{
	t_cmd *cmd;

	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		if (ft_strcmp(cmd->argv[0], "unset") == 0)
			cmd->spec_built = 4;
		else if (ft_strcmp(cmd->argv[0], "export") == 0)
			cmd->spec_built = 3;
		else if (ft_strcmp(cmd->argv[0], "cd") == 0)
			cmd->spec_built = 2;
		else if (ft_strcmp(cmd->argv[0], "exit") == 0)
			cmd->spec_built = 1;
		else
			cmd->spec_built = 0;
		cmd = cmd->next;
	}
}
/*
void spec_built(t_cmd *cmd, t_data * data)
{
	if (cmd->spec_built == 1)
		ft_exit(cmd, data);
	else if (cmd->spec_built == 2)
		ft_cd(cmd, data);
	else
		return ;
}

void spec_built_first(t_data *data)
{
	t_cmd *cmd;

	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		if (ft_strcmp(cmd->argv[0], "cd") == 0)
			cmd->spec_built = 2;
		else if (ft_strcmp(cmd->argv[0], "exit") == 0)
			cmd->spec_built = 1;
		else
			cmd->spec_built = 0;
		cmd = cmd->next;
	}
}
*/