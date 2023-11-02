/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_addback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 11:02:55 by garance           #+#    #+#             */
/*   Updated: 2023/07/20 10:21:22 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	lst_two_addback(t_cont *cont, t_lst_two *new)
{
	if (cont->first == NULL)
	{
		cont->first = new;
		cont->first->prev = NULL;
		cont->first->next = NULL;
	}
	else if (cont->first->next == NULL)
	{
		new->prev = cont->first;
		cont->first->next = new;
	}
	else
	{
		new->prev = cont->last;
		cont->last->next = new;
	}
	cont->last = new;
	cont->size += 1;
}
