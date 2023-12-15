/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:06:45 by galambey          #+#    #+#             */
/*   Updated: 2023/12/15 16:19:35 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
If error malloc in a builtin return 255
If return 0 > no builtin
If return 1 > builtin exec ok
*/
int		ft_search_builtin(t_msh *msh)
{
	if (ft_strcmp(msh->p.cmd_opt[0], "echo") == 0)
		return (builtin_echo(msh), 1);
	else if (ft_strcmp(msh->p.cmd_opt[0], "cd") == 0)
	{
		builtin_cd(msh);
		if (status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_opt[0], "pwd") == 0)
		return (builtin_pwd(msh), 1);
	else if (ft_strcmp(msh->p.cmd_opt[0], "export") == 0)
	{
		builtin_export(msh);
		if (status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_opt[0], "unset") == 0)
		return (builtin_unset(msh), 1);
	else if (ft_strcmp(msh->p.cmd_opt[0], "env") == 0)
	{
		builtin_env(msh);
		if (status == 255)
			return (255);
		return (1);
	}
	// else if (ft_strcmp(msh->p.cmd_opt[0], "exit") == 0)
	// {
	// 	ft_echo(msh);
	// 	return (1);
	// }
	return (0);
}

void	ft_child_exec(t_msh *msh)
{
	int	err;
	char **env;

	err = ft_access_cmd(msh->p.path, msh->p.cmd_opt[0], &msh->p.good_path);
	if (err > 0) // OK PROTEGER
		(ft_perr(err, msh->p.cmd_opt[0]), ft_exit(-1, -1, -1));
	env = ft_transcript_env(msh->env, msh->p.good_path);
	if (err > 0) // OK PROTEGER
		(ft_perr(err, msh->p.cmd_opt[0]), ft_exit(-1, -1, -1));
	execve(msh->p.good_path, msh->p.cmd_opt, env);
	(perror("execve"), ft_exit(-1, -1, -1)); //implementer F_EXIT);
}

//TO DO : free tous les elemnts jusqu au pipe ou prochain operateur
void	ft_parent(t_msh *msh, int fd_1, int fd_2, int rule)
{
	t_split *head;
	t_env *head_hd;
	t_env *prev_hd;
	
	if (fd_1 > -1)
		close(fd_1);
	if (fd_2 > -1)
		close(fd_2);
	head = msh->av;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR /* && msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE */)
	{
		if (msh->av->token == HERE_DOC && rule != CMD_ALONE)
		{
			head_hd = msh->p.here_doc;
			prev_hd = NULL;
			while (msh->p.here_doc && (ft_strcmp(msh->p.here_doc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.here_doc->name, msh->av->data) == 0 && msh->p.here_doc->read == 1)))
			{
				prev_hd = msh->p.here_doc;
				msh->p.here_doc = msh->p.here_doc->next;
			}
			if (msh->p.here_doc)
				msh->p.here_doc->read = 1;
			msh->p.here_doc = head_hd;
		}
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
	}
	if (msh->av && (msh->av->token == PIPE || msh->av->token == PAR_CLOSE))
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
}
