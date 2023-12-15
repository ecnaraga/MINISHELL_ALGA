/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/14 19:25:21 by athiebau         ###   ########.fr       */
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
	static int	statut = 0;
	
	printf("statut cd : %d\n", statut);
	printf("str : %s\n", old_pwd);
	if(old_pwd || statut != 1)
	{
		printf("1\n");
		new_env_node(old_pwd, 2, minish->env, 2);
		new_env_node(old_pwd, 2, minish->export_env, 1);
		statut = 0;
	}
	else
	{
		if (statut == 1)
		{
			tmp = getcwd(NULL, 0);
			old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", tmp), NO_ENV);
			free(tmp);
			new_env_node(old_pwd, 2, minish->env, 2);
			new_env_node(old_pwd, 2, minish->export_env, 1);
		}
		else
		{
			printf("3\n");
			new_env_node("OLDPWD=", 2, minish->env, 2);
			new_env_node("OLDPWD=", 2, minish->export_env, 1);
			statut = 1;
		}
	}
	tmp = getcwd(NULL, 0);
	if (!tmp)
	 	return ;
	if (old_pwd && statut == 0)
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
	//char	*tmp;

	//printf("BUILTIN\n");
	if(minish->p.cmd_opt[2])
	{
		printf("minishell: cd: too many arguments\n");
		return (1) ;
	}
	old_pwd = get_old_pwd(minish->env);
	/*if(!old_pwd)
	{
		tmp = getcwd(NULL, 0);
		old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", tmp), NO_ENV);
		free(tmp);
	}*/
	if (old_pwd)
		old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", old_pwd), NO_ENV);
	path = get_path(minish->p.cmd_opt);
	if (!path)
	{
		ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
		return (1);
	}
	if (chdir(path) == 0)
		change_env(old_pwd, minish);
	else
		perror("minishell");
	return (0);
}
