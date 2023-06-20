/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:10:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/20 13:08:20 by sbocanci         ###   ########.fr       */
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
to check how it works compile:
gcc test_tok.c ../libft/ft_putstr_fd.c ../libft/ft_strlen.c -lreadline


*/

typedef struct s_data_cmd
{
	char	**quoted_str;
}	t_data_cmd;

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
void	check_quotes(char *str)
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
				fprintf(stderr, "syntax error: unmatched quote\n");
				//global.last_status = 1;
				exit (1);
			}
		}
		i++;
	}
	//printf("\tcheck_q. len: [%d], i: [%d], quote_char: [%c]\n", len, i, quote_char);
}

static bool	malloc_for_quoted_str(t_data_cmd *data, int k, int len)
{
	if (!data->quoted_str)
		data->quoted_str = malloc(sizeof(char*) * 2);
	else
		data->quoted_str = realloc(data->quoted_str, sizeof(char*) * (k + 2));
	if (!data->quoted_str)
	{
		free_array(data->quoted_str);
		return (false);
		//global.last_status = 1;
	}
	data->quoted_str[k] = malloc(sizeof(char) * (len + 1));
	if (!data->quoted_str[k])
	{
		free_array(data->quoted_str);
		return (false);
	}
	return (true);
}

static bool	get_quoted_str(t_data_cmd *data, char *str, int *i)
{
	static int	num;
	int			quote_char;
	int			len;
	int			j;

	quote_char = str[*i];
	j = *i;
	len = 2;
	while (str[++j] && str[j] != quote_char)
		len++;
	if (!malloc_for_quoted_str(data, num, len))
		return (false);
	j = *i;
	len = 0;
	while (str[++j] && str[j] != quote_char)
		data->quoted_str[num][len++] = str[j - 1];
	data->quoted_str[num][len++] = str[j];
	data->quoted_str[num++][len] = '\0';
	*i = j;
	data->quoted_str[num] = (NULL);
	return (true);
}

void split_quoted(t_data_cmd *data, char* str) {
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			if (!get_quoted_str(data, str, &i))
				break ;
		}
	}
}

int	main()
{
	t_data_cmd	data;
	char		*line;
	int			i = 0;

	data.quoted_str = NULL;
	line = readline("input: ");
	
	check_quotes(line);
	split_quoted(&data, line);
	
	while (data.quoted_str && data.quoted_str[i])
	{
		printf("[%d]: [%s]\n", i, data.quoted_str[i]);
		i++;
	}
	free(line);
	free_array(data.quoted_str);
	
	return (0);
}
