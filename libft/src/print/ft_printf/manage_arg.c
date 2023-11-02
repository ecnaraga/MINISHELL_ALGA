/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 09:59:13 by garance           #+#    #+#             */
/*   Updated: 2023/06/23 16:33:30 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

int	ft_free_arg(t_aglist *ag, int result)
{
	if (ag->flag)
		free(ag->flag);
	if (ag)
		free(ag);
	return (result);
}

static void	ft_orgflag(char *flag, char *str, size_t size)
{
	int	i;
	int	f;

	i = -1;
	f = 0;
	ft_strlcpy(flag, str, size);
	while (flag[++i])
	{
		if (flag[i] == '-')
			f = 1;
	}
	i = -1;
	while (f == 1 && flag[++i])
	{
		if (flag[i] == '0')
			flag[i] = 'v';
	}
}

static int	ft_make_flag_point(t_aglist *ag, char *s, int *i)
{
	if (s[*i] == '.')
	{
		ft_strcat(ag->flag, ".");
		*i += 1;
	}
	ag->prec = ft_atoi_printf(s + *i);
	if (ag->prec == -2)
		return (ft_free_arg(ag, -2));
	if (ck_char(ag->flag, '.') > 0 && ag->prec == -1)
		ag->prec = 0;
	while (s[*i] >= '0' && s[*i] <= '9')
		*i += 1;
	return (0);
}

int	ft_make_arg(t_aglist *ag, char *s, int *i)
{
	int	result;

	while (ck_char("-0# +", s[*i]) > 0)
		*i += 1;
	ag->flag = ft_calloc(*i + 2, sizeof(char));
	if (ag->flag == NULL)
		return (ft_free_arg(ag, -2));
	ft_orgflag(ag->flag, s, *i + 1);
	ag->nb = ft_atoi_printf(s + *i);
	while (s[*i] >= '0' && s[*i] <= '9')
		*i += 1;
	if (ag->nb == -2)
		return (ft_free_arg(ag, -2));
	result = ft_make_flag_point(ag, s, i);
	if (result == -2)
		return (-2);
	if (s[*i] == '%')
		return (ft_free_arg(ag, *i));
	return (*i);
}
