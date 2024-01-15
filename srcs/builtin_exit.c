/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 10:37:46 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/15 10:56:56 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_nbr(char *str)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	ft_atol(str, &err);
	if (err == 1)
		return (0);
	return (1);
}

static void	args_handler_bis(t_msh *msh, char **args)
{
	char	*tmp;
	char	*tmp2;

	tmp = mcgic(mlcp(ft_strjoin("minishell: exit: ", args[1]), 1), ADD,
			NO_ENV, msh);
	if (msh->status == 255)
		return ;
	tmp2 = mcgic(mlcp(ft_strjoin(tmp, ": numeric argument required\n"), 1),
			ADD, NO_ENV, msh);
	if (msh->status == 255)
		return ;
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	ft_putstr_fd(tmp2, 2);
	mcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
}

static int	args_handler(t_msh *msh, char **args)
{
	if (!is_nbr(args[1]))
	{
		args_handler_bis(msh, args);
		if (msh->status == 255)
			return (255);
		return (msh->status = 2, 1);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (msh->status = 1, 2);
	}
	else
	{
		printf("exit\n");
		return (msh->status = ft_atol(args[1], NULL) % 256, 1);
	}
}

int	builtin_exit(t_msh *msh)
{
	if (msh->p.cmd_t[1])
		return (args_handler(msh, msh->p.cmd_t));
	else
	{
		printf("exit\n");
		return (msh->status = 0, 1);
	}
}
