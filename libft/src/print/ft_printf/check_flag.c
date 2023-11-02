/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_flag.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:23:14 by garance           #+#    #+#             */
/*   Updated: 2023/06/23 16:35:40 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static void	ft_flag_plus_space_diese(t_aglist *ag, int id, int *zero)
{
	if (ck_char(ag->flag, '#') > 0 && *(int *)&ag->content != 0
		&& (id == 4 || id == 3))
	{
		if (id == 4)
			write (1, "0x", 2);
		if (id == 3)
			write (1, "0X", 2);
		if (ag->prec >= ag->len)
			*zero += 2;
		else if (ag->prec > 0 && ag->prec == ag->len - 1)
		{
			*zero += 1;
			ag->prec += 1;
		}
	}
	else if ((id == 2 || (id == 5 && ag->content))
		&& ck_sset(ag->flag, "+ ") > -1)
	{
		if (ck_char(ag->flag, '+') > 0)
			write(1, "+", 1);
		else if (ck_char(ag->flag, ' ') > 0)
			write(1, " ", 1);
		if (ag->prec >= ag->len)
			*zero += 1;
	}
}

static void	print_minus(t_aglist *ag, int *zero)
{
	write(1, "-", 1);
	if (ag->prec >= ag->len)
		*zero += 1;
}

static void	print_front_space(t_aglist *ag, unsigned int *count, int *zero,
			int id)
{
	unsigned int	tmp;

	tmp = *count;
	while (tmp++ < ag->prec)
			*zero += 1;
	if (ck_char(ag->flag, '-') == 0 && (ck_sset(ag->flag, "0.") == -1
			|| (ag->prec < ag->nb && ag->prec > -1)))
	{
		if (ck_char(ag->flag, '#') > 0 && *(int *)&ag->content != 0
			&& (id == 4 || id == 3) && ag->prec > 0)
		{
			if (ag->prec >= ag->len)
				tmp += 2;
			else if (ag->prec == ag->len - 1)
				tmp += 1;
		}
		if (ag->prec >= ag->len && (((id == 2 || (id == 5 && ag->content))
					&& ck_sset(ag->flag, "+ ") > -1) || id == 0))
			tmp += 1;
		while (tmp++ <= ag->nb)
			*count += write(1, " ", 1);
	}
}

int	ck_front_flag(t_aglist *ag, unsigned int count, int id)
{
	int	zero;

	zero = 0;
	if ((id >= 0 && id <= 4) || (id == 5 && ag->content))
	{
		print_front_space(ag, &count, &zero, id);
		ft_flag_plus_space_diese(ag, id, &zero);
		if (id == 0)
			print_minus(ag, &zero);
		if (id == 5)
			write(1, "0x", 2);
		while ((count < ag->nb && ck_char(ag->flag, '0') > 0 && ag->prec == -1)
			|| (zero-- > 0 && (ag->prec >= ag->len)))
			count += write(1, "0", 1);
	}
	else if (ck_char(ag->flag, '-') == 0)
	{
		while (count < ag->nb)
			count += write(1, " ", 1);
	}
	return (count);
}

int	ck_back_flag(t_aglist *ag, int count)
{
	if (ag->flag || ag->nb > -1)
	{
		if (count < ag->nb && ck_char(ag->flag, '-') > 0)
		{
			while (count < ag->nb)
				count += write(1, " ", 1);
		}
	}
	return (count);
}
