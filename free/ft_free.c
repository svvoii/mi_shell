/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:38 by vloth             #+#    #+#             */
/*   Updated: 2023/06/14 22:06:32 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_cmd(t_cmd *cmd, t_redir *redir)
{
	t_redir	*r_tmp;
	t_cmd	*tmp_cmd;

	while (cmd)
	{
		while (redir)
		{
			if (redir->file)
				free(redir->file);
			r_tmp = redir->next;
			free(redir);
			redir = NULL;
			redir = r_tmp;
		}
		free_tab(cmd->argv);
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->just_cmd)
			free(cmd->just_cmd);
		tmp_cmd = cmd->next;
		free(cmd);
		cmd = NULL;
		cmd = tmp_cmd;
	}
}

void	ft_free_all_memory(t_data *data)
{
	t_cmdIndex	*cmd_index;
	char		**path;

	cmd_index = data->cmdIndex;
	ft_free_cmd(cmd_index->begin, cmd_index->begin->lredir->begin);
	if (cmd_index)
		free(cmd_index);
	path = data->path_dirs;
	free_tab(path);
}
