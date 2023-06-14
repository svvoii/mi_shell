/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:04 by vloth             #+#    #+#             */
/*   Updated: 2023/06/14 22:26:50 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_envp(t_data *data)
{
	char	**my_envp;
	int		count;

	my_envp = (char **)ft_calloc(BUFFER_SIZE_MAX, sizeof(char*));
	if (!my_envp)
		return ;
	count = 0;
	while (data->envp[count])
	{
		my_envp[count] = ft_strdup(data->envp[count]);
		count++;
	}
	my_envp[count] = NULL;
	data->m_envp = my_envp;
}

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

void	add_envp_variable(char **envp, char *key)
{
	(void)envp;
	(void)key;
}
