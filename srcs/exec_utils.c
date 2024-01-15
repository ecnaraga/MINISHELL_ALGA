/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:06:45 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:04:19 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	ft_handle_par(t_msh *msh, int rule, int *par)
{
	if (msh->av->token == PAR_OPEN && rule != CMD_ALONE)
	{
		if (*par == 0)
			ft_strtrim_msh(msh, &msh->line, 1);
		(*par)++;
	}
	else if (msh->av->token == PAR_CLOSE)
		(*par)--;
}

void	ft_parent(t_msh *msh, int fd_1, int fd_2, int rule)
{
	t_split	*head;
	t_env	*head_hd;
	int		par;

	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		perror("signal");
	ft_close_fd(NULL, -1, fd_1, fd_2);
	head_hd = NULL;
	par = 0;
	head = msh->av;
	while ((msh->av && rule != CMD_ALONE) || (msh->av && rule == CMD_ALONE
			&& msh->av->token != PIPE && msh->av->token != OP))
	{
		if (rule != CMD_ALONE)
		{
			if ((msh->av->token == OP || msh->av->token == PIPE) && par == 0)
				break ;
		}
		ft_handle_par(msh, rule, &par);
		if (msh->av->token == HDOC && rule != CMD_ALONE)
			update_hdoc_list(msh, head_hd);
		msh->av = lstdel_relink_split(msh, msh->av, NULL, &head);
	}
	if (msh->av && (msh->av->token == PIPE || msh->av->token == PAR_CLOSE))
		msh->av = lstdel_relink_split(msh, msh->av, NULL, &head);
}

void	ft_child_exec(t_msh *msh)
{
	int		err;
	char	**env;

	err = ft_access_cmd(msh, &msh->p.good_path);
	if (msh->status == 255)
		ft_exit(-1, -1, -1, msh);
	if (err > 0)
		(ft_perr(msh, err, msh->p.cmd_t[0]), ft_exit(-1, -1, -1, msh));
	env = ft_transcript_env(msh->env, msh);
	if (msh->status == 255)
		ft_exit(-1, -1, -1, msh);
	execve(msh->p.good_path, msh->p.cmd_t, env);
	(perror("execve"), ft_exit(-1, -1, -1, msh));
}
