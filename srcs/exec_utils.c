/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/12/19 10:57:59 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(t_pipex *p)
{
	p->good_path = NULL;
	p->cmd_opt = NULL;
	p->fd_p = NULL;
	p->re_split = 0;
}

void	ft_parse(t_msh *msh, int sub)
{
	ft_init_var(&msh->p);
	msh->p.path = ft_research_path(msh, msh->env, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
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
			return (ft_split_magic_malloc(msh, sub, node->content, ':')); // IF ERROR MALLOC ON QUITTE LE PROCESS ACTUEL A L INTERIEUR
			// return (ft_split_magic_malloc(msh, sub, node->content + 1, ':')); // IF ERROR MALLOC ON QUITTE LE PROCESS ACTUEL A L INTERIEUR
		node = node->next;
	}
	return (NULL);
}

static int	ft_find_good_path(char **path, char **good_path, char *cmd,
		int accss)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path[i] && accss != 0)
	{
		*good_path = ft_magic_malloc(ADD, 0, ft_strjoin(path[i], "/"), PIP);
		if (!good_path)
			return (255);// OK PROTEGER
		tmp = *good_path;
		*good_path = ft_magic_malloc(ADD, 0, ft_strjoin(*good_path, cmd), PIP);
		if (!*good_path)
			return (255);// OK PROTEGER
		ft_magic_malloc(FREE, 0, tmp, PIP);
		accss = access(*good_path, F_OK | X_OK);
		if (accss == 0)
			return (E_OK);
		ft_magic_malloc(FREE, 0, *good_path, PIP);
		*good_path = NULL;
		i++;
	}
	return (E_NO_CMD);
}

int	ft_access_cmd(char **path, char *cmd, char **good_path)
{
	int		accss;
	char	*tmp;

	accss = access(cmd, F_OK | X_OK);
	if (accss == 0)
	{
		tmp = ft_magic_malloc(ADD, 0, ft_strjoin(cmd, "/"), PIP);
		if (!tmp) // OK PROTEGER
			return (255);
		accss = access(tmp, F_OK | X_OK);
		if (accss == 0)
			return (ft_magic_malloc(FREE, 0, tmp, PIP), E_NO_CMD);
		*good_path = ft_magic_malloc(ADD, 0, ft_strdup(cmd), PIP);
		if (!*good_path) // OK PROTEGER
			return (255);
		return (E_OK);
	}
	if (!path)
		return (E_NO_CMD);
	return (ft_find_good_path(path, good_path, cmd, accss));
}
void	ft_inc_shlvl(char **env_tab, char *str)//IF ERROR MALLOC?
{
	int	i;
	int shlvl;
	char *tmp;

	i = -1;
	if (ft_strcmp(str, "/usr/bin/bash") != 0 && ft_strcmp(str, "/usr/bin/sh") != 0 && ft_strcmp(str, "/usr/bin/zsh") != 0 && ft_strcmp(str, "./minishell")!= 0 )
		return;
	while (env_tab[++i])
	{
		if (ft_strncmp(env_tab[i], "SHLVL", 5) == 0)
		{
			shlvl = ft_atoi(env_tab[i] + 6) + 1;
			tmp = ft_magic_malloc(ADD, 0, ft_itoa(shlvl), PIP);
			if (status == 255) // OK PROTEGER
				return ;
			ft_magic_malloc(FREE, 0, env_tab[i], PIP);
			env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin("SHLVL=", tmp), PIP);
			if (status == 255) // OK PROTEGER
				return ;
			ft_magic_malloc(FREE, 0, tmp, PIP);
			return ;
		}
	}
	env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin("SHLVL=", "0"), PIP);
	if (status == 255) // OK PROTEGER
		return ;
	env_tab[++i] = NULL;
}

char **ft_transcript_env(t_env **env, char *str)
{
	t_env	*head;
	char	**env_tab;
	char *tmp;
	int i;
	
	head = *env;
	env_tab = ft_magic_malloc(MALLOC, sizeof(char *) * (ft_lstsize_env(*env) + 2), NULL, PIP);
	if (!env_tab)  // OK PROTEGER
			return (NULL);
	i = 0;
	while (*env)
	{
		env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin((*env)->name, "="), PIP);
		if (!env_tab[i])  // OK PROTEGER
			return (NULL);
		tmp = env_tab[i];
		env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin(env_tab[i], (*env)->content), PIP);
		ft_magic_malloc(FREE, 0, tmp, PIP);
		if (!env_tab[i])  // OK PROTEGER
			return (NULL);
		i++;
		*env = (*env)->next;
	}
	env_tab[i] = NULL;
	*env = head;
	ft_inc_shlvl(env_tab, str);
	if (status == 255) // OK PROTEGER
		return (NULL);
	return (env_tab);
}
