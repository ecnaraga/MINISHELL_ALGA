/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/06 18:35:46 by athiebau         ###   ########.fr       */
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
	old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD", old_pwd), NO_ENV);
	return (old_pwd);
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
			return (NULL);
	}
	return (path);
}

void	change_env(char	*old_pwd, t_msh *minish)
{
	char	*tmp;
	char	*newpath;

	new_env_node(old_pwd, 2, minish->env, 2);
	new_env_node(old_pwd, 2, minish->export_env, 1);
	tmp = getcwd(NULL, 0);
	if (!tmp)
	 	return ;
	printf("newpath : %s\n", tmp);
	newpath = ft_magic_malloc(ADD, 0, ft_strjoin("PWD=", tmp), NO_ENV);
	new_env_node(newpath, 2, minish->env, 2);
	new_env_node(newpath, 2, minish->export_env, 1);
	ft_magic_malloc(FREE, 0, tmp, NO_ENV);
}

void	builtin_cd(char **str, t_msh *minish)
{
	char	*path;
	char	*old_pwd;

	old_pwd = get_old_pwd(minish->env);
	if (!old_pwd)
		return ;
	printf("old pwd : %s\n", old_pwd);
	path = get_path(str);
	if (!path)
	{
		ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
		return ;
	}
	printf("path : %s\n", path);
	if (chdir(path) == 0)
		change_env(old_pwd, minish);
	else
		perror("minishell");
}
