/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 12:11:00 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/04 14:06:01 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(int *i, int *s, int *count, long long *result)
{
	*i = 0;
	*s = 1;
	*result = 0;
	*count = 0;
}

long long	ft_atol(char *nptr, int *err)
{
	int					i;
	int					s;
	int					count;
	long long			result;
	long long unsigned	res;

	ft_init_var(&i, &s, &count, &result);
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
		*err = 1;
	result = (long long)res;
	return (result * s);
}
