/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_par.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2024/01/13 13:03:41 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_redef_std_sub(t_msh *msh)
{
	while (msh->av && msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC)
		{
			if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1)
				ft_exit(-1, -1, -1, msh);
			msh->av = msh->av->next;
		}
		else if (msh->av->token == INFILE)
		{
			if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1)
				ft_exit(-1, -1, -1, msh);
		}
		else if (msh->av->token == OUTFILE_TRUNC
			|| msh->av->token == OUTFILE_NO_TRUNC)
		{
			if (redef_stdout(msh, PAR_OPEN, 0, 1) == -1)
				ft_exit(-1, -1, -1, msh);
		}
		else
			msh->av = msh->av->next;
	}
}

static void	ft_init_var(t_msh *msh, t_msh *sub_msh, int sub)
{
	sub_msh->line = ft_strtrim_msh(msh, &msh->line, sub);
	sub_msh->env = msh->env;
	sub_msh->status = msh->status;
	sub_msh->export_env = msh->export_env;
}

static void	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, int sub)
{
	pid_t	pid;

	ft_init_var(msh, sub_msh, sub);
	pid = fork();
	if (pid == -1)
	{
		msh->status = 255;
		ft_exit_bis(msh, sub, -1, -1);
	}
	else if (pid == 0)
	{
		msh->sub = sub;
		(ft_redef_std_sub(msh), ft_minishell(sub_msh, 1));
	}
}

void	ft_exec_par(t_msh *msh, t_split **head, int sub)
{
	t_msh	sub_msh;
	t_env	*head_hd;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	sub_msh.p.hdoc = ft_copy_heredoc(msh, msh->p.hdoc, sub);
	*head = msh->av;
	head_hd = NULL;
	ft_skip_subelem(msh, head_hd, head);
	while (msh->av && msh->av->token == PAR_CLOSE)
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	ft_create_sub_msh(&sub_msh, msh, sub);
	while (msh->av && msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC)
			ft_handle_hdoc(msh, head_hd);
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
	while (wait(&msh->status) > 0)
		;
	if (WIFEXITED(msh->status))
		msh->status = WEXITSTATUS(msh->status);
	msh->av = *head;
}
