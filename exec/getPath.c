/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:51 by vloth             #+#    #+#             */
/*   Updated: 2023/05/24 16:09:41 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Recup PATH dans mon env et le split avec : pour mon exec
/* This saves the PATH variable directories in separate strings
** MALLOC (split) need to be freed */
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