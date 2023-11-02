/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_two_delone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:02:12 by garance           #+#    #+#             */
/*   Updated: 2023/07/16 12:02:16 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	lst_two_delone(t_lst_two *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->data);
	free(lst);
}
