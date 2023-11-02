/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 09:53:34 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:32:48 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

int	ft_print_char(void *c)
{
	char	x;

	x = *(char *)c;
	return (write(1, &x, 1));
}

int	print_char(t_aglist *ag)
{
	int	count;
	int	id;

	count = 1;
	id = 6;
	count = ck_front_flag(ag, count, id);
	ft_print_char(&ag->content);
	count = ck_back_flag(ag, count);
	return (count);
}
