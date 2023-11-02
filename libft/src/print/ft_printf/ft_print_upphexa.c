/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_upphexa.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:09:56 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:33:18 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static int	ft_print_upphexa(void *n)
{
	unsigned int	nb;
	int				count;
	char			*base;
	unsigned int	tmp;

	nb = *(unsigned int *)n;
	base = "0123456789ABCDEF";
	if (nb < 16)
		return (ft_print_char(&base[nb]));
	else
	{
		tmp = nb / 16;
		count = ft_print_upphexa(&tmp);
		return (count + ft_print_char(&base[nb % 16]));
	}
}

int	print_upphexa(t_aglist *ag)
{
	int				count;
	int				id;
	unsigned int	n;

	count = 1;
	id = 3;
	n = *(int *)&ag->content;
	digit_len(&n, &count, 16);
	if (n > 0 && ck_char(ag->flag, '#') > 0)
		count += 2;
	ag->len = count;
	if (n == 0 && ck_char(ag->flag, '.') > 0 && ag->prec == 0)
		count = 0;
	count = ck_front_flag(ag, count, id);
	if (!(n == 0 && ck_char(ag->flag, '.') > 0 && ag->prec == 0))
		ft_print_upphexa(&ag->content);
	count = ck_back_flag(ag, count);
	return (count);
}
