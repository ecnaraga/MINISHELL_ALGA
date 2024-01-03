/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/03 16:35:10 by galambey         ###   ########.fr       */
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
		*ok_path = mlcgic(mlcp(ft_strjoin(msh->p.path[i], "/"), 1), ADD, PIP, msh);
		if (!ok_path)
			return (255);// OK PROTEGER
		tmp = *ok_path;
		*ok_path = mlcgic(mlcp(ft_strjoin(*ok_path, msh->p.cmd_t[0]), 1), ADD, PIP, msh);
		if (!*ok_path)
			return (255);// OK PROTEGER
		mlcgic(mlcp(tmp, 0), FREE, PIP, msh);
		accss = access(*ok_path, F_OK | X_OK);
		if (accss == 0)
			return (E_OK);
		mlcgic(mlcp(*ok_path, 0), FREE, PIP, msh);
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
		tmp = mlcgic(mlcp(ft_strjoin(msh->p.cmd_t[0], "/"), 1), ADD, PIP, msh);
		if (!tmp) // OK PROTEGER
			return (255);
		accss = access(tmp, F_OK | X_OK);
		if (accss == 0)
			return (mlcgic(mlcp(tmp, 0), FREE, PIP, msh), E_NO_CMD);
		*ok_path = mlcgic(mlcp(ft_strdup(msh->p.cmd_t[0]), 1), ADD, PIP, msh);
		if (!*ok_path) // OK PROTEGER
			return (255);
		return (E_OK);
	}
	if (!msh->p.path)
		return (E_NO_CMD);
	return (ft_find_good_path(msh, ok_path, accss));
}

// int	ft_maj_shlvl(t_msh *msh, char **env_tab, int i)
// {
// 	int shlvl;
// 	char *tmp;
	
// 	if (ft_strncmp(env_tab[i], "SHLVL", 5) == 0)
// 	{
// 		shlvl = ft_atoi(env_tab[i] + 6) + 1;
// 		tmp = mlcgic(mlcp(ft_itoa(shlvl), 1), ADD, PIP, msh);
// 		if (msh->status == 255) // OK PROTEGER
// 			return (1);
// 		mlcgic(mlcp(env_tab[i], 0), FREE, PIP, msh);
// 		env_tab[i] = mlcgic(mlcp(ft_strjoin("SHLVL=", tmp), 1), ADD, PIP, msh);
// 		if (msh->status == 255) // OK PROTEGER
// 			return (255);
// 		mlcgic(mlcp(tmp, 0), FREE, PIP, msh);
// 		return (1);
// 	}
// 	return (0);
// }
// void	ft_inc_shlvl(char **env_tab, char *str, t_msh *msh)
// {
// 	int	i;
// 	int shlvl;
// 	char *tmp;

// 	i = -1;
// 	if (/* ft_strcmp(str, "/usr/bin/bash") != 0 && ft_strcmp(str, "/usr/bin/sh") != 0 && ft_strcmp(str, "/usr/bin/zsh") != 0 && */ ft_strcmp(str, "./minishell")!= 0 )
// 		return;
// 	while (env_tab[++i])
// 	{
// 		// if (ft_maj_shlvl(msh, env_tab, i) > 0)
// 		// 	return ;
// 		if (ft_strncmp(env_tab[i], "SHLVL", 5) == 0)
// 		{
// 			shlvl = ft_atoi(env_tab[i] + 6) + 1;
// 			tmp = mlcgic(mlcp(ft_itoa(shlvl), 1), ADD, PIP, msh);
// 			if (msh->status == 255) // OK PROTEGER
// 				return ;
// 			mlcgic(mlcp(env_tab[i], 0), FREE, PIP, msh);
// 			env_tab[i] = mlcgic(mlcp(ft_strjoin("SHLVL=", tmp), 1), ADD, PIP, msh);
// 			if (msh->status == 255) // OK PROTEGER
// 				return ;
// 			mlcgic(mlcp(tmp, 0), FREE, PIP, msh);
// 			return ;
// 		}
// 	}
// 	env_tab[i] = mlcgic(mlcp(ft_strjoin("SHLVL=", "0"), 1), ADD, PIP, msh);
// 	if (msh->status == 255) // OK PROTEGER
// 		return ;
// 	env_tab[++i] = NULL;
// }

char *ft_copy_env(t_msh *msh, t_env	**env)
{
	char *str;
	char *tmp;
	
	str = mlcgic(mlcp(ft_strjoin((*env)->name, "="), 1), ADD, PIP, msh);
	if (!str)  // OK PROTEGER
		return (NULL);
	tmp = str;
	str = mlcgic(mlcp(ft_strjoin(str, (*env)->content), 1), ADD, PIP, msh);
	mlcgic(mlcp(tmp, 0), FREE, PIP, msh);
	if (!str)  // OK PROTEGER
		return (NULL);
	return (str);
}

char **ft_transcript_env(t_env **env,/*  char *str, */ t_msh *msh)
{
	t_env	*head;
	char	**env_tab;
	int i;
	
	head = *env;
	env_tab = mlcgic(mlcp(NULL, sizeof(char *) * (ft_lstsize_env(*env) + 2)), MALLOC, PIP, msh);
	if (!env_tab)  // OK PROTEGER
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
	// ft_inc_shlvl(env_tab, str, msh);
	// if (msh->status == 255) // OK PROTEGER
	// 	return (NULL);
	return (env_tab);
}
/*
rule == 1 : close fd.in
rule == 2 : close fd.out
rule == 0 : close fd.in and fd.out
*/
void	ft_close_fd(t_msh *msh, int rule)
{
	if ((rule == 1 || rule == 0) && msh->fd.in > -1)
	{
		close(msh->fd.in);
		msh->fd.in = -1;
	}
	if ((rule == 2 || rule == 0) && msh->fd.out > -1)
	{
		close(msh->fd.out);
		msh->fd.out = -1;
	}
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
		if (dup2(msh->fd.in, STDIN_FILENO) == -1) // A PROTEGER
			return (1);
		close(msh->fd.in);
		msh->fd.in = -1;
	}
	if ((rule == 2 || rule == 0) && msh->fd.out > -1)
	{
		if (dup2(msh->fd.out, STDOUT_FILENO) == -1) // A PROTEGER
			return (1);
		close(msh->fd.out);
		msh->fd.out = -1;
	}
	return (0);
}
