/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:01:37 by garance           #+#    #+#             */
/*   Updated: 2023/08/31 13:01:37 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"
#include <stdio.h>
#include <unistd.h>

t_cont	lst_two_map(t_cont cont, void *(*f)(void *), void (*del)(void *))
{
	t_cont		new_cont;
	t_lst_two	*elem;

	new_cont.first = NULL;
	if (!cont.first)
		return (new_cont);
	while (cont.first)
	{
		elem = lst_two_addnew(f(cont.first->data));
		if (!elem)
			lst_two_clear(&new_cont.first, del);
		lst_two_addback(&new_cont, elem);
		cont.first = cont.first->next;
	}
	return (new_cont);
}
