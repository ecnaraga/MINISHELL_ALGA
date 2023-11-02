/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:04:35 by galambey          #+#    #+#             */
/*   Updated: 2023/07/17 10:22:07 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	lst_two_clear(t_lst_two **lst, void (*del)(void *))
{
	t_lst_two	*tmp;

	if (!del || !lst || !*lst)
		return ;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		lst_two_delone(*lst, del);
		*lst = tmp;
	}
}
