#include "../minishell.h"

//void ft_pipex(t_cmdIndex *index, char **path_dirs, t_envSom *envp_nodes)

int	close_if(int fd)
{
	if (fd > 2)
		close(fd);
	return (1);
}

void	ft_close_fds(t_cmd *cmd)
{
	if (cmd->in_file != -1)
		close_if(cmd->in_file);
	if (cmd->out_file != -1)
		close_if(cmd->out_file);
	if (cmd->in_fd != -1)
		close_if(cmd->in_fd);
	if (cmd->out_fd != -1)
		close_if(cmd->out_fd);
}

void	ft_close_all_fds(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		ft_close_fds(cmd);
		cmd = cmd->next;
	}
}

void	ft_perror_clean_exit(t_data *data, char *str)
{
	perror(str);
	ft_close_all_fds(data);
	exit(-1);
}

/*
void	ft_set_redirection_fd(t_cmd *cmd)
{
	if (cmd->in_file > 2)
	{
		close_if(cmd->fd_in);
		cmd->fd_in = cmd->in_file;
	}
	if (cmd->out_file > 2)
	{
		close_if(cmd->fd_out);
		cmd->fd_out = cmd->out_file;
	}
}
void	ft_set_redirection_fd(t_cmd *cmd)
{
	//ft_open(cmd->lredir->begin);
	if (cmd->redir && cmd->in_file != -1)
	{
		close_if(cmd->fd_in);
		cmd->fd_in = cmd->in_file;
	}
	if (cmd->redir && cmd->out_file != -1)
	{
		close_if(cmd->fd_out);
		cmd->fd_out = cmd->out_file;
	}
}
void	ft_exec_command(t_cmd *cmd, t_data *data)
{
	if (cmd->redir)
		ft_set_redirection_fd(cmd);
	if (cmd->is_built)
	{
		ft_builtins(cmd, data->env, data);
		close_if(cmd->fd_in);
		close_if(cmd->fd_out);
		exit(data->exit_return);
	}
	else
	{
		if (dup2(cmd->fd_in, 0) == -1)
			ft_perror_clean_exit(data, "(fd_in) Dup2 failure in child.");
		close_if(cmd->fd_in);
		if (dup2(cmd->fd_out, 1) == -1)
			ft_perror_clean_exit(data, "(fd_out) Dup2 failure in child.");
		close_if(cmd->fd_out);
		ft_execve(cmd, data);
	}
	close_if(cmd->fd_in);
	close_if(cmd->fd_out);
	exit(127);
}

int	ft_pipe_and_fork(t_data * data, t_cmd *cmd, int pipe_fd[2], int prev_fd_in)
{
	int		pid;

	if (pipe(pipe_fd) == -1)
		ft_perror_clean_exit(data, "PIPE ERROR");
	pid = fork();
	if (pid == -1)
		ft_perror_clean_exit(data, "Fork failure");
	if (pid == 0)
	{
		close(pipe_fd[0]);
		cmd->fd_in = prev_fd_in;
		cmd->fd_out = pipe_fd[1];
		ft_exec_command(cmd, data);
	}
	else
	{
		close(pipe_fd[1]);
		close_if(prev_fd_in);
		prev_fd_in = pipe_fd[0];
	}
	return (prev_fd_in);
}
*/

void	ft_exec_command(t_cmd *cmd, t_data *data)
{
	if (cmd->is_built)
	{
		ft_builtins(cmd, data->env, data);
		exit(data->exit_return);
	}
	else
		ft_execve(cmd, data);
	exit(127);
}

int	get_cmd_in_fd(t_cmd *cmd)
{
	if (cmd->redir)
		return (cmd->in_file);
	return (cmd->prev_in_fd);
}

int	get_cmd_out_fd(t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->redir)
		return (cmd->out_file);
	return (pipe_fd[1]);
}

void	ft_fork(t_data *data, t_cmd *cmd, int pipe_fd[2])
{
	int	pid;

	pid = fork();
	if (pid == -1)
		ft_perror_clean_exit(data, "PIPE ERROR");
	if (pid == 0)
	{
		close_if(pipe_fd[0]);
		cmd->in_fd = get_cmd_in_fd(cmd);
		cmd->out_fd = get_cmd_out_fd(cmd, pipe_fd);
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			ft_perror_clean_exit(data, "(in_fd) Dup2 failure in child.");
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			ft_perror_clean_exit(data, "(out_fd) Dup2 failure in child.");
		close_if(cmd->in_fd);
		close_if(cmd->out_fd);
		ft_exec_command(cmd, data);
	}
	else 
	{
		cmd->prev_in_fd = pipe_fd[0];
		close_if(pipe_fd[1]);
	}
}

void	ft_execute_commands(t_data *data)
{
	t_cmd	*cmd;
	int		pipe_fd[2];

	cmd = data->cmdIndex->begin;
	cmd->prev_in_fd = STDIN_FILENO;
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	while (cmd)
	{
		if (data->cmdIndex->nb_pipe > 0)	
		{
			if (pipe(pipe_fd) == -1)
				ft_perror_clean_exit(data, "PIPE ERROR");
			ft_fork(data, cmd, pipe_fd);
		}
		else
		{
			ft_fork(data, cmd, pipe_fd);
		}
		cmd = cmd->next;
	}
	close_if(cmd->prev_in_fd);
	ft_wait_for_child_processes(data);
	//ft_close_all_fds(data);
}

/*
void	ft_multiple_pipes(t_data *data)
{
	t_cmd		*cmd;
	int		pipe_fd[2];
	int		prev_pipe_fd_in;
	int		pid;

	cmd = data->cmdIndex->begin;
	prev_pipe_fd_in = IN;
	while (cmd->next)
	{
		prev_pipe_fd_in = ft_pipe_and_fork(data, cmd, pipe_fd, prev_pipe_fd_in);
		cmd = cmd->next;
	}
	pid = fork();
	if (pid == -1)
		ft_perror_clean_exit(data, "Fork failure");
	if (pid == 0)
	{
		cmd->fd_in = prev_pipe_fd_in;
		ft_exec_command(cmd, data);
	}
	close_if(prev_pipe_fd_in);
	ft_wait_for_child_processes(data);
	//ft_close_all_fds(data);
}
*/
void	ft_wait_for_child_processes(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	//while (i < data->p_cmd.active_cmds)
	while (i < data->cmdIndex->nb_cmd)
	{
		if (wait(&status) == -1)
			ft_perror_clean_exit(data, "Wait Failed");
		if (WIFSIGNALED(status) && WTERMSIG(status) != 13)
		{
			ft_putstr_fd("\nChild was terminated by signal ", 2);
			ft_putnbr_fd(WTERMSIG(status), 2);
			ft_putchar_fd('\n', 2);
		}
		i++;
	}
}
