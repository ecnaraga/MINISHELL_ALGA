/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 10:37:46 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/19 12:33:28 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

long long	ft_atol(char *nptr, int *err)
{
	int	i;
	int	s;
	long long	result;
	long long unsigned	res;
	int count;

	i = 0;
	s = 1;
	result = 0;
	count = 0;
	res = 0;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			s = -s;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - '0';
		i++;
		count++;
	}
	if (count > 19 || res > 9223372036854775807)
	{
		dprintf(2, "OVERFLOW");
		*err = 1;
	}
	result = (long long) res;
	return (result * s);
}

// static int	is_nbr(char *str)
// {
// 	int	i;
// 	char	*tmp;

// 	i = 0;
// 	while(str[i])
// 	{
// 		if (str[i] <= '0' || str[i] >= '9')
// 			return (0);
// 		i++;
// 	}
// 	tmp = ft_magic_malloc(ADD, 0, ft_itoa(ft_atol(str)), PIP);
// 	if (strcmp(str, tmp))
// 	{
// 		ft_magic_malloc(FREE, 0, tmp, PIP);
// 		return (0);
// 	}
// 	ft_magic_malloc(FREE, 0, tmp, PIP);
// 	return (1);
// }

static int	is_nbr(char *str)
{
	int	i;
	int		err;

	i = 0;
	err = 0;
	while(str[i])
	{
		if (str[i] <= '0' || str[i] >= '9')
			return (0);
		i++;
	}
	ft_atol(str, &err);
	if (err == 1)
		return (0);
	return (1);
}

static int	args_handler(char **args)
{
	char	*tmp;
	char	*tmp2;

	if (!is_nbr(args[1]))
	{
		printf("exit\n");
		tmp = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: exit: ", args[1]), NO_ENV);
		if (status == 255)
			return (255);
		tmp2 = ft_magic_malloc(ADD, 0, ft_strjoin(tmp, ": numeric argument required\n"), NO_ENV);
		if (status == 255)
			return (255);
		ft_magic_malloc(FREE, 0, tmp, NO_ENV);
		ft_putstr_fd(tmp2, 2);
		ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
		return (status = 2, 1);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (status = 1, 2);
	}
	else
	{
		printf("exit\n");
		return(status = ft_atol(args[1], NULL) % 256, 1);
	}
		
}

int	builtin_exit(t_msh *minish)
{
	int	statut;

	statut = 0;
	if(minish->p.cmd_opt[1])
		return(args_handler(minish->p.cmd_opt));
	else
	{
		printf("exit\n");
		return(status = 0, 1);
		
	}
}