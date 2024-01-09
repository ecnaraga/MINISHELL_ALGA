/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:06:45 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 12:13:50 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		ft_search_builtin_bis(t_msh *msh)
{
	int r_exit;
	
	if (ft_strcmp(msh->p.cmd_t[0], "unset") == 0)
		return (builtin_unset(msh, 0), 1);
	else if (ft_strcmp(msh->p.cmd_t[0], "env") == 0)
	{
		builtin_env(msh);
		if (msh->status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_t[0], "exit") == 0)
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
	if (ft_strcmp(msh->p.cmd_t[0], "echo") == 0)
		return (builtin_echo(msh), 1);
	else if (ft_strcmp(msh->p.cmd_t[0], "cd") == 0)
	{
		builtin_cd(msh);
		if (msh->status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_t[0], "pwd") == 0)
		return (builtin_pwd(msh), 1);
	else if (ft_strcmp(msh->p.cmd_t[0], "export") == 0)
	{
		builtin_export(msh);
		if (msh->status == 255)
			return (255);
		return (1);
	}
	return (ft_search_builtin_bis(msh));
}

void	ft_child_exec(t_msh *msh)
{
	int	err;
	char **env;

	err = ft_access_cmd(msh, &msh->p.good_path);
	if (msh->status == 255)// OK PROTEGER
		ft_exit(-1, -1, -1, msh);
	if (err > 0) // OK PROTEGER
		(ft_perr(msh, err, msh->p.cmd_t[0]), ft_exit(-1, -1, -1, msh));
	env = ft_transcript_env(msh->env, msh);
	if (msh->status == 255) // OK PROTEGER
		ft_exit(-1, -1, -1, msh);
	execve(msh->p.good_path, msh->p.cmd_t, env);
	(perror("execve"), ft_exit(-1, -1, -1, msh));
}

static void	update_hdoc_list(t_msh *msh, t_env *head)
{
	head = msh->p.hdoc;
	while (msh->p.hdoc && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0
		|| (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0
				&& msh->p.hdoc->read == 1)))
		msh->p.hdoc = msh->p.hdoc->next;
	if (msh->p.hdoc)
		msh->p.hdoc->read = 1;
	msh->p.hdoc = head;
}

// static void	ft_init_var(t_env **head_hd, int *par)
// {
// 	*head_hd = NULL;
// 	*par = 0;
// }

void	ft_handle_par(t_msh *msh, int rule, int *par)
{
	if(msh->av->token == PAR_OPEN && rule != CMD_ALONE)
	{
		if (*par == 0)
			ft_strtrim_msh(msh, &msh->line, 1); // OK PROTEGER A L INTERIEUR
		(*par)++;
	}
	else if (msh->av->token == PAR_CLOSE)
		(*par)--;
}

void	ft_parent(t_msh *msh, int fd_1, int fd_2, int rule)
{
	t_split *head;
	t_env *head_hd;
	int		par;
	
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		perror("signal");
	ft_close_fd(NULL, -1, fd_1, fd_2);
	// ft_init_var(&head_hd, &par);
	head_hd = NULL;
	par = 0;
	head = msh->av;
	while (msh->av && ((msh->av->token != PIPE && msh->av->token != OPERATOR) || (par == 1 && rule != CMD_ALONE)))
	{
		ft_handle_par(msh, rule, &par);
		if (msh->av->token == HDOC && rule != CMD_ALONE)
			update_hdoc_list(msh, head_hd);
		msh->av = lstdel_relink_split(msh, msh->av, NULL, &head);
	}
	if (msh->av && (msh->av->token == PIPE || msh->av->token == PAR_CLOSE))
		msh->av = lstdel_relink_split(msh, msh->av, NULL, &head);
}
