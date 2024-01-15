/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:33:55 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/15 11:08:18 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_old_pwd(t_env **env, t_msh *msh)
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
		old = mcgic(mlcp(ft_strjoin("OLDPWD=", old), 1),
				ADD, NO_ENV, msh);
		return (old);
	}
}

char	*get_path(char **str)
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

static void	change_env_helper(char *old_pwd, t_msh *msh)
{
	if (old_pwd)
	{
		new_env_node_env(msh, old_pwd, 2, msh->env);
		new_env_node_export(msh, old_pwd, 2, msh->export_env);
		mcgic(mlcp(old_pwd, 0), FREE, NO_ENV, msh);
	}
	else
	{
		new_env_node_env(msh, "OLDPWD=", 2, msh->env);
		new_env_node_export(msh, "OLDPWD=", 2, msh->export_env);
	}
}

int	change_env(char *old_pwd, t_msh *msh, int statut)
{
	char	*tmp;
	char	*newpath;

	newpath = NULL;
	change_env_helper(old_pwd, msh);
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		del_env("OLDPWD", msh, 0);
		return (0);
	}
	else if (statut == 0)
	{
		newpath = mcgic(mlcp(ft_strjoin("PWD=", tmp), 1), ADD, NO_ENV, msh);
		if (msh->status == 255)
		{
			free(tmp);
			return (255);
		}
		new_env_node_env(msh, newpath, 2, msh->env);
		new_env_node_export(msh, newpath, 2, msh->export_env);
	}
	free(tmp);
	return (0);
}

int	pwd_exist(t_env **env)
{
	t_env	*tmp;

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
