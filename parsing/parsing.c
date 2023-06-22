#include "../minishell.h"

/* Need to look into all instances of just_cmd and optimize the use of parsed data */
void cut_arg(t_data *data)
{
	t_cmd *cmd;

	cmd = data->cmdIndex->begin;
	while (cmd)
	{
		//cmd->argv = ft_split(cmd->output, ' ');
		if (cmd->is_built == false)
		{
			if (cmd->redir)
				cmd->argv = ft_split(cmd->just_cmd, ' ');
			else
				cmd->argv = ft_split(cmd->output, ' ');
		}
		/*
		*/
		cmd = cmd->next;
	}
}

/*
void cut_arg(t_data *data)
{
    t_cmd *cmd;
    
    cmd = data->cmdIndex->begin;
    while (cmd)
    {
        if (cmd->is_built == false)
        {
            if (cmd->redir)
                cmd->argv = ft_split(cmd->just_cmd, ' ');
            else
                cmd->argv = ft_split(cmd->cmd, ' ');
        }
        cmd = cmd->next;
    }
}
*/