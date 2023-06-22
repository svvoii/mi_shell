/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_quoted_string.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:11:17 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/22 19:18:33 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
This code expands the string with `$` sign if it is double quoted or not quoted replacing the envp $KEY with its VALUE.
If the quotes are right after the `$` sign it is not expanded.

example of input line:
input: 
```echo $USER Hello ". $USER  .."  $NOT_IN_ENVP '  follow the $USER..  ' THEN..$'USER' .. $"USER"```
bash output: 
```user_name Hello . user_name  ..   follow the $USER..   THEN..USER .. USER```

input:
echo $USER Hello ". $USER .."     $NOT_IN_ENV '. ..follow the $USER .' THEN..$'USER' .. $"USER"$
bash:
sbocanci Hello . sbocanci .. . ..follow the $USER . THEN..USER .. USER$
mi_shel:
sbocanci Hello . sbocanci .. . ..follow the $USER . THEN..USER .. USER$

*/

char	*ft_strncpy(char *destination, const char *source, size_t num)
{
	size_t	i;

	i = 0;
	while (i < num && source[i])
	{
		destination[i] = source[i];
		i++;
	}
	return (destination);
}

void	quoted_state(int *is_double_q, int *is_single_q, char q, int *i)
{
	if (q == '\"')
		*is_double_q = !*is_double_q;
	else if (q == '\'')
		*is_single_q= !*is_single_q;
	*i += 1;
}

int	look_up_key_value(char *key, char *value, char **m_envp)
{
	int j;
	int	key_len;
	int	value_len;

	key_len = ft_strlen(key);
	value_len = 0;
	j = 0;
	while (m_envp[j])
	{
		if (ft_strncmp(key, m_envp[j], key_len) == 0 && m_envp[j][key_len] == '=')
		{
			value_len = ft_strlen(m_envp[j]) - key_len - 1;
			//ft_strlcpy(value, m_envp[j] + key_len + 1, value_len);
			ft_strncpy(value, m_envp[j] + key_len + 1, value_len);
			value[value_len] = '\0';
			return (value_len);
			break;
		}
		value[0] = '\0';
		j++;
	}
	return (0);
}

int	get_key_and_look_up_value(t_cmd *cmd, int j, int *output_len, char **m_envp)
{
	int		key_len;
	int		value_len;
	char	key[BUFFER_SIZE_MAX];
	char	value[BUFFER_SIZE_MAX];

	key_len = 0;
	value_len = 0;
	j++;
	while (ft_isalnum(cmd->cmd[j]) || cmd->cmd[j] == '_')
	{
		key[key_len++] = cmd->cmd[j++];
	}
	key[key_len] = '\0';
	//printf("\tkey: [%s], len: [%d]\t", key, key_len);

	value_len = look_up_key_value(key, value, m_envp);

	//printf("\tvalue: [%s], len: [%d]\n", value, value_len);

	//ft_strcat(cmd->output, value);
	ft_strcat(cmd->output, value);
	*output_len += value_len;
	return (key_len);
}

void	expand_envp(t_cmd *cmd, char **m_envp)
{
	int i;
	int output_len = 0;
	int is_double_quoted = 0;
	int is_single_quoted = 0;

	i = 0;
	while (cmd->cmd[i])
	{
		//printf("[%d]: [%c]\n", i, cmd->cmd[i]);
		if (cmd->cmd[i] == '\'' || cmd->cmd[i] == '\"')
			quoted_state(&is_double_quoted, &is_single_quoted, cmd->cmd[i], &i);
		if (cmd->cmd[i] == '$' && !is_single_quoted && (!space(cmd->cmd[i + 1]) && cmd->cmd[i + 1] != '\0'))
		{
			i += get_key_and_look_up_value(cmd, i, &output_len, m_envp);
		}
		else
			cmd->output[output_len++] = cmd->cmd[i];
		i++;
	}
}

void	replace_envp_key_with_value(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		ft_memset(cmd->output, 0, BUFFER_SIZE_MAX);
		expand_envp(cmd, data->m_envp);
		cmd = cmd->next;
	}
}
