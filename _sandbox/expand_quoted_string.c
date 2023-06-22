/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quoted_string.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:11:17 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/22 12:04:36 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

/*
This code expands the string with `$` sign if it is double quoted or not quoted and replace the envp $KEY with its VALUE.
If the quotes are right after the `$` sign it is not expanded.

example of input line:
input: 
```echo $USER Hello ". $USER  .."  $NOT_IN_ENVP '  follow the $USER..  ' THEN..$'USER' .. $"USER"```
bash output: 
```user_name Hello . user_name  ..   follow the $USER..   THEN..USER .. USER```

The code check if there are any $ signs and replace the `$KEY` to `VALUE` from `**m_envp` if it is not singlequoted.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct s_cmd_data
{
	char	input[2048];
	char	output[2048];
}	t_cmd_data;

void	expand_envp(t_cmd_data *cmd, const char **m_envp);

int main(int argc, const char **argv, const char **envp)
{
	t_cmd_data	cmd;

	memset(cmd.input, 0, 2048);
	memset(cmd.output, 0, 2048);

	printf("input: ");
	fgets(cmd.input, 1000, stdin);

	expand_envp(&cmd, envp);

	printf("Output: %s\n", cmd.output);
}

void	quoted_state(int *is_double_q, int *is_single_q, char q, int *i)
{
	if (q == '\"')
		*is_double_q = !*is_double_q;
	else if (q == '\'')
		*is_single_q= !*is_single_q;
	*i += 1;
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	look_up_key_value(char *key, char *value, const char **m_envp)
{
	int j;
	int	key_len;
	int	value_len;

	key_len = ft_strlen(key);
	value_len = 0;
	j = 0;
	while (m_envp[j])
	{
		if (strncmp(key, m_envp[j], key_len) == 0 && m_envp[j][key_len] == '=')
		{
			value_len = ft_strlen(m_envp[j]) - key_len - 1;
			strncpy(value, m_envp[j] + key_len + 1, value_len);
			value[value_len] = '\0';
			return (value_len);
			break;
		}
		value[0] = '\0';
		j++;
	}
}

int	get_key_and_look_up_value(t_cmd_data *cmd, int j, int *output_len, const char **m_envp)
{
	int key_len;
	int value_len;
	char key[2048];
	char value[2048];

	key_len = 0;
	value_len = 0;
	j++;
	while (isalnum(cmd->input[j]) || cmd->input[j] == '_')
	{
		key[key_len++] = cmd->input[j++];
	}
	key[key_len] = '\0';
	//printf("\tkey: [%s], len: [%d]\t", key, key_len);

	value_len = look_up_key_value(key, value, m_envp);

	//printf("\tvalue: [%s], len: [%d]\n", value, value_len);

	strcat(cmd->output, value);
	*output_len += value_len;
	return (key_len);
}

void	expand_envp(t_cmd_data *cmd, const char **m_envp)
{
	int i;
	int output_len = 0;
	int is_double_quoted = 0;
	int is_single_quoted = 0;

	i = 0;
	while (cmd->input[i])
	{
		//printf("[%d]: [%c]\n", i, cmd->input[i]);
		if (cmd->input[i] == '\'' || cmd->input[i] == '\"')
			quoted_state(&is_double_quoted, &is_single_quoted, cmd->input[i], &i);
		if (cmd->input[i] == '$' && !is_single_quoted)
		{
			i += get_key_and_look_up_value(cmd, i, &output_len, m_envp);
		}
		else
		{
            cmd->output[output_len++] = cmd->input[i];
		}
		i++;
	}
}

/*
void	expand_envp(char *input, char *output, const char **envp)
{
	int i;
	int j;
	char key[1000];
	char value[1000];
	int key_len = 0;
	int output_len = 0;
	int is_double_quoted = 0;
	int is_single_quoted = 0;

	i = 0;
	while (input[i])
	{
		key_len = 0;
		if (input[i] == '\'' || input[i] == '\"')
			quoted_state(&is_double_quoted, &is_single_quoted, input[i], &i);
		if (input[i] == '$' && !is_single_quoted)
		{
			j = i + 1;
			while (isalnum(input[j]) || input[j] == '_')
				key[key_len++] = input[j++];
			key[key_len] = '\0';
			//printf("\tkey: [%s], len: [%d]\n", key, key_len);
			look_up_key_value(key, value, envp);
			//printf("\tvalue: [%s], len: [%ld]\n", value, ft_strlen(value));
			strcat(output, value);
            output_len += ft_strlen(value);
			i += key_len;
		}
		else
            output[output_len++] = input[i];
		i++;
	}
}
*/