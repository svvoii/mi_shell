/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_nd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:24:01 by vloth             #+#    #+#             */
/*   Updated: 2023/06/22 16:24:52 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redir_or_cmd(char c)
{
    if (c == '<' || c == '>')
        return 1;
    return 0;
}

bool	is_quote(const char c)
{
	return (c == '"' || c == '\'');
}

/* maybe to add '\0' as well for cases of single $ in the end of the string */
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
