/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_addnew.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 11:02:14 by garance           #+#    #+#             */
/*   Updated: 2023/08/31 13:00:02 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_lst_two	*lst_two_addnew(void *content)
{
	t_lst_two	*new;

	new = (t_lst_two *)malloc(sizeof(t_lst_two));
	if (!new)
		return (NULL);
	new->prev = NULL;
	new->data = content;
	new->next = NULL;
	return (new);
}
