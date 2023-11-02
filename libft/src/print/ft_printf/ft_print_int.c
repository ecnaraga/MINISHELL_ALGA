/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 11:31:34 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:32:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static int	ft_print_int(void *n, t_aglist *ag)
{
	unsigned int	nb;
	int				count;
	unsigned int	tmp;

	nb = *(int *)n;
	count = 0;
	if (*(int *)n < 0)
		nb = -*(int *)n;
	if (nb < 10)
	{
		tmp = nb + 48;
		return (ft_print_char(&tmp) + count);
	}
	else
	{
		tmp = (nb / 10);
		count += ft_print_int(&tmp, ag);
		tmp = (nb % 10) + 48;
		return (ft_print_char(&tmp) + count);
	}
}

int	print_int(t_aglist *ag)
{
	int				count;
	unsigned int	n;
	int				id;

	n = *(int *)(&ag->content);
	ag->len = 0;
	id = 2;
	count = 1;
	if (*(int *)(&ag->content) < 0)
	{
		n = -(*(int *)(&ag->content));
		id = 0;
	}
	digit_len(&n, &count, 10);
	if (n == 0 && ck_char(ag->flag, '.') > 0 && ag->prec == 0)
		count = 0;
	if (ck_sset(ag->flag, "+ ") > -1 || id == 0)
		count += 1;
	ag->len += count;
	count = ck_front_flag(ag, count, id);
	if (!(n == 0 && ck_char(ag->flag, '.') > 0 && ag->prec == 0))
		ft_print_int(&ag->content, ag);
	count = ck_back_flag(ag, count);
	return (count);
}
