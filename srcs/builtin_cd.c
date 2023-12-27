/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/24 17:03:57 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_old_pwd(t_env **env, t_msh *msh)
{
	char	*old;
	t_env	*tmp;

	old = NULL;
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PWD"))
		{
			old = tmp->content;
			break ;
		}
		tmp = tmp->next;
	}
	if (!old)
		return (NULL);
	else
	{
		old = mlcgic(mlcp(ft_strjoin("OLDPWD=", old), 1),
				ADD, NO_ENV, msh);
		return (old);
	}
}

static char	*get_path(char	**str)
{
	char	*path;

	if (str[1])
		path = str[1];
	else
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	return (path);
}

int	change_env(char	*old_pwd, t_msh *msh, int statut)
{
	char	*tmp;
	char	*newpath;
	
	if(old_pwd)
	{
		new_env_node_env(msh, old_pwd, 2, msh->env);
		new_env_node_export(msh, old_pwd, 2, msh->export_env);
		mlcgic(mlcp(old_pwd, 0), FREE, NO_ENV, msh); // ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
	}
	else
	{
		new_env_node_env(msh, "OLDPWD=", 2, msh->env);
		new_env_node_export(msh, "OLDPWD=", 2, msh->export_env);
	}
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		del_env("OLDPWD", msh);
		return (0);
	}
	else if (statut == 0)
	{
		newpath = mlcgic(mlcp(ft_strjoin("PWD=", tmp), 1), ADD, NO_ENV, msh); // newpath = ft_magic_malloc(ADD, 0, ft_strjoin("PWD=", tmp), NO_ENV);
		if (msh->status == 255)
			return (255);
		new_env_node_env(msh, newpath, 2, msh->env);
		new_env_node_export(msh, newpath, 2, msh->export_env);
	}
	free(tmp);
	return (0);
}

int	pwd_exist(t_env **env)
{
	// int	i;
	t_env	*tmp;

	// i = 0;
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PWD"))
		{
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	builtin_cd(t_msh *msh)
{
	char	*path;
	char	*old_pwd;
	char	*tmp;
	static int	statut = 0;

	if(msh->p.cmd_opt[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (msh->status = 1, 0);
	}
	old_pwd = get_old_pwd(msh->env, msh);
	if (msh->status == 255)
		return (255);
	if (statut == 1 && !old_pwd)
	{
		tmp = getcwd(NULL, 0);
		old_pwd = mlcgic(mlcp(ft_strjoin("OLDPWD=", tmp), 1), ADD, NO_ENV, msh); // old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", tmp), NO_ENV);
		if (msh->status == 255)
			return (255);
		free(tmp);
	}
	if (!old_pwd)
		statut = 1;
	else if (old_pwd && pwd_exist(msh->env))
		statut = 0;
	path = get_path(msh->p.cmd_opt);
	if (!path)
	{
		if (old_pwd)
			mlcgic(mlcp(old_pwd, 0), FREE, NO_ENV, msh); // ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
		return (msh->status = 1, 0);
	}
	if (chdir(path) == 0)
		change_env(old_pwd, msh, statut);
	else
		perror("minishell");
	return (0);
}
