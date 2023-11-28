/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/28 14:13:50 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_cwd(t_list **env)
{
	char	*str;
	t_list	*tmp;

	str = NULL;
	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "PWD=", 4))
		{
			str = tmp->content + 4;
			break ;
		}
		tmp = tmp->next;
	}
	return (str);
}

static char	*get_old_pwd(t_list **env)
{
	char	*tmp;
	char	*old_pwd;

	tmp = get_cwd(env);
	if (!tmp)
		return (NULL);
	else
	{
		old_pwd = ft_magic_malloc(ADD, 0, ft_strjoin("OLDPWD=", tmp), NO_ENV);
		return (old_pwd);
	}
}

// static char	*get_path(char	**str)
// {
// 	char	*path;

// 	if (str[1])
// 		path = str[1];
// 	else
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 			return (NULL);
// 	}
// 	return (path);
// }
/*void	change_env(char	*old_pwd)
{
	
}*/

void	builtin_cd(t_msh *minish)
{
	char	*path;
	char	*old_pwd;

	old_pwd = get_old_pwd(minish->env);//malloc
	if (!old_pwd)
		return ;
	printf("old_pwd : %s\n", old_pwd);
	/*path = get_path(str);
	if (!path)
	{
		ft_magic_malloc(FREE, 0, old_pwd);
		return ;
	}
	printf("path : %s\n", path);*/
	path = NULL;
	if (chdir(path) == 0)
	{
		printf("coucou\n");
		//change_env(old_pwd);
	}
	else
		perror("minishell");
	ft_magic_malloc(FREE, 0, old_pwd, NO_ENV);
}
