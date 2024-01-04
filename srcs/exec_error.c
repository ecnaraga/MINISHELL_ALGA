/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:26:41 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 10:23:14 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_fd_p(int **fd_p, int j)
{
	if (j != -1)
	{
		while (--j >= 0)
			free(fd_p[j]);
		free(fd_p);
	}
	else
	{
		while (fd_p[++j])
			free(fd_p[j]);
		free(fd_p);
	}
}

void	ft_exit_bis(t_msh *msh, int sub, int fd1, int fd2)
{
	ft_close_fd(&msh->fd, 0);
	if (fd1 > -1)
		close(fd1);
	if (fd2 > -1)
		close(fd2);
	if (sub == 0 && msh->p.hdoc)
		ft_unlink_heredoc(msh->p.hdoc);
	rl_clear_history();
	mlcgic(NULL, QUIT, 0, msh);
	// ft_magic_malloc(QUIT, 0, NULL, 0);
	exit(msh->status);
}

void	ft_exit(int fd_1, int fd_2, int fd_3, t_msh *msh)
{
	ft_close_fd(&msh->fd, 0);
	if (fd_1 > -1)
		close(fd_1);
	if (fd_2 > -1)
		close(fd_2);
	if (fd_3 > -1)
		close(fd_3);
	rl_clear_history();
	mlcgic(NULL, QUIT, 0, msh);
	// ft_magic_malloc(QUIT, 0, NULL, 0);
	exit(msh->status);
}

static int	ft_check_cmd(char *cmd)
{
	int		i;
	size_t	count_slash;
	size_t	count_space;

	i = -1;
	count_slash = 0;
	count_space = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '/')
			count_slash++;
		if (cmd[i] == ' ')
			count_space++;
	}
	if (count_slash == ft_strlen(cmd))
		return (1);
	if (count_slash > 0 && count_slash + count_space == ft_strlen(cmd))
		return (2);
	if (access(cmd, F_OK | X_OK) == 0)
		return (3);
	return (0);
}

static int	ft_message_directory(t_msh *msh, char *cmd)
{
	char	*str;
	int		ck;

	ck = ft_check_cmd(cmd);
	if (ck == 1 || ck == 3)
	{
		str = mlcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return (255);
		str = mlcgic(mlcp(ft_strjoin(str, ": Is a directory\n"), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return (255);
		write(2, str, ft_strlen(str));
		return (msh->status = 126);
	}
	return (0);
}

void	ft_cmd_not_found(t_msh *msh, char*cmd)
{
	char	*str;
	
	if (ck_char(cmd, '/') > 0)
	{
		str = mlcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return ;
		perror(str);
		msh->status = 127;
	}
	else
	{
		str = mlcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return ;
		str = mlcgic(mlcp(ft_strjoin(str, ": command not found\n"), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return ;
		msh->status = 127;
		write(2, str, ft_strlen(str));
	}
}

int	ft_perr(t_msh *msh, int err, char *cmd)
{
	int		ck;

	if (err == E_NO_CMD)
	{
		ck = ft_message_directory(msh, cmd);
		if (ck != 0)
			return (ck);
		ft_cmd_not_found(msh, cmd);
	}
	return (msh->status);
}
