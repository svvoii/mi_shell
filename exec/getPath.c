/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:51 by vloth             #+#    #+#             */
/*   Updated: 2023/06/14 18:58:11 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Recup PATH dans mon env et le split avec : pour mon exec
/* This saves the PATH variable directories in separate strings
** MALLOC (split) need to be freed */
char	**ft_getpath(char **m_envp)
{
	char	**path;
	int		i;
	//t_env 	*tmp;

	i = 0;
	while (m_envp[i])
	{
		if (ft_strncmp(m_envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	path = ft_split(m_envp[i] + 5, ':');
	return (path);
}

/*
char	**ft_getpath(t_envSom *env)
{
	char	**path;
	t_env 	*tmp;

	tmp = env->begin;
	while (tmp && ft_strncmp(tmp->name, "PATH=", 5))
		tmp = tmp->next;
	path = ft_split(tmp->name + 5, ':');
	return (path);
}
*/