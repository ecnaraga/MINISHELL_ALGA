/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stdout_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/05 18:18:44 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_dup_pipe(t_msh *msh, int rule, int j)
{
	if (rule == FIRST || rule == MID)
	{
		if (dup2(msh->p.fd_p[j][1], STDOUT_FILENO) == -1)
		{
			perror("dup2"); // EXIT STATUS ? 
			ft_exit(msh->p.fd_p[j][1], -1, -1, msh);
		}
	}
}

/*
fail open gere dans ft_invalid_infile
*/
static int	ft_open_outfile_tr(t_msh *msh, int *fd_outfile, t_head *save)
{
	if (msh->av->data[0] == '*')
		return (msh->ambiguous = -3, 0);
	if (*fd_outfile != -2)
		close(*fd_outfile);
	if (msh->av->type)
		msh->av->data = ft_expand(msh, msh->av->data, OUTFILE_TRUNC);
	if (msh->status == 255) // IF ERREUR MALLOC RETURN (255)
		return (255);
	if (!msh->av->data[0] || msh->av->data[0] == '*')
		return (msh->ambiguous = -3, 0);
	*fd_outfile = open(msh->av->data, O_CREAT | O_TRUNC | O_WRONLY, 0744); // IF ERREUR OPEN > GERE DANS REDEF_STDOUT
	if (*fd_outfile > -1)
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

/*
fail open gere dans ft_invalid_infile
*/
static int	ft_open_outfile_notr(t_msh *msh, int *fd_outfile, t_head *save)
{
	if (msh->av->data[0] == '*')
		return (msh->ambiguous = -3, 0);
	if (*fd_outfile != -2)
		close(*fd_outfile);
	if (msh->av->type)
		msh->av->data = ft_expand(msh, msh->av->data, OUTFILE_NO_TRUNC);
	if (msh->status == 255) // IF ERREUR MALLOC RETURN (255)
		return (255);
	if (!msh->av->data[0] || msh->av->data[0] == '*')
		return (msh->ambiguous = -3, 0);
	*fd_outfile = open(msh->av->data, O_CREAT | O_APPEND | O_WRONLY, 0744); // IF ERREUR OPEN > GERE DANS REDEF_STDOUT
	if (*fd_outfile > -1)
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

static int	ft_exit_invalidfile(t_msh *msh, int rule, int j)
{
	if ((rule == CMD_ALONE && msh->status == 255) || rule == LAST) // IF ERROR MALLOC
		ft_exit(-1, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j][1], -1, -1, msh);
	return (-1); // donc if (rule == CMD_ALONE + pas d erreur de malloc return -1)
}

int	ft_invalid_outfile(t_msh *msh, int rule, int j, t_head *save)
{
	char *str;
	
	str = mcgic(mlcp(ft_strjoin("minishell: ", msh->av->data), 1), ADD, PIP, msh);
	if (str && msh->ambiguous == -3)
	{
		msh->status = 1;
		str = mcgic(mlcp(ft_strjoin(str, ": ambiguous redirect\n"), 1), ADD, PIP, msh);
		if (str)
			write(2, str, ft_strlen(str));
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
		close (fd.file);
		close (fd.old_std);
		return (-1);
	}
	if (rule == PAR_OPEN)
		ft_exit(fd.file, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], fd.file, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j][1], fd.file, -1, msh);
	else
		ft_exit(fd.file, -1, -1, msh);
	return (-1);
}

int	ft_dup_stdout(t_msh *msh, t_fd *fd, int rule, int j)
{
	fd->old_std = -2;
	if (rule == CMD_ALONE)
	{
		fd->old_std = dup(STDOUT_FILENO);
		if (fd->old_std == -1)
		{
			perror("dup"); // IF ERREUR DUP RETOURNE -1
			msh->status = 1;
			close(fd->file);
			return (-1);
		}
	}
	if (fd->file != -2 && dup2(fd->file, STDOUT_FILENO) == -1)
		return (ft_error_duptwo(msh, rule, *fd, j));
	if (fd->file != -2)
		close(fd->file);
	return (fd->old_std);
}
void	ft_exit_stdout_error_malloc(t_msh *msh, int rule, int j, int sub)
{
	if (sub == 0)
		ft_unlink_heredoc(msh->p.hdoc);
	if (rule == CMD_ALONE || rule == PAR_OPEN)
		ft_exit(-1, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j][1], -1, -1, msh);
	else
		ft_exit(-1, -1, -1, msh);
}



int	redef_stdout(t_msh *msh, int rule, int j, int sub)
{
	t_head	save;
	t_fd	fd;
	
	ft_init_var_std(&save, &fd, msh);
	ft_dup_pipe(msh, rule, j); // A CHECK AVEC LES PIPES
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR
		&& msh->av->token != PAR_CLOSE) //voir si PAR CLOSE OU PAS CAR DANS STDIN PARCLOSE ET PAS LA
	{
		if (msh->av->token == OUTFILE_TRUNC)
		{
			if (ft_open_outfile_tr(msh, &fd.file, &save) == 255) 
				ft_exit_stdout_error_malloc(msh, rule, j, sub); // IF ERREUR MALLOC DANS EXPAND DANS FT_OPEN_OUTFILE_TR > ON QUITTE LE PROCESS ACTUEL
		}
		else if (msh->av->token == OUTFILE_NO_TRUNC)
		{
			if (ft_open_outfile_notr(msh, &fd.file, &save) == 255)
				ft_exit_stdout_error_malloc(msh, rule, j, sub); // IF ERREUR MALLOC DANS EXPAND DANS FT_OPEN_OUTFILE_TR > ON QUITTE LE PROCESS ACTUEL
		}
		else
			ft_next(msh, &save);
		if (fd.file == -1 || msh->ambiguous == -3)
			return (ft_invalid_outfile(msh, rule, j, &save)); // IF ERREUR OPEN OUTFILETR OU OUTFILENOTR > QUITTE LE PROCESS ENFANT SI PIPE ET RETURN (-1 SI CMD_ALONE OU EXEC_PAR)
	}
	msh->av = save.head;
	return (ft_dup_stdout(msh, &fd, rule, j)); // IF ERREUR DUP OU DUP2 QUITTE PROCESS ENFANT SI PIPE SINON RETOURNE -1 (-1 SI CMD_ALONE OU EXEC_PAR)
}
