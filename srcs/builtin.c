/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:52:53 by galambey          #+#    #+#             */
/*   Updated: 2024/01/11 15:08:51 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_search_builtin_bis(t_msh *msh)
{
	int	r_exit;

	if (ft_strcmp(msh->p.cmd_t[0], "unset") == 0)
		return (builtin_unset(msh, 0), 1);
	else if (ft_strcmp(msh->p.cmd_t[0], "env") == 0)
	{
		builtin_env(msh);
		if (msh->status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_t[0], "exit") == 0)
	{
		r_exit = builtin_exit(msh);
		if (r_exit == 255)
			return (255);
		if (r_exit == 1)
			return (2);
		return (1);
	}
	return (0);
}

/*
If error malloc in a builtin return 255
If return 0 > no builtin
If return 1 > builtin exec ok
*/
int	ft_search_builtin(t_msh *msh)
{
	if (ft_strcmp(msh->p.cmd_t[0], "echo") == 0)
		return (builtin_echo(msh), 1);
	else if (ft_strcmp(msh->p.cmd_t[0], "cd") == 0)
	{
		builtin_cd(msh);
		if (msh->status == 255)
			return (255);
		return (1);
	}
	else if (ft_strcmp(msh->p.cmd_t[0], "pwd") == 0)
		return (builtin_pwd(msh), 1);
	else if (ft_strcmp(msh->p.cmd_t[0], "export") == 0)
	{
		builtin_export(msh);
		if (msh->status == 255)
			return (255);
		return (1);
	}
	return (ft_search_builtin_bis(msh));
}
