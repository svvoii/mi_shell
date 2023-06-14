/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_datas.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:25:14 by vloth             #+#    #+#             */
/*   Updated: 2023/06/07 21:24:49              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**parse_readline(char *str);
char	**split_tokens(char *str);
/*
void	init_data(t_data *data, char **en);
void 	init_data_cmd(t_data *data);
*/
void	print_2d_array(char	**array)
{
	int	i = 0;

	while (array[i])
		printf("'%s'\n", array[i++]);
}
//init les datas avant le lancement du shell
void	init_data(t_data *data, char **en)
{
	//printTitle();
	/* strdup en into envp.. ?! */
	data->envp = en;
	init_envp(data);
	change_pwd(data->m_envp, "PWD=");
	//print_2d_array(data->envp);
	//data->env = init_envp(en);
	global.pid = 42;
	global.signal = 0;
	global.last_status = 0;
}

//init les datas dans la boucle
/* Added parse_readline which splits the str into separate tokens/strings 
** based on any white space. See 'space' in utils_nd.c */
//void init_data_cmd(t_data *data, char *str)
void init_data_cmd(t_data *data)
{
	data->path_dirs = ft_getpath(data->m_envp); // ft_split(PATH, ':')

	data->cmdIndex = init_cmd(); // createts a node to store beginning/end of t_cmd list
	data->argv_readline = NULL;

	//data->argv_readline = parse_readline(str);
	
	/* DEBUG */ 
	/*
	int i = 0;
	printf("\tsplit_tokens: ");
	while (data->argv_readline[i]) {
		printf("[%s], ", data->argv_readline[i]);
		i++;
	}
	printf("\n");
	i = 0;
	printf("\tpath_dirs: ");
	while (data->path_dirs[i]) {
		printf("\t\t'%s'\n", data->path_dirs[i]);
		i++;
	}
	printf("\n");
	********/
}
