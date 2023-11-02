/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_addfront.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 15:44:53 by galambey          #+#    #+#             */
/*   Updated: 2023/08/31 12:59:38 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	lst_two_addfront(t_cont *cont, t_lst_two *new)
{
	if (cont->last == NULL)
	{
		cont->last = new;
	}
	else if (cont->last->next == NULL)
	{
		new->next = cont->last;
		cont->last->prev = new;
	}
	else
	{
		new->next = cont->first;
		cont->first->prev = new;
	}
	cont->first = new;
}
