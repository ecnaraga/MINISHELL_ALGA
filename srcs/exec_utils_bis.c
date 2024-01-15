/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/15 10:53:50 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_copy_env(t_msh *msh, t_env **env)
{
	char	*str;
	char	*tmp;

	str = mcgic(mlcp(ft_strjoin((*env)->name, "="), 1), ADD, PIP, msh);
	if (!str)
		return (NULL);
	tmp = str;
	str = mcgic(mlcp(ft_strjoin(str, (*env)->content), 1), ADD, PIP, msh);
	mcgic(mlcp(tmp, 0), FREE, PIP, msh);
	if (!str)
		return (NULL);
	return (str);
}

char	**ft_transcript_env(t_env **env, t_msh *msh)
{
	t_env	*head;
	char	**env_tab;
	int		i;

	head = *env;
	env_tab = mcgic(mlcp(NULL, sizeof(char *) * (ft_lstsize_env(*env) + 2)),
			MLC, PIP, msh);
	if (!env_tab)
		return (NULL);
	i = -1;
	while (*env)
	{
		env_tab[++i] = ft_copy_env(msh, env);
		if (msh->status == 255)
			return (NULL);
		*env = (*env)->next;
	}
	env_tab[++i] = NULL;
	*env = head;
	return (env_tab);
}

/*
rule == 1 : close fd.in
rule == 2 : close fd.out
rule == 0 : close fd.in and fd.out
*/
void	ft_close_fd(t_fdpar *fd, int rule, int fd1, int fd2)
{
	if ((rule == 1 || rule == 0) && fd && fd->in > -1)
	{
		close(fd->in);
		fd->in = -1;
	}
	if ((rule == 2 || rule == 0) && fd && fd->out > -1)
	{
		close(fd->out);
		fd->out = -1;
	}
	if (fd1 > -1)
		close(fd1);
	if (fd2 > -1)
		close(fd2);
}

/*
rule == 1 : dup and close fd.in
rule == 2 : dup and close fd.out
rule == 0 : dup and close fd.in and fd.out
*/
int	ft_dup_fd(t_msh *msh, int rule)
{
	if ((rule == 1 || rule == 0) && msh->fd.in > -1)
	{
		if (dup2(msh->fd.in, STDIN_FILENO) == -1)
			return (1);
		close(msh->fd.in);
		msh->fd.in = -1;
	}
	if ((rule == 2 || rule == 0) && msh->fd.out > -1)
	{
		if (dup2(msh->fd.out, STDOUT_FILENO) == -1)
			return (1);
		close(msh->fd.out);
		msh->fd.out = -1;
	}
	ft_close_fd(&msh->fd, 0, -1, -1);
	return (0);
}
