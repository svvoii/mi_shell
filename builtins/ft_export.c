/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:28 by vloth             #+#    #+#             */
/*   Updated: 2023/06/15 17:37:49 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* DEBUG */
void	print_envp(char **envp)
{
	int	i = 0;
	while (envp[i])
	{
		printf("[%d]: '%s'\n", i, envp[i]);
		i++;
	}
}
/*********/

void	put_envp(char **m_envp)
{
	int	i;

	i = 0;
	while (m_envp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(m_envp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}

int ft_export(t_cmd *cmd, t_data *data)
{
	char	*key;
	int		i;

	if (cmd->argv[1] == NULL)
		put_envp(data->m_envp);
	else
	{
		i = 0;
		while (cmd->argv[1])
		{
			if (cmd->argv[1][i++] == '=')
				break ;
		}
		key = ft_substr(cmd->argv[1], 0, i);
		printf("\tft_export. key: '%s', value: '%s'\n", key, cmd->argv[1] + i);
		//add_envp_variable(data->m_envp, key, cmd->argv[1] + ft_strlen(key) + 1);
		add_envp_variable(data, key, cmd->argv[1] + i);
		free(key);
	}
	//print_envp(data->m_envp);
	data->exit_return = 0;
	return (0);
}

/*
int ft_export(t_cmd *cmd, t_data *data)
{
	int i;

	i = 0;
	if (cmd->argv[1] == NULL)
	{
		while (data->m_envp[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(data->m_envp[i], 1);
			ft_putchar_fd('\n', 1);
			i++;
		}
	}
	else
	{
		add_envp_variable(data->m_envp, cmd->argv[1]);
	}
	data->exit_return = 0;
	return (0);
}
*/