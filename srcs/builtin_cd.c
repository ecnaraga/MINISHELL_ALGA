/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/05 18:07:24 by athiebau         ###   ########.fr       */
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
		old = mcgic(mlcp(ft_strjoin("OLDPWD=", old), 1),
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

static int	change_env(char	*old_pwd, t_msh *msh, int statut)
{
	char	*tmp;
	char	*newpath;

	change_env_helper(old_pwd, msh);
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		del_env("OLDPWD", msh);
		return (0);
	}
	else if (statut == 0)
	{
		newpath = mcgic(mlcp(ft_strjoin("PWD=", tmp), 1), ADD, NO_ENV, msh);
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

void	cd_error_message(t_msh *msh, char *str)
{
	char	*message;
	char	*tmp;

	tmp = mcgic(mlcp(ft_strjoin("minishell: cd: ", str), 1), ADD, NO_ENV,
			msh);
	message = mcgic(mlcp(ft_strjoin(tmp, ": No such file or directory\n"), 1),
			ADD, NO_ENV, msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	ft_putstr_fd(message, 2);
	mcgic(mlcp(message, 0), FREE, NO_ENV, msh);
}

char	*build_cd_exec(t_msh *msh, char **old_pwd, int *statut)
{
	char	*tmp;

	*old_pwd = get_old_pwd(msh->env, msh);
	if (msh->status == 255)
		return (NULL);
	if (*statut == 1 && !(*old_pwd))
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
		{
			perror("minishell: cd");
			msh->status = 1;
			return (NULL);
		}
		*old_pwd = mcgic(mlcp(ft_strjoin("OLDPWD=", tmp), 1), ADD, NO_ENV, msh);
		if (msh->status == 255)
			return (NULL);
		free(tmp);
	}
	if (!(*old_pwd))
		*statut = 1;
	else if (*old_pwd && pwd_exist(msh->env))
		*statut = 0;
	return (getcwd(NULL, 0));
}

int	builtin_cd(t_msh *msh)
{
	char		*path;
	char		*old_pwd;
	char		*test;
	static int	statut = 0;

	if (msh->p.cmd_t[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (msh->status = 1, 0);
	}
	test = build_cd_exec(msh, &old_pwd, &statut);
	if (msh->status != 0)
		return (msh->status);
	if (!test && msh->p.cmd_t[1])
	{
		cd_error_message(msh, msh->p.cmd_t[1]);
		return (msh->status = 1, 0);
	}
	path = get_path(msh->p.cmd_t);
	if (!path)
	{
		if (old_pwd)
			mcgic(mlcp(old_pwd, 0), FREE, NO_ENV, msh);
		return (msh->status = 1, 0);
	}
	if (chdir(path) == 0)
		change_env(old_pwd, msh, statut);
	else
	{
		cd_error_message(msh, path);
		msh->status = 1;
	}
	return (0);
}
