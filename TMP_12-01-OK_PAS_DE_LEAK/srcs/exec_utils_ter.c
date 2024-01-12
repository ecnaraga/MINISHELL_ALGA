/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_ter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/09 16:19:03 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(t_pipex *p)
{
	p->good_path = NULL;
	p->cmd_t = NULL;
	p->fd_p = NULL;
	p->re_split = 0;
}

void	ft_parse(t_msh *msh, int sub)
{
	ft_init_var(&msh->p);
	msh->p.path = ft_research_path(msh, msh->env, sub);
}

/*
Return a tab of strings of the the known paths.
If malloc ko, the process is quit in ft_split.
Return NULL if there is no env or if the path has been unset or contain nothing.
*/
char	**ft_research_path(t_msh *msh, t_env **env, int sub)
{
	t_env	*node;

	if (!env)
		return (NULL);
	node = *env;
	while (node)
	{
		if (ft_strncmp(node->name, "PATH", 4) == 0)
			return (ft_split_magic_malloc(msh, sub, node->content, ':'));
		node = node->next;
	}
	return (NULL);
}

static int	ft_find_good_path(t_msh *msh, char **ok_path, int accss)
{
	char	*tmp;
	int		i;

	i = 0;
	while (msh->p.path[i] && accss != 0)
	{
		*ok_path = mcgic(mlcp(ft_strjoin(msh->p.path[i], "/"), 1), ADD, PIP,
				msh);
		if (!ok_path)
			return (255);
		tmp = *ok_path;
		*ok_path = mcgic(mlcp(ft_strjoin(*ok_path, msh->p.cmd_t[0]), 1), ADD,
				PIP, msh);
		if (!*ok_path)
			return (255);
		mcgic(mlcp(tmp, 0), FREE, PIP, msh);
		accss = access(*ok_path, F_OK | X_OK);
		if (accss == 0)
			return (E_OK);
		mcgic(mlcp(*ok_path, 0), FREE, PIP, msh);
		*ok_path = NULL;
		i++;
	}
	return (E_NO_CMD);
}

int	ft_access_cmd(t_msh *msh, char **ok_path)
{
	int		accss;
	char	*tmp;

	accss = access(msh->p.cmd_t[0], F_OK | X_OK);
	if (accss == 0)
	{
		tmp = mcgic(mlcp(ft_strjoin(msh->p.cmd_t[0], "/"), 1), ADD, PIP, msh);
		if (!tmp)
			return (255);
		accss = access(tmp, F_OK | X_OK);
		if (accss == 0)
			return (mcgic(mlcp(tmp, 0), FREE, PIP, msh), E_NO_CMD);
		*ok_path = mcgic(mlcp(ft_strdup(msh->p.cmd_t[0]), 1), ADD, PIP, msh);
		if (!*ok_path)
			return (255);
		return (E_OK);
	}
	if (!msh->p.path)
		return (E_NO_CMD);
	return (ft_find_good_path(msh, ok_path, accss));
}
