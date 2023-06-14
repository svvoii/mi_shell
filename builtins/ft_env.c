	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloth <vloth@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:26:25 by vloth             #+#    #+#             */
/*   Updated: 2023/05/13 18:26:26 by vloth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->m_envp[i])
	{
		ft_putendl_fd(data->m_envp[i], 1);
		i++;
	}
	data->exit_return = 0; 
	return (0);
}

// regarde si dans notre env il y a la variable oldpwd
int have_olpwd(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "OLDPWD=", 7) == 0)
			return (1);
		i++;
	}
	return (0);
}
