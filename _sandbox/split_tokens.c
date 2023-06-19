/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:11:17 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/17 20:11:20 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* This is used in init_data_cmd
** it splits the input line into words separated by any kind of whitespace
*/
	//printf("\tempty_line: '%d' (if true '1' then its empty or spaces)\n", empty_line(str));
	//if (empty_line(str))
	//	return (NULL);
char	**parse_readline(char *str)
{
	return (split_tokens(str));
}

static int	word_count(const char *str)
{
	int	i;
	int	word;

	word = 0;
	i = 0;
	while (str[i])
	{
		if (!space(str[i]) && (space(str[i + 1]) || str[i + 1] == '\0'))
			word++;
		i++;
	}
	return (word);
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
		free(spl);
		return (NULL);
	}
	return (spl[*word]);
}

static void ft_split_words(char **spl, char *str, int *words)
{
	size_t		i;
	size_t		j;
	size_t		start;
	
	i = 0;
	while (i < ft_strlen(str))
	{
		while (space(str[i]) && str[i])
			i++;
		start = i;
		while (!space(str[i]) && str[i])
			i++;
		if (i > start)
		{
			if ((spl[*words] = malloc_word(spl, i, start, words)) == NULL)
				break ;
			j = -1;
			while (++j < (i - start))
				spl[*words][j] = str[start + j];
			spl[*words][j] = '\0';
			*words += 1;
		}
		i++;
	}
}

char **split_tokens(char *str)
{
	char	**spl;
	int		words;
	
	words = 0;
	words = word_count(str);

	spl = (char **)malloc(sizeof(char *) * (words + 1));
	if (!spl)
		return (NULL);
	if (words > 0)
	{
		words = 0;
		ft_split_words(spl, str, &words);
	}
	spl[words] = (void *)0;
	return (spl);
}

	/* debug */
	//printf("\t\tspl: words:'%d'\tstr len(i):'%ld'\n", words, ft_strlen(str));
	/* ***** */