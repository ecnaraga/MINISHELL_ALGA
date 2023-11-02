/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 11:04:17 by garance           #+#    #+#             */
/*   Updated: 2023/08/31 13:00:13 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	lst_two_size(t_cont *cont)
{
	t_lst_two	*lst;
	int			count;

	lst = cont->first;
	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
