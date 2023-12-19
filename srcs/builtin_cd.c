/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/19 14:21:26 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_old_pwd(t_env **env)
{
	char	*old_pwd;
	t_env	*tmp;

	old_pwd = NULL;
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PWD"))
		{
			old_pwd = tmp->content;
			break ;
		}
		tmp = tmp->next;
	}
	if (!old_pwd)
		return (NULL);
	else
	{
		old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", old_pwd), NO_ENV);
		return (old_pwd);
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

void	change_env(char	*old_pwd, t_msh *minish, int statut)
{
	char	*tmp;
	char	*newpath;
	
	if(old_pwd)
	{
		new_env_node(old_pwd, 2, minish->env, 2);
		new_env_node(old_pwd, 2, minish->export_env, 1);
		ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
	}
	else
	{
		new_env_node("OLDPWD=", 2, minish->env, 2);
		new_env_node("OLDPWD=", 2, minish->export_env, 1);
	}
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		del_env("OLDPWD", minish);
		free(tmp);
		return ;
	}
	else if (statut == 0)
	{
		newpath = ft_magic_malloc(ADD, 0, ft_strjoin("PWD=", tmp), NO_ENV);
		new_env_node(newpath, 2, minish->env, 2);
		new_env_node(newpath, 2, minish->export_env, 1);
	}
	free(tmp);
}

int	pwd_exist(t_env **env)
{
	int	i;
	t_env	*tmp;

	i = 0;
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

int	builtin_cd(t_msh *minish)
{
	char	*path;
	char	*old_pwd;
	char	*tmp;
	static int	statut = 0;

	if(minish->p.cmd_opt[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (status = 1, 0);
	}
	old_pwd = get_old_pwd(minish->env);
	if (status == 255)
		return (255);
	if (statut == 1 && !old_pwd)
	{
		tmp = getcwd(NULL, 0);
		old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", tmp), NO_ENV);
		free(tmp);
	}
	if (!old_pwd)
		statut = 1;
	else if (old_pwd && pwd_exist(minish->env))
		statut = 0;
	path = get_path(minish->p.cmd_opt);
	if (!path)
	{
		if (old_pwd)
			ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
		return (status = 1, 0);
	}
	if (chdir(path) == 0)
		change_env(old_pwd, minish, statut);
	else
		perror("minishell");
	return (0);
}
