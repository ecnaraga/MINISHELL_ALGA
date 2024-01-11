/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_lst_bis.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:56:04 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:54:47 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	del_two(t_msh *msh, t_split *lst)
{
	lst->data = mcgic(mlcp(lst->data, 0), FREE, NO_ENV, msh);
	if (lst->type)
		lst->type = mcgic(mlcp(lst->type, 0), FREE, NO_ENV, msh);
}

void	ft_lstdelone_split(t_msh *msh, t_split *lst, void (*del)(t_msh *msh,
			t_split *))
{
	if (!lst || !del)
		return ;
	del(msh, lst);
	lst = mcgic(mlcp(lst, 0), FREE, NO_ENV, msh);
}

t_split	*lstdel_relink_split(t_msh *msh, t_split *av, t_split *prev,
		t_split **head)
{
	t_split	*tmp;

	if (!av)
		return (NULL);
	if (prev != NULL)
		prev->next = av->next;
	else
		*head = av->next;
	tmp = av->next;
	ft_lstdelone_split(msh, av, del_two);
	return (tmp);
}
