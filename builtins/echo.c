/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:14 by vloth             #+#    #+#             */
/*   Updated: 2023/06/22 19:14:25 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_echo_with_n(t_cmd *cmd)
{
	return (cmd->argv[0] && ft_strncmp(cmd->argv[0], "echo", 4) == 0
		&& cmd->argv[1] && ft_strncmp(cmd->argv[1], "-n", 2) == 0);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	bool	n_option;
	int		i;

	n_option = ft_echo_with_n(cmd);
	i = 0;
	if (n_option)
		i = 2;
	else
		i = 1;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
	data->exit_return = 0;
	return 0;
}

/*
int ft_echo_n(char *str, t_cmd *cmd)
{
	if (cmd->redir)
		str = cmd->just_cmd + 8;
	else
		str = cmd->cmd + 8;
	str = ft_strtrim(str, " ");
	ft_putstr_fd(str, 1);
	return 0;
}

void echo_redir(t_cmd *cmd, t_data *data, char *str)
{
	// this should handle multiple spaces as well !!!
	if (ft_strncmp(cmd->just_cmd, "echo -n ", 8)== 0)
	{
		ft_echo_n(str, cmd);
		data->exit_return = 0;
	}
	else
	{
		str = cmd->just_cmd + 5;
		str = ft_strtrim(str, " ");
		ft_putendl_fd(str, 1);
		data->exit_return = 0;
	}
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	char *str;

	str = NULL;
	if (cmd->redir)
		echo_redir(cmd, data, str);
	else
	{
		// this should handle multiple spaces as well !!!
		if (ft_strncmp(cmd->cmd, "echo -n ", 8)== 0)
		{
			ft_echo_n(str, cmd);
			data->exit_return = 0;
		}
		else
		{
			str = cmd->cmd + 5;
			str = ft_strtrim(str, " ");
			ft_putendl_fd(str, 1);
			data->exit_return = 0;
		}
	}
	return 0;
}
*/