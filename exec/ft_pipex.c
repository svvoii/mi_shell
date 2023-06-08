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

void	ft_exec_command(t_cmd *cmd, t_data *data)
{
	if (cmd->is_built)
	{
		ft_builtins(cmd, data->env, data);
		exit(data->exit_return);
	}
	else
	{
		ft_execve(cmd, data);
	}
}

int	get_cmd_in_fd(t_cmd *cmd)
{
	if (cmd->redir && cmd->in_file > 0)
		return (cmd->in_file);
	return (cmd->prev_in_fd);
}

int	get_cmd_out_fd(t_cmd *cmd, int pipe_fd_1)
{
	if (cmd->redir && cmd->out_file > 0)
		return (cmd->out_file);
	else if (cmd->next)
		return (pipe_fd_1);
	return (STDOUT_FILENO);
}

void	ft_fork(t_data *data, t_cmd *cmd, int pipe_fd[2])
{
	//int	pid;

	global.pid = fork();
	if (global.pid == -1)
		ft_perror_clean_exit(data, "PIPE ERROR");
	if (global.pid == 0)
	{
		printf("child: OK so far, pid:'%d'\n", global.pid);
		close_if(pipe_fd[0]);
		cmd->in_fd = get_cmd_in_fd(cmd);
		cmd->out_fd = get_cmd_out_fd(cmd, pipe_fd[1]);

		//printf("\tfork, cmd:'%s', in_fd:'%d', out_fd:'%d', prev_fd_in:'%d'\n", cmd->cmd, cmd->in_fd, cmd->out_fd, cmd->prev_in_fd);

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
		if (pipe_fd[0] != -1)
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
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	while (cmd)
	{
		//if (data->cmdIndex->nb_pipe > 0)	
		if (cmd->next)	
		{
			if (pipe(pipe_fd) == -1)
			{
				printf("error pipe\n");
				ft_perror_clean_exit(data, "PIPE ERROR");
			}
		}
		ft_fork(data, cmd, pipe_fd);
		cmd = cmd->next;
	}
	printf("\texecute_command: prev_fd: '%d'\n", cmd->prev_in_fd);
	close_if(cmd->prev_in_fd);
	ft_wait_for_child_processes(data);
	//ft_close_all_fds(data);
}

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

/*
*/