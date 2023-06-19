/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quoted_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:10:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/19 18:56:36 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*

The following code extracts the string within quotes. 
For example if this is the input of the read_line:

```echo " Hello $USER  " '  follwo the white rabbit..  '```

this should be the output of the printf function bellow:
```
[0]: [" Hello $USER  "]
[1]: ['  follwo the white rabbit..  ']
```
*/

void	free_array(char **array)
{
	int	i = 0;

	if (array)
	{
		while (array[i])
		{
			free(array[i]);
			i++;
		}
		free(array);
	}
}

/* This will check if the quotes in a string are matched */
static bool	check_matching_quotes(char *str)
{
	int	quote_char;
	int	len;
	int	i;

	len = strlen(str);
	quote_char = 0;
	i = 0;
	while (i < len)
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote_char = str[i++];
			while (i < len && str[i] != quote_char)
				i++;
			if (i == len)
			{
				//fprintf(stderr, "syntax error: unmatched quote\n");
				ft_putstr_fd("syntax error: unmatched quote\n", 2);
				global.last_status = 1;
				return (false);
				//exit (1);
			}
		}
		i++;
	}
	return (true);
	//printf("\tcheck_q. len: [%d], i: [%d], quote_char: [%c]\n", len, i, quote_char);
}

static bool	malloc_for_quoted_str(t_cmd *cmd, int k, int len)
{
	if (!cmd->quoted_str)
		cmd->quoted_str = malloc(sizeof(char*) * 2);
	else
		cmd->quoted_str = realloc(cmd->quoted_str, sizeof(char*) * (k + 2));
	if (!cmd->quoted_str)
	{
		free_array(cmd->quoted_str);
		global.last_status = 1;
		return (false);
	}
	cmd->quoted_str[k] = malloc(sizeof(char) * (len + 1));
	if (!cmd->quoted_str[k])
	{
		free_array(cmd->quoted_str);
		global.last_status = 1;
		return (false);
	}
	return (true);
}

static int	number_of_quoted_str(char **quoted_str)
{
	int	i;

	i = 0;
	while (quoted_str && quoted_str[i])
		i++;
	return (i);
}

static bool	get_quoted_str(t_cmd *cmd, char *str, int *i)
{
	int	num;
	int	quote_char;
	int	len;
	int	j;

	num = number_of_quoted_str(cmd->quoted_str);
	quote_char = str[*i];
	j = *i;
	len = 2;
	while (str[++j] && str[j] != quote_char)
		len++;
	if (!malloc_for_quoted_str(cmd, num, len))
		return (false);
	j = *i;
	len = 0;
	while (str[++j] && str[j] != quote_char)
		cmd->quoted_str[num][len++] = str[j - 1];
	cmd->quoted_str[num][len++] = str[j - 1];
	cmd->quoted_str[num][len++] = str[j];
	cmd->quoted_str[num++][len] = '\0';
	*i = j;
	cmd->quoted_str[num] = (NULL);
	return (true);
}

void	extract_quoted_str(t_data *data)
{
	t_cmd		*cmd;
	char		*str;
	int			i;
	
	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		cmd->quoted_str = NULL;
		str = cmd->cmd;
		i = -1;
		if (!check_matching_quotes(str))
			return ;
		while (str[++i])
		{
			if (str[i] == '"' || str[i] == '\'')
			{
				if (!get_quoted_str(cmd, str, &i))
					break ;
			}
		}
		cmd = cmd->next;
	}

	/* DEBUG 
	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		i = 0;
		while (cmd->quoted_str && cmd->quoted_str[i])
		{
			printf("[%d]: [%s]\n", i, cmd->quoted_str[i]);
			i++;
		}
		free_array(cmd->quoted_str);
		cmd = cmd->next;
	}
	*/
	/* ***** */
}
