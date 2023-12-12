/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:26:41 by galambey          #+#    #+#             */
/*   Updated: 2023/12/12 13:50:13 by galambey         ###   ########.fr       */
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

void	ft_exit(int fd_1, int fd_2, int fd_3)
{
	if (fd_1 > -1)
		close(fd_1);
	if (fd_2 > -1)
		close(fd_2);
	if (fd_3 > -1)
		close(fd_3);
	ft_magic_malloc(QUIT, 0, NULL, 0);
	// if (rule == CMD_ALONE)
	// 	return (status);
	exit(status);
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

static int	ft_message_directory(char *cmd)
{
	char	*str;
	int		ck;

	ck = ft_check_cmd(cmd);
	if (ck == 1 || ck == 3)
	{
		str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", cmd), PIP);
		if (!str)
			return (1);
		str = ft_magic_malloc(ADD, 0, ft_strjoin(str, ": Is a directory\n"), PIP);
		if (!str)
			return (1);
		write(2, str, ft_strlen(str));
		return (status = 126);
	}
	return (0);
}

void	ft_perr(int err, char *cmd)
{
	char	*str;
	int		ck;

	if (err == E_NO_CMD)
	{
		ck = ft_message_directory(cmd);
		if (ck != 0)
			return ;
		if (ck_char(cmd, '/') > 0)
		{
			str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", cmd), PIP);
			perror(str);
			status = 127;
		}
		else
		{
			str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", cmd), PIP);
			str = ft_magic_malloc(ADD, 0, ft_strjoin(str, ": command not found\n"), PIP);
			status = 127;
			write(2, str, ft_strlen(str));
		}
	}
	else if (err == E_STRJOIN)
		write(2, "ft_strjoin: error malloc\n", 26);
	else if (err == E_STRDUP)
		write(2, "ft_strdup: error malloc\n", 25);
}
