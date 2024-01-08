/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:26:41 by galambey          #+#    #+#             */
/*   Updated: 2024/01/08 12:29:46 by galambey         ###   ########.fr       */
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
	ft_close_fd(&msh->fd, 0, fd1, fd2);
	if (sub == 0 && msh->p.hdoc)
		ft_unlink_heredoc(msh->p.hdoc);
	rl_clear_history();
	mcgic(NULL, QUIT, 0, msh);
	exit(msh->status);
}

void	ft_exit(int fd1, int fd2, int fd3, t_msh *msh)
{
	ft_close_fd(&msh->fd, 0, fd1, fd2);
	if (fd3 > -1)
		close(fd3);
	rl_clear_history();
	mcgic(NULL, QUIT, 0, msh);
	exit(msh->status);
}

static int	ft_check_cmd(t_msh *msh, char *cmd)
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
	if (access(cmd, F_OK | X_OK) == 0 && msh->ambiguous == 0)
		return (3);
	return (0);
}

void	ft_mess_cmdnotfound(t_msh *msh, char *cmd)
{
	char *str;
	
	str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
	if (!str) // OK PROTEGE
		return ;
	str = mcgic(mlcp(ft_strjoin(str, ": command not found\n"), 1), ADD, PIP, msh);
	if (!str) // OK PROTEGE
		return ;
	msh->status = 127;
	write(2, str, ft_strlen(str));
}

void	ft_special_notdir(t_msh *msh)
{
	char *str;
	int access;
	
	//inclure find the good path
	str = mcgic(mlcp(ft_strjoin("minishell: ", msh->av->next->data), 1), ADD, PIP, msh);
	if (!str) // OK PROTEGE
		return ;
	// str = mcgic(mlcp(ft_strjoin(str, ": command not found\n"), 1), ADD, PIP, msh);
	// if (!str) // OK PROTEGE
	// 	return ;
	msh->status = 1;
	perror(str);
	// write(2, str, ft_strlen(str));
}

void	ft_special_dir(t_msh *msh)
{
	char	*tmp;
	char *tmp2;
	
	tmp2 = mcgic(mlcp(ft_strdup(msh->av->next->data), 1), ADD, PIP, msh);
	if (!tmp2) // OK PROTEGER
		return (255);
	tmp = mcgic(mlcp(ft_strjoin(tmp2, "/"), 1), ADD, PIP, msh);
	if (!tmp) // OK PROTEGER
		return (255);
	if (access(tmp, F_OK | X_OK) == 0)
	{
		tmp = mcgic(mlcp(ft_strjoin("minishell: .: ", msh->av->next->data), 1), ADD, PIP, msh);
		if (!tmp) // OK PROTEGER
			return (255);
		tmp = mcgic(mlcp(ft_strjoin(tmp, ": is a directory\n"), 1), ADD, PIP, msh);
		msh->status = 1;
	}
	else
	{
		tmp = mcgic(mlcp(ft_strjoin("minishell: .: ", msh->av->next->data), 1), ADD, PIP, msh);
		if (!tmp) // OK PROTEGER
			return (255);
		tmp = mcgic(mlcp(ft_strjoin(tmp, ": file not found\n"), 1), ADD, PIP, msh);
		msh->status = 1;
	}
}

static int	ft_chec_argument_error(t_msh *msh, char *cmd)
{
	int i;

	i = -1;
	if (cmd[0] != '.' || (cmd[1] && cmd[1]!= '.'))
		return (0);
	if (cmd[1] == '.')
	{
		ft_mess_cmdnotfound(msh, cmd);
		return (1);
	}
	if (!msh->av->next)
	{
		ft_putstr_fd("minishell: .: filename argument required\n.: usage: . filename [arguments]\n", 2);
		msh->status = 2;
		// return (1);
	}
	else
	{
		if (access(msh->av->data, F_OK | X_OK) != 0)
			ft_special_notdir(msh);
		else
			ft_special_dir(msh, cmd);
	}
	return (1);
}

static int	ft_message_directory(t_msh *msh, char *cmd)
{
	char	*str;
	int		ck;

	ck = ft_check_cmd(msh, cmd);
	if (ck == 1 || ck == 3)
	{
		dprintf(2, "cmd |%s|\n", cmd);
		if (ft_chec_argument_error(msh, cmd))
			return (msh->status);
		str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return (255);
		str = mcgic(mlcp(ft_strjoin(str, ": Is a directory\n"), 1), ADD, PIP, msh);
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
		str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str) // OK PROTEGE
			return ;
		perror(str);
		msh->status = 127;
	}
	else
		ft_mess_cmdnotfound(msh, cmd);
	// {
	// 	str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
	// 	if (!str) // OK PROTEGE
	// 		return ;
	// 	str = mcgic(mlcp(ft_strjoin(str, ": command not found\n"), 1), ADD, PIP, msh);
	// 	if (!str) // OK PROTEGE
	// 		return ;
	// 	msh->status = 127;
	// 	write(2, str, ft_strlen(str));
	// }
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
