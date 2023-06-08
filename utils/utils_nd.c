/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_nd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:24:01 by vloth             #+#    #+#             */
/*   Updated: 2023/06/08 09:40:52 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redir_or_cmd(char c)
{
    if (c == '<' || c == '>')
        return 1;
    return 0;
}

bool	space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\r' || c == '\f');
}

bool	empty_line(char *str)
{
	while (*str)
	{
		if (!space(*str))
			return (false);
		str++;
	}
	return (true);
}
