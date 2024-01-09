/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 11:11:53 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_skip_par(t_msh *msh)
{
	int par;

	par = 1;
	dprintf(2, "1 msh->av->data %s\n", msh->av->data);
	msh->av = msh->av->next;
	while (msh->av && par > 0)
	{
		dprintf(2, "2 msh->av->data %s\n", msh->av->data);
		if (msh->av->token == PAR_OPEN)
			(par)++;
		else if (msh->av->token == PAR_CLOSE)
			(par)--;
		msh->av = msh->av->next;
	}
}

int	ft_search_pipe(t_msh *msh)
{
	t_split *head;

	head = msh->av;
	while (msh->av && msh->av->token != OPERATOR)
	{
		dprintf(2, "0 msh->av->data %s\n", msh->av->data);
		if (msh->av->token == PAR_OPEN)
		{
			ft_skip_par(msh);
			if (!msh->av)
				break;
		}
		if (msh->av->token == PIPE)
			return (msh->av = head, 1);
		if (msh->av->token == OPERATOR)
			return (msh->av = head, 0);
		msh->av = msh->av->next;
	}
	return (msh->av = head, 0);
}

static void	ft_init_var(t_msh *msh, t_msh *sub_msh, int sub)
{
	sub_msh->line = ft_strtrim_msh(msh, &msh->line, sub); // SI MALLOC KO, ON QUITTE LE PROCESS ACTUEL A L INTERIEUR
	sub_msh->env = msh->env;
	sub_msh->status = msh->status;
	sub_msh->export_env = msh->export_env;
}

void	ft_redef_std_sub(t_msh *msh, t_fdpar *fd)
{
	while (msh->av && msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC)
		{
			ft_close_fd(fd, 1, -1, -1);
			if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
				ft_exit(-1, -1, -1, msh);
			msh->av = msh->av->next;
		}
		else if (msh->av->token == INFILE)
		{
			ft_close_fd(fd, 1, -1, -1);
			if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
				ft_exit(-1, -1, -1, msh);
		}
		else if (msh->av->token == OUTFILE_TRUNC
			|| msh->av->token == OUTFILE_NO_TRUNC)
		{	
			ft_close_fd(fd, 2, -1, -1);
			if (redef_stdout(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
				ft_exit(-1, -1, -1, msh); 
		}
		else
			msh->av = msh->av->next;
	}
}

void	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, int sub, t_fdpar *fd)
{
	pid_t	pid;
	
	ft_init_var(msh, sub_msh, sub);
	pid = fork();
	if (pid == -1)
	{
		if (fd && fd->in > -1)
			close(fd->in);
		if (fd && fd->out > -1)
			close(fd->out);
		(msh->status = 255, ft_exit_bis(msh, sub, -1, -1)); // SI FORK KO, ON QUITTE LE PROCESS ACTUEL 
	}
	else if (pid == 0)
	{
		(dprintf(2, "sub_msh pid %d\n", getpid()));
		msh->sub = sub;
		ft_redef_std_sub(msh, fd);
		ft_minishell(sub_msh, 1, fd);
	}
	else
	{
		if (fd && fd->in > -1)
			close(fd->in);
		if (fd && fd->out > -1)
			close(fd->out);
	}
}

/*
Return 1 : We go into the loop
Return 0 : We don't
The condition wrote in the condition's loop would be : 
	while (msh->p.hdoc && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0 && msh->p.hdoc->read == 1)))
*/
int		ft_test_hdoc(t_msh *msh)
{
	if (!msh->p.hdoc)
		return (0);
	if (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0)
		return (1);
	if (msh->p.hdoc->read != 1)
		return (0);
	if (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0)
		return (1);
	return (0);
}

void	ft_handle_hdoc(t_msh *msh, t_env *head_hd)
{
		head_hd = msh->p.hdoc;
		while (ft_test_hdoc(msh))
			msh->p.hdoc = msh->p.hdoc->next;
		if (msh->p.hdoc)
			msh->p.hdoc->read = 1;
		msh->p.hdoc = head_hd;
}

void	ft_skip_subelem(t_msh *msh, t_env *head_hd, t_split **head)
{
	int par;

	par = 0;
	while (msh->av && (msh->av->token != PAR_CLOSE || par != 0))
	{
		if (msh->av->token == PAR_OPEN)
			par++;
		else if (msh->av->token == PAR_CLOSE)
		{
			par--;
			if (par == 0)
				break ;
		}
		else if (msh->av->token == HDOC)
			ft_handle_hdoc(msh, head_hd);
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
}

void ft_exec_par(t_msh *msh, t_split **head, int sub, t_fdpar *fd)
{
	t_msh	sub_msh;
	t_env *head_hd;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	sub_msh.p.hdoc = ft_copy_heredoc(msh, msh->p.hdoc, sub); // SI MALLOC KO ON QUITTE DANS 
	*head = msh->av;
	head_hd = NULL;
	ft_skip_subelem(msh, head_hd, head);
	while (msh->av && msh->av->token == PAR_CLOSE) // et pourquoi pas if a la place
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	ft_create_sub_msh(&sub_msh, msh, sub, fd); // OK TOUT EST PROTEGE A L INTERIEUR
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

void	ft_choice_exec(t_msh *msh, t_split **head, int sub)
{
	if (ft_search_pipe(msh) == 1)
		pipex_multi(msh, sub); // OK PROTEGE A L INTERIEUR
	else if (msh->av->token == PAR_OPEN)
		ft_exec_par(msh, head, sub, NULL); // OK PROTEGE A L INTERIEUR
	else
		ft_cmd_alone(msh, sub); // OK PROTEGE A L INTERIEUR
}

void	ft_exec_and(t_msh *msh, t_split **head, int sub)
{
	if (msh->status == 0)
	{
		msh->av = msh->av->next;
		ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
		*head = msh->av;
	}
	else
	{
		while (msh->av)
			msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
}

void	ft_exec_or(t_msh *msh, t_split **head, int sub)
{
	if (msh->status != 0)
	{
		msh->previous_status = msh->status;
		msh->status = 0;
		msh->av = msh->av->next;
		ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
		*head = msh->av;
	}
	else
	{
		while (msh->av)
			msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
}
	
int	ft_exec_operator(t_msh *msh, t_split **head, int sub)
{
	ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
	*head = msh->av;
	while (msh->av)
	{
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0)
			ft_exec_and(msh, head, sub);
		else if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
			ft_exec_or(msh, head, sub);
		else
			msh->av = msh->av->next;
	}
	return (0);
}



int	ft_exec(t_msh *msh, int sub, t_fdpar *fd)
{
	t_split *head;
	
	if (sub == 0 && ft_heredoc(msh) == 130) // SI ERREUR MALLOC OU OPEN ON QUITTE A L INTERIEUR
		return (1); // OK CTRL + C GERE
	head = msh->av;
	if (fd)
	{
		msh->fd.in = fd->in;
		msh->fd.out = fd->out;
	}
	else
	{
		msh->fd.in = -1;
		msh->fd.out = -1;
	}
	ft_exec_operator(msh, &head, sub); // OK PROTEGE A L INTERIEUR
	if (sub == 0)
		ft_unlink_heredoc(msh->p.hdoc);
	mcgic(NULL, FLUSH, NO_ENV, msh);
	return (0);
}
