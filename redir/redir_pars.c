/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:24:37 by vloth             #+#    #+#             */
/*   Updated: 2023/06/09 12:06:41 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//malloc les redirections pour toute les cmd
/*
	I have added file which suppose to handle heredoc. We need to understand when to create the heredoc file..
	

*/
void	malloc_all(t_data *data)
{
	redirOrNot(data->cmdIndex);
	initRedirOrnot(data->cmdIndex);
	malloc_redir(data);
    exec_find_cmd(data);
	//printf("OK so far\n");
    cut_arg(data);
    is_built(data);
	/* maybe we can process heredoc here to get its fd before (boucle_redir).. see heredoc.c */
	// ft_heredoc();
    boucle_redir(data);
    is_meta(data);
}

//Open des file parser plus haut, dans lexec si fd = -1 exit le process
void    ft_open(t_redir *red)
{
    if (red->type == APPEND)
        red->fd = open(red->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    else if (red->type == R_OUT)
        red->fd = open(red->file,  O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (red->type == R_IN)
        red->fd = open(red->file, O_RDONLY, 0644);
    else if (red->type == HERD)
	{
		ft_create_here_doc(red->file);
        red->fd = open(HERE_DOC_FILE, O_RDONLY, 0644);
	}
	/* adding error handler: (bash: 'red->lile': No such file or directory) 
	** the program should return to the eternal_loop !! */
	if (red->fd == -1 && red->type != HERD)
	{
		global.last_status = errno;
		mini_error_file(red->file);
	}
}

//decoupe les fichier de redirection et open les file en en mettant les fd dans ls struct
void    begin_end_file(int i, t_redir *red, char *str)
{
    red->begin = i;
    while (str[i] && str[i] != '>' && str[i] != '<' && str[i] != ' ')
        i++;
    red->len = i - red->begin;
    red->file = ft_substr(str, red->begin, red->len);
    ft_open(red);    
}

void    redir_fd(t_cmd *cmd)
{
    t_redir *redir;

    cmd->in_file = -2;
    cmd->out_file = -2;
    if (cmd->redir)
    {
        redir = cmd->lredir->begin;
        while (redir)
        {
            if (redir->type == R_IN || redir->type == HERD)
            {
                if (cmd->in_file >= 0)
                    close(cmd->in_file);
                cmd->in_file = redir->fd;
            }
            if (redir->type == R_OUT || redir->type == APPEND)
            {
                if (cmd->out_file >= 0)
                    close(cmd->out_file);
                cmd->out_file = redir->fd;
            }
            redir = redir->next;
        }
    }
}

void boucle_redir(t_data *data)
{
    t_cmd *cmd;

    cmd = data->cmdIndex->begin;
    while (cmd)
    {
		redir_fd(cmd);
        cmd = cmd->next;
    }
}
/*
void    redir_fd(t_cmd *cmd)
{
    t_redir *redir;

    cmd->in_file = -2;
    cmd->out_file = -2;
    if (cmd->redir)
    {
        redir = cmd->lredir->begin;
        while (redir)
        {
            if (redir->type == R_IN)
            {
                if (cmd->in_file >= 0)
                    close(cmd->in_file);
                cmd->in_file = redir->fd;
            }
            if (redir->type == R_OUT || redir->type == APPEND)
            {
                if (cmd->out_file >= 0)
                    close(cmd->out_file);
                cmd->out_file = redir->fd;
            }
            redir = redir->next;
        }
    }
}
*/