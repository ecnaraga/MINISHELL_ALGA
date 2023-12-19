/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:06:45 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 11:58:51 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		ft_search_builtin_bis(t_msh *msh)
{
	int r_exit;
	
	if (ft_strcmp(msh->p.cmd_opt[0], "unset") == 0)
		return (builtin_unset(msh), 1);
	else if (ft_strcmp(msh->p.cmd_opt[0], "env") == 0)
	{
		builtin_env(msh);
		if (status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_opt[0], "exit") == 0)
	{
		r_exit = builtin_exit(msh);
		if (r_exit == 255)
			return (255);
		if (r_exit == 1)
			return (2);
		return (1);
	}
	return (0);
}

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
	return (ft_search_builtin_bis(msh));
}

void	ft_child_exec(t_msh *msh)
{
	int	err;
	char **env;
	(void) env;

	err = ft_access_cmd(msh->p.path, msh->p.cmd_opt[0], &msh->p.good_path);
	if (status == 255)// OK PROTEGER
		ft_exit(-1, -1, -1);
	if (err > 0) // OK PROTEGER
		(ft_perr(err, msh->p.cmd_opt[0]), ft_exit(-1, -1, -1));
	env = ft_transcript_env(msh->env, msh->p.good_path);
	if (status == 255) // OK PROTEGER
		ft_exit(-1, -1, -1);
	execve(msh->p.good_path, msh->p.cmd_opt, env);
	(perror("execve"), ft_exit(-1, -1, -1));
}

static void	update_hdoc_list(t_msh *msh, t_env *head, t_env *prev)
{
	head = msh->p.hdoc;
	prev = NULL;
	while (msh->p.hdoc && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0
		|| (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0
				&& msh->p.hdoc->read == 1)))
	{
		prev = msh->p.hdoc;
		msh->p.hdoc = msh->p.hdoc->next;
	}
	if (msh->p.hdoc)
		msh->p.hdoc->read = 1;
	msh->p.hdoc = head;
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
	head_hd = NULL;
	prev_hd = NULL;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC && rule != CMD_ALONE)
			update_hdoc_list(msh, head_hd, prev_hd);
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
	}
	if (msh->av && (msh->av->token == PIPE || msh->av->token == PAR_CLOSE))
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
}
