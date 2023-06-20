/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_with_quotes.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:10:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/20 13:08:43by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
/*

In here we extracts the separate words divided by space and additional condition if initial string line contains pair of quotes then 
it shall be saved as entire string including the quotes! 
For example if this is the input of the read_line:

```echo " Hello $USER  "  $FIRST '  follow the white rabbit..  ' THEN.. ```

this is how it is saved in char **arguments:
```
[0]: [echo]
[1]: [" Hello $USER  "]
[2]: [$FIRST]
[3]: ['  follwo the white rabbit..  ']
[4]: [THEN..]
[5]: Null;
```
We also extracted quoted parts of the line string and they are stored in 
char **quoted_str :
```
[0]: [" Hello $USER  "]
[1]: ['  follwo the white rabbit..  ']
[2]: Null;
```
The next step would be to expand the $  if it is present within any of the strings in **arguments 

this file can be compiled as it is with:
gcc parsing_line_with_quotes.c -lreadline

*/
typedef struct s_data_cmd
{
	char	**quoted_str;
	char	**arguments;
}	t_data_cmd;

#define BUFFER 64

static void	check_quotes(char *str);
void		split_line(t_data_cmd *cmd_data, char *str);
void		split_quoted(t_data_cmd *data, char* str);
void		free_array(char **array);

/* LIBFT */
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);


int	main()
{
	t_data_cmd	cmd_data;
	char		*line;
	int			i = 0;

	cmd_data.quoted_str = NULL;
	cmd_data.arguments = NULL;
	line = readline("input: ");
	
	check_quotes(line);
	split_quoted(&cmd_data, line);

	printf("\n extract quoted str:\n");
	while (cmd_data.quoted_str && cmd_data.quoted_str[i])
	{
		printf("[%d]: [%s]\n", i, cmd_data.quoted_str[i]);
		i++;
	}

	split_line(&cmd_data, line);

	printf("\n split_line:\n");
	i = 0;
	while (cmd_data.arguments && cmd_data.arguments[i])
	{
		printf("[%d]: [%s]\n", i, cmd_data.arguments[i]);
		i++;
	}
	free(line);
	free_array(cmd_data.quoted_str);
	free_array(cmd_data.arguments);
	
	return (0);
}

static char	*malloc_word(char **spl, int i, int start, int *word)
{
	spl[*word] = (char *)malloc(sizeof(char) * (i - start + 1));
	if (!spl[*word])
	{
		while (*word >= 0)
		{
			free(spl[*word]);
			*word -= 1;
		}
		return (NULL);
	}
	return (spl[*word]);
}

static bool	is_quote(const char c)
{
	return (c == '"' || c == '\'');
}

static bool	is_space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\r' || c == '\f');
}


static void ft_non_quoted_str(char **spl, char *str, size_t *index, int *words)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = *index;
	while (is_space(str[i]) && str[i])
		i++;
	start = i;
	while (!is_space(str[i]) && str[i] && !is_quote(str[i]))
		i++;
	if (i > start)
	{
		if ((spl[*words] = malloc_word(spl, i, start, words)) == NULL)
			return ;
		j = -1;
		while (++j < (i - start))
			spl[*words][j] = str[start + j];
		spl[*words][j] = '\0';
		*words += 1;
		*index = i;
	}
}

/*
Here iterating through the str each character we want to compare the str + i with quoted_str[] 
ft_strncmp(str + i, quoted_str[..], ft_strlen(quoted_str[..]))
and if its a match we save its pointer to cmd_data->arguments 
and advance i by ft_strlen(quoted_str[..])
*/
static void ft_split_words(t_data_cmd *cmd_data, char **spl, char *str, int *words)
{
	size_t		i;
	size_t		q;
	size_t		quoted_len;
	
	i = 0;
	q = 0;
	while (i < ft_strlen(str))
	{
		if (is_quote(str[i]))
		{
			quoted_len = ft_strlen(cmd_data->quoted_str[q]);
			if (ft_strncmp(str + i, cmd_data->quoted_str[q], quoted_len) == 0)
			{
				/* DEBUG */
				//printf("\t\tstrncmp\n\t\t\t[%s]\n\t\t\t[%s]\n", str + i, cmd_data->quoted_str[q]);
				/* *** */

				spl[*words] = ft_strdup(cmd_data->quoted_str[q]);
				*words += 1;
				q++;
				i += (quoted_len - 1);
			}
		}
		else
			ft_non_quoted_str(spl, str, &i, words);
		i++;
	}
}

void split_line(t_data_cmd *cmd_data, char *str)
{
	char	*spl[BUFFER];
	int		words;
	
	words = 0;
	ft_split_words(cmd_data, spl, str, &words);

	cmd_data->arguments = (char **)malloc(sizeof(char *) * (words + 1));
	if (!cmd_data->arguments)
	{
		fprintf(stderr, "malloc error\n");
		return ;
	}
	cmd_data->arguments[words] = NULL;
	while (--words >= 0)
	{
		cmd_data->arguments[words] = ft_strdup(spl[words]);
		free(spl[words]);
	}
}

/* QUOTED STRING */
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
static void	check_quotes(char *str)
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
	data->quoted_str[num][len++] = str[j - 1];
	data->quoted_str[num][len++] = str[j];
	data->quoted_str[num++][len] = '\0';
	*i = j;
	data->quoted_str[num] = (NULL);
	return (true);
}

void split_quoted(t_data_cmd *data, char* str)
{
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

/* LIBFT */

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*one;
	unsigned char	*two;
	size_t			i;

	one = (unsigned char *)s1;
	two = (unsigned char *)s2;
	i = 0;
	while ((i < n && one[i]) || (i < n && two[i]))
	{
		if (one[i] > two[i] || one[i] < two[i])
			return (one[i] - two[i]);
		i++;
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	size_t		i;
	size_t		s_len;
	char		*dup;

	s_len = ft_strlen((char *)s);
	dup = malloc(s_len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
