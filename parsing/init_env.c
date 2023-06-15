/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:04 by vloth             #+#    #+#             */
/*   Updated: 2023/06/15 17:36:23 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_envp(t_data *data)
{
	//char	**my_envp;
	int		count;

	data->m_envp = (char **)ft_calloc(BUFFER_SIZE_MAX, sizeof(char*));
	if (!data->m_envp)
		return ;
	count = 0;
	while (data->envp[count])
	{
		data->m_envp[count] = ft_strdup(data->envp[count]);
		count++;
	}
	while (count < BUFFER_SIZE_MAX)
	{
		data->m_envp[count] = NULL;
		count++;
	}
	//printf("\ninit_env. count:'%d', buff:'%d'\n", count, BUFFER_SIZE_MAX);
	//data->m_envp[count] = NULL;
	//data->m_envp = my_envp;
}

/* this adds new key=value pair to the data->m_envp. However, next call of env command wont show it ?! */
void	add_envp_variable(t_data *data, char *key, char *value)
{
	int		i;
	char	*new_var;

	new_var = ft_strjoin(key, value);
	if (!new_var)
		return ;
	i = 0;
	while (data->m_envp[i])
	{
		if (ft_strncmp(data->m_envp[i], key, ft_strlen(key)) == 0)
		{
			free(data->m_envp[i]);
			data->m_envp[i] = new_var;
			return ;
		}
		//printf("\t[%d] @ '%p'\n", i, data->m_envp[i]);
		i++;
	}
	if (i < BUFFER_SIZE_MAX - 1)
	{
		data->m_envp[i] = new_var;
		data->m_envp[i + 1] = NULL;
	}
	else
		free(new_var);
}
//printf("\nadd. key:'%s' = value:'%s', [%d]\n", key, value, i);
//printf("\nm_envp[%d]:'%s' @ '%p'\n", i, data->m_envp[i], data->m_envp[i]);

void	change_pwd(char **m_envp, char *key)
{	
	char	*get_cwd;
	int		i;
	
	i = 0;
	while (m_envp[i])
	{
		if (ft_strncmp(m_envp[i], key, ft_strlen(key)) == 0)
			break ;
		i++;
	}
	get_cwd = getcwd(NULL, 0);
	if (m_envp[i])
		free(m_envp[i]);
	m_envp[i] = ft_strjoin(key, get_cwd);
	free(get_cwd);
}
