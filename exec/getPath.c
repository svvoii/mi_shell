/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:51 by vloth             #+#    #+#             */
/*   Updated: 2023/06/14 22:20:59 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_getpath(char **m_envp)
{
	char	**path;
	int		i;

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
