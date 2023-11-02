/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 17:54:47 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:33:03 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static int	print_longlowhexa(long unsigned int n, t_aglist *ag, int y)
{
	long unsigned int	nb;
	int					x;
	char				*base;

	nb = n;
	base = "0123456789abcdef";
	(void) y;
	if (nb < 16)
		return (ft_print_char(&base[nb]));
	else
	{
		x = print_longlowhexa(nb / 16, ag, 0);
		ft_print_char(&base[nb % 16]);
		return (x + 1);
	}
}

static void	ptr_len(uintptr_t n, int *count, unsigned int base)
{
	while (n >= base)
	{
		*count += 1;
		n = n / base;
	}
}

int	print_ptr(t_aglist *ag)
{
	int			count;
	int			id;
	uintptr_t	n;

	count = 3;
	id = 5;
	n = (uintptr_t)ag->content;
	if (n != 0)
		ptr_len(n, &count, 16);
	if (n == 0)
		count = 5;
	if (ck_sset(ag->flag, "+ ") > -1 && ag->content)
		count += 1;
	ag->len = count;
	if (ck_char(ag->flag, '.') > 0)
		ag->prec += 2;
	count = ck_front_flag(ag, count, id);
	if (n != 0)
		print_longlowhexa((uintptr_t)ag->content, ag, 1);
	else
		write(1, "(nil)", 5);
	count = ck_back_flag(ag, count);
	return (count);
}
