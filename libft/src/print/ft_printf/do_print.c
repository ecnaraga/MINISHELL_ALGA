/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 17:08:26 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:32:38 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static t_print	ft_storage(int c)
{
	t_print	*storage;

	storage = (t_print[128]){
	['c'] = print_char,
	['s'] = print_str,
	['p'] = print_ptr,
	['d'] = print_int,
	['i'] = print_int,
	['u'] = print_unsignedint,
	['x'] = print_lowhexa,
	['X'] = print_upphexa};
	return (storage[c]);
}

int	ft_arg(va_list ap, char *s, int *count)
{
	t_aglist	*ag;
	t_print		print;
	int			j;
	int			i;

	j = 0;
	i = 0;
	ag = (t_aglist *)malloc(sizeof(t_aglist));
	if (!ag)
		return (-2);
	j = ft_make_arg(ag, (char *)s + 1, &i);
	if (j == -2)
		return (j);
	if (s[1 + j] == '%')
		*count += write(1, "%", 1);
	else
	{
		print = ft_storage(s[1 + j]);
		ag->content = va_arg(ap, void *);
		*count += print(ag);
		ft_free_arg(ag, 0);
	}
	return (j);
}
