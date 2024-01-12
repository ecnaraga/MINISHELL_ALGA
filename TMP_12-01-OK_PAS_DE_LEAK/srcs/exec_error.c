/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:26:41 by galambey          #+#    #+#             */
/*   Updated: 2024/01/11 15:04:04 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	ft_message_directory(t_msh *msh, char *cmd)
{
	char	*str;
	int		ck;

	ck = ft_check_cmd(msh, cmd);
	if (ck == 1 || ck == 3)
	{
		str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str)
			return (255);
		str = mcgic(mlcp(ft_strjoin(str, ": Is a directory\n"), 1), ADD, PIP,
				msh);
		if (!str)
			return (255);
		write(2, str, ft_strlen(str));
		return (msh->status = 126);
	}
	return (0);
}

static void	ft_cmd_not_found(t_msh *msh, char *cmd)
{
	char	*str;

	if (ck_char(cmd, '/') > 0)
	{
		str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str)
			return ;
		perror(str);
		msh->status = 127;
	}
	else
	{
		str = mcgic(mlcp(ft_strjoin("minishell: ", cmd), 1), ADD, PIP, msh);
		if (!str)
			return ;
		str = mcgic(mlcp(ft_strjoin(str, ": command not found\n"), 1), ADD, PIP,
				msh);
		if (!str)
			return ;
		msh->status = 127;
		write(2, str, ft_strlen(str));
	}
}

int	ft_perr(t_msh *msh, int err, char *cmd)
{
	int	ck;

	if (err == E_NO_CMD)
	{
		ck = ft_message_directory(msh, cmd);
		if (ck != 0)
			return (ck);
		ft_cmd_not_found(msh, cmd);
	}
	return (msh->status);
}
