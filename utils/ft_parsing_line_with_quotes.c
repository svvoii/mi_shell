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

#include "../minishell.h"

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
*/

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

static void ft_non_quoted_str(char **spl, char *str, size_t *index, int *words)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = *index;
	while (space(str[i]) && str[i])
		i++;
	start = i;
	while (!space(str[i]) && str[i] && !is_quote(str[i]))
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
static void ft_split_words(t_cmd *cmd, char **spl, int *words)
{
	char 		*str;
	size_t		i;
	size_t		q;
	size_t		quoted_len;
	
	str = cmd->cmd;
	i = 0;
	q = 0;
	while (i < ft_strlen(str))
	{
		if (is_quote(str[i]))
		{
			quoted_len = ft_strlen(cmd->quoted_str[q]);
			if (ft_strncmp(str + i, cmd->quoted_str[q], quoted_len) == 0)
			{
				/* DEBUG */
				//printf("\t\tstrncmp\n\t\t\t[%s]\n\t\t\t[%s]\n", str + i, cmd->quoted_str[q]);
				/* *** */
				spl[*words] = ft_strdup(cmd->quoted_str[q]);
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

static void split_line(t_cmd *cmd)
{
	char	*spl[BUFFER_SIZE_MAX];
	int		words;
	
	words = 0;
	ft_split_words(cmd, spl, &words);

	cmd->arguments = (char **)malloc(sizeof(char *) * (words + 1));
	if (!cmd->arguments)
	{
		fprintf(stderr, "malloc error\n");
		return ;
	}
	cmd->arguments[words] = NULL;
	while (--words >= 0)
	{
		cmd->arguments[words] = ft_strdup(spl[words]);
		free(spl[words]);
	}
}

void	parse_arguments_with_quotes(t_data *data)
{
	t_cmd		*cmd;
	
	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		split_line(cmd);
		cmd = cmd->next;
	}
}
