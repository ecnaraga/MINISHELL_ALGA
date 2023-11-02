/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsignedint.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 13:31:06 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:33:13 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static int	ft_print_unsignedint(void *n)
{
	unsigned int	nb;
	int				count;
	unsigned int	tmp;

	nb = *(unsigned int *)n;
	count = 0;
	if (nb < 10)
	{
		tmp = nb + 48;
		return (ft_print_char(&tmp) + count);
	}
	else
	{
		tmp = nb / 10;
		count += ft_print_unsignedint(&tmp);
		tmp = (nb % 10) + 48;
		return (ft_print_char(&tmp) + count);
	}
}

int	print_unsignedint(t_aglist *ag)
{
	int				count;
	int				id;
	unsigned int	n;

	count = 1;
	id = 1;
	n = *(unsigned int *)&ag->content;
	digit_len(&n, &count, 10);
	ag->len = count;
	if (n == 0 && ck_char(ag->flag, '.') > 0 && ag->prec == 0)
		count = 0;
	count = ck_front_flag(ag, count, id);
	if (!(n == 0 && ck_char(ag->flag, '.') > 0 && ag->prec == 0))
		ft_print_unsignedint(&ag->content);
	count = ck_back_flag(ag, count);
	return (count);
}
