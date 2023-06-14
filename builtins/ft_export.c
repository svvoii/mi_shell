/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:28 by vloth             #+#    #+#             */
/*   Updated: 2023/06/14 22:18:00 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		/* need to add this function in init_env.c */
		add_envp_variable(data->m_envp, cmd->argv[1]);
	}
	data->exit_return = 0;
	return (0);
}