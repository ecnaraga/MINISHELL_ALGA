/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/18 14:44:31 by galambey         ###   ########.fr       */
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

void	change_env(char	*old_pwd, t_msh *minish)
{
	char	*tmp;
	char	*newpath;
	
	if(old_pwd)
	{
		new_env_node(old_pwd, 2, minish->env, 2);
		new_env_node(old_pwd, 2, minish->export_env, 1);
		ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
	}
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		del_env("OLDPWD", minish);
	 	return ;
	}
	else
	{
		newpath = ft_magic_malloc(ADD, 0, ft_strjoin("PWD=", tmp), NO_ENV);
		new_env_node(newpath, 2, minish->env, 2);
		new_env_node(newpath, 2, minish->export_env, 1);
	}
	free(tmp);
}

int	builtin_cd(t_msh *minish)
{
	char	*path;
	char	*old_pwd;

	if(minish->p.cmd_opt[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (status = 1, 0);
	}
	old_pwd = get_old_pwd(minish->env);
	if (status == 255)
		return (255);
	path = get_path(minish->p.cmd_opt);
	if (!path)
	{
		ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
		return (status = 1, 0);
	}
	if (chdir(path) == 0)
		change_env(old_pwd, minish);
	else
		perror("minishell");
	return (0);
}
