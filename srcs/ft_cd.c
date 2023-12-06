/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/06 15:30:16 by athiebau         ###   ########.fr       */
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

void	change_env(char	*old_pwd, t_env **env)
{
	t_env	*tmp;
	char	*tmp2;
	char	*newpath;

	tmp = *env;
	while(tmp)
	{
		if(ft_strcmp(tmp->name, "OLDPWD"))
		{
			tmp2 = tmp->content;
			break ;
		}
		tmp = tmp->next;
	}
	tmp->content = old_pwd;
	if (tmp2)
		ft_magic_malloc(FREE, 0, tmp2, ENV);
	newpath = getcwd(NULL, 0);
	if (!newpath)
		return ;
	else
	{
		tmp = *env;
		while(tmp)
		{
			if(ft_strcmp(tmp->name, "PWD"))
			{
				tmp2 = tmp->content;
				break ;
			}
			tmp = tmp->next;
		}
		tmp->content = old_pwd;
		if (tmp2)
			ft_magic_malloc(FREE, 0, tmp2, ENV);
	}	
}

void	builtin_cd(char **str, t_msh *minish)
{
	char	*path;
	char	*old_pwd;

	old_pwd = get_old_pwd(minish->env);
	if (!old_pwd)
		return ;
	path = get_path(str);
	if (!path)
	{
		ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
		return ;
	}
	if (chdir(path) == 0)
	{
		printf("coucou\n");
		change_env(old_pwd, minish->env);
	}
	else
		perror("minishell");
	ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
}
