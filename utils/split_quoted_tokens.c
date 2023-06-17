/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quoted_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:10:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/17 20:11:02 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* This is used in init_data_cmd
** it splits the input line into words separated by any kind of whitespace
*/
	//printf("\tempty_line: '%d' (if true '1' then its empty or spaces)\n", empty_line(str));
	//if (empty_line(str))
	//	return (NULL);
void	ft_parse_readline(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		cmd->tok = split_quoted_tokens(cmd->cmd);
		cmd = cmd->next;
	}
}

static bool is_quote(const char c)
{
    return (c == '\'' || c == '\"');
}

/* ..in utils_nd.c
static bool is_space(const char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\r' || c == '\f');
}
*/
static int word_count(const char *str)
{
    int i;
    int word;
    bool within_quotes;

    word = 0;
    i = 0;
    within_quotes = false;
    while (str[i])
    {
        if (!is_quote(str[i]))
        {
            if (!space(str[i]) && (space(str[i + 1]) || str[i + 1] == '\0'))
                word++;
        }
        else
        {
            if (within_quotes)
                within_quotes = false;
            else
                within_quotes = true;
        }
        i++;
    }
    return word;
}

static char *malloc_word(char **spl, int i, int start, int *word)
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
        return NULL;
    }
    return spl[*word];
}

static void ft_split_words(char **spl, char *str, int *words)
{
    size_t i = 0;
    size_t j;
    size_t start;
    bool within_quotes = false;

    while (i < strlen(str))
    {
        if (!is_quote(str[i]))
        {
            while (space(str[i]) && str[i])
                i++;
            start = i;
            while (!space(str[i]) && str[i])
                i++;
            if (i > start)
            {
                if ((spl[*words] = malloc_word(spl, i, start, words)) == NULL)
                    break;
                j = 0;
                while (start < i)
                {
                    if (is_quote(str[start]))
                    {
                        within_quotes = !within_quotes;
                        start++;
                        continue;
                    }
                    spl[*words][j++] = str[start++];
                }
                spl[*words][j] = '\0';
                *words += 1;
            }
        }
        else
        {
            within_quotes = !within_quotes;
            i++;
        }
    }
}

char **split_quoted_tokens(char *str)
{
    char **spl;
    int words;

    words = word_count(str);

    spl = (char **)malloc(sizeof(char *) * (words + 1));
    if (!spl)
        return NULL;
    if (words > 0)
    {
        words = 0;
        ft_split_words(spl, str, &words);
    }
    spl[words] = NULL;
    return spl;
}
