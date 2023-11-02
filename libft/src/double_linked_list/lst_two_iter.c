/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_iter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 12:20:26 by galambey          #+#    #+#             */
/*   Updated: 2023/08/31 13:00:43 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	lst_two_iter(t_cont *cont, void (*f)(void *))
{
	t_lst_two	*tmp;

	if (!(cont || cont->first || f))
		return ;
	tmp = cont->first;
	while (cont->first)
	{
		f(cont->first->data);
		cont->first = cont->first->next;
	}
	cont->first = tmp;
}
