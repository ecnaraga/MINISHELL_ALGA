/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stdin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/05 18:18:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_dup_pipe(t_msh *msh, int rule, int j)
{
	if (rule == MID || rule == LAST)
	{
		if (dup2(msh->p.fd_p[j - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2");  // EXIT STATUS ?
			if (rule == MID)
				ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1, msh);
			else
				ft_exit(msh->p.fd_p[j - 1][0], -1, -1, msh);
		}
	}
}

/*
fail open gere dans ft_invalid_infile
*/
static int	ft_open_infile(t_msh *msh, int *fd_infile, t_head *save)
{
	if (msh->av->data[0] == '*')
		return (msh->ambiguous = -3, 0);
	if (*fd_infile != -2)
		close(*fd_infile);
	if (msh->av->type )
		msh->av->data = ft_expand(msh, msh->av->data, INFILE); //IF ERROR MLC, EXPAND RETURN (NULL)
	if (msh->status == 255) // IF ERREUR MALLOC RETURN (255)
		return (255);
	if (!msh->av->data[0] || msh->av->data[0] == '*')
		return (msh->ambiguous = -3, 0);
	*fd_infile = open(msh->av->data, O_RDONLY); // IF ERREUR OPEN > GERE DANS REDEF_STDIN
	if (*fd_infile > -1)
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

/*
fail open gere dans ft_invalid_infile
*/
static void ft_open_heredoc(t_msh *msh, int *fd_infile, t_head *save)
{
	if (*fd_infile != -2)
		close(*fd_infile);
	save->head_hd = msh->p.hdoc;
	while (msh->p.hdoc && msh->av && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0 && msh->p.hdoc->read == 1)))//
		msh->p.hdoc = msh->p.hdoc->next;
	*fd_infile = open(msh->p.hdoc->content, O_RDONLY);  // IF ERREUR OPEN > GERE DANS REDEF_STDIN
	msh->p.hdoc->read = 1;
	msh->p.hdoc = save->head_hd;
	save->prev = msh->av;
	msh->av = msh->av->next;
}

static int	ft_exit_invalidfile(t_msh *msh, int rule, int j)
{
	if (rule == CMD_ALONE && msh->status == 255) // IF ERROR MALLOC
		ft_exit(-1, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1, msh);
	if (rule == LAST)
		ft_exit(msh->p.fd_p[j - 1][0], -1, -1, msh);
	return (-1); // donc if (rule == CMD_ALONE + pas d erreur de malloc return -1)
}

int	ft_invalid_infile(t_msh *msh, int rule, int j, t_head *save)
{
	char *str;
	
	str = mcgic(mlcp(ft_strjoin("minishell: ", msh->av->data), 1), ADD, PIP, msh);
	if (str && msh->ambiguous == -3)
	{
		msh->status = 1;
		str = mcgic(mlcp(ft_strjoin(str, ": ambiguous redirect\n"), 1), ADD, PIP, msh);
		if (str)
			write(2, str, ft_strlen(str));
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
		mcgic(mlcp(str, 0), FREE, PIP, msh);
	}
	else if (str)
	{
		perror(str);
		msh->status = 1;
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
		mcgic(mlcp(str, 0), FREE, PIP, msh);
	}
	return (ft_exit_invalidfile(msh, rule, j));
}

//laisser en static
static int	ft_error_duptwo(t_msh *msh, int rule, t_fd fd, int j)
{
	perror("dup2");
	msh->status = 1;
	if (rule == CMD_ALONE)
	{
		close(fd.file);
		close(fd.old_std);
		return (-1);
	}
	if (rule == PAR_OPEN)
		ft_exit(fd.file, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], fd.file, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], fd.file, msh);
	else
		ft_exit(msh->p.fd_p[j - 1][0], fd.file, -1, msh);
	return (-1);
}

int	ft_dup_stdin(t_msh *msh, t_fd *fd, int rule, int j)
{
	fd->old_std = -2;
	if (rule == CMD_ALONE)
	{
		fd->old_std = dup(STDIN_FILENO);
		if (fd->old_std == -1)
		{
			perror("dup"); // IF ERREUR DUP RETOURNE -1
			msh->status = 1;
			close(fd->file);
			return (-1);
		}
	}
	if (fd->file != -2 && dup2(fd->file, STDIN_FILENO) == -1)
		return (ft_error_duptwo(msh, rule, *fd, j)); // IF ERREUR DUP2 QUITTE PROCESS ENFANT SI PIPE SINON RETOURNE -1
	if (fd->file != -2)
		close(fd->file);
	return (fd->old_std);
}

void	ft_exit_stdin_error_malloc(t_msh *msh, int rule, int j, int sub)
{
	if (sub == 0)
		ft_unlink_heredoc(msh->p.hdoc);
	if (rule == CMD_ALONE || rule == PAR_OPEN)
		ft_exit(-1, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1, msh);
	else
		ft_exit(msh->p.fd_p[j - 1][0], -1, -1, msh);
}

void	ft_init_var_std(t_head *save, t_fd	*fd, t_msh *msh)
{
	save->head = msh->av;
	save->prev = NULL;
	fd->file = -2;
	msh->ambiguous = 0;
}

void ft_next(t_msh *msh, t_head *save)
{
	save->prev = msh->av;
	msh->av = msh->av->next;
}

/*
Open and close in a loop all the heredoc and infile until a pipe, operator or
	the end of commandline.
The last heredoc or infile encountered is dup2 to stdin_fileno before being
	closed
*/
int	redef_stdin(t_msh *msh, int rule, int j, int sub)
{
	t_fd	fd;
	t_head	save;

	ft_init_var_std(&save, &fd, msh);
	ft_dup_pipe(msh, rule, j); // A CHECKER QUAND CHECK PIPEX MULTI
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR && msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == INFILE)
		{
			if (ft_open_infile(msh, &fd.file, &save) == 255)
				ft_exit_stdin_error_malloc(msh, rule, j, sub); // IF ERREUR MALLOC DANS EXPAND DANS FT_OPEN_INFILE > ON QUITTE LE PROCESS ACTUEL
		}
		else if (msh->av->token == HDOC)
			ft_open_heredoc(msh, &fd.file, &save);
		else
			ft_next(msh, &save);
		if (fd.file == -1 || msh->ambiguous == -3) // IF ERREUR OPEN DE FT_OPEN_INFILE OU FT_OPEN_HEREDOC > QUITTE LE PROCESS ENFANT SI PIPE ET RETURN (-1 SI CMD_ALONE OU EXEC_PAR)
			return (ft_invalid_infile(msh, rule, j, &save));
		if (rule == PAR_OPEN)
			break;
	}
	msh->av = save.head;
	return (ft_dup_stdin(msh, &fd, rule, j)); // IF ERREUR DUP OU DUP2 QUITTE PROCESS ENFANT SI PIPE SINON RETOURNE -1 (-1 SI CMD_ALONE OU EXEC_PAR)
}

