/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:31 by vloth             #+#    #+#             */
/*   Updated: 2023/06/16 14:57:41 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//renvoie la la place du egale dans la cmd
int search_egal(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/* As well as export command unset have to be executed out side child process 
** added it to spec_built_first() in exec.c */
int ft_unset(t_cmd *cmd, t_data *data)
{
	char	**name;
	int		i;
	int		j;

	name = ft_split(cmd->cmd, ' ');
	if (name[1] != NULL)
	{
		i = 0;
		while (data->m_envp[i])
		{
			if (ft_strncmp(data->m_envp[i], name[1], search_egal(data->m_envp[i])) == 0)
			{
				free(data->m_envp[i]);
				j = i;
				while (data->m_envp[++j])
					data->m_envp[j - 1] = data->m_envp[j];
				data->m_envp[j - 1] = NULL;
				break ;
			}
			i++;
		}
	}
	data->exit_return = 0;
	return (0);
}
