/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_datas.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serge <serge@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:14 by vloth             #+#    #+#             */
/*   Updated: 2023/05/25 08:26:12 by serge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**parse_readline(char *str);
char	**ft_split_words(char *str);
/*
void	init_data(t_data *data, char **en);
void 	init_data_cmd(t_data *data);
*/
//init les datas avant le lancement du shell
void	init_data(t_data *data, char **en)
{
	printTitle();
	data->envp = en;
	data->env = init_envp(en);
	global.pid = 42;
	signal_handler();
}

//init les datas dans la boucle
void init_data_cmd(t_data *data, char *str)
{
	data->path_dirs = ft_getpath(data->env); // ft_split(PATH, ':')
	data->cmdIndex = init_cmd(); // createts a node to store beginning/end of t_cmd list
	data->argv_readline = NULL;
	data->argv_readline = parse_readline(str);
	
	/* debug */
	int i = -1;
	while (data->argv_readline[++i])
		printf("'%s'\n", data->argv_readline[i]);
	/*********/
}

bool	empty_line(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}

char	**parse_readline(char *str)
{
	printf("\tempty_line: '%d'\n", empty_line(str));
	if (empty_line(str))
		return (NULL);
	return (ft_split_words(str));
}

char **ft_split_words(char *str)
{
	char	**spl;
	int		word = 0;
	int		i;
	int		j = 0;
	int		start = 0;

	i = -1;
	while (str[++i])
	{
		if ((ft_isspace(str[i + 1]) || str[i + 1] == '\n' || str[i + 1] == '\0')
			&& (!ft_isspace(str[i]) || str[i] != '\n'))
			word++; 
		/*
		if ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\0')
			&& (str[i - 1] != ' ' || str[i - 1] != '\t' || str[i - 1] != '\n'))
			word++; 
		*/
	}
	//printf("words:'%d'\ti:'%d'\n", word, i);
	spl = (char **)malloc(sizeof(char *) * (word + 1));
	if (!spl)
		return (NULL);
	if (!word)
	{
		spl[word] = (void *)0;
		return (spl);
	}
	i = 0;
	word = 0;
	while (str[++i])
	{
		//while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
		while (str[i] && (ft_isspace(str[i]) || str[i] == '\n'))
			i++;
		start = i;
		//while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
		while (str[i] && (!ft_isspace(str[i]) && str[i] != '\n'))
			i++;
		//printf("start: '%d' > i:'%d'\n", start, i);
		if (i > start)
		{
			spl[word] = (char *)malloc(sizeof(char) * (i - start + 1));
			if (!spl[word])
			{
				while (word >= 0)
					free(spl[word--]);
				free(spl);
				return (NULL);
			}
			j = -1;
			while (++j < (i - start))
			{
				spl[word][j] = str[start + j];
				//j++;
			}
			spl[word][j] = '\0';
			//printf("w:'%d'\ts:'%d'\t'%s'\n", word, start, &str[start]);
			word++;
		}
	}
	spl[word] = (void *)0;

	return (spl);
}