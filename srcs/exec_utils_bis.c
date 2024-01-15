/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/13 13:05:30 by garance          ###   ########.fr       */
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
void	ft_close_fd(int fd1, int fd2)
{
	if (fd1 > -1)
		close(fd1);
	if (fd2 > -1)
		close(fd2);
}
