/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magic_malloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:18:58 by galambey          #+#    #+#             */
/*   Updated: 2023/11/24 14:13:21 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_list_remove(t_list **begin_list, t_list **lst, t_list **prev)
{
	if ((*lst)->next == NULL)
	{
		if ((*lst) == *begin_list)
			*begin_list = NULL;
		if (*prev)
			(*prev)->next = NULL;
		ft_lstdelone(*lst, del);
		*lst = NULL;
	}
	else if (*lst == *begin_list)
	{
		*begin_list = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = *begin_list;
	}
	else
	{
		(*prev)->next = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = (*prev)->next;
	}
}

void	ft_list_remove_if(t_list **begin_list, void *addr, int (*cmp)())
{
	t_list	*prev;
	t_list	*lst;

	lst = *begin_list;
	prev = NULL;
	while (lst)
	{
		if (cmp(lst->content, addr))
			ft_list_remove(begin_list, &lst, &prev);
		else
		{
			prev = lst;
			lst = lst->next;
		}
	}
}

void	*ft_magic_add_malloc(t_list **mlc, int rule, size_t size, void *addr)
{
	t_list	*head;
	t_list	*tmp;

	head = *mlc;
	if (rule == MALLOC)
		tmp = ft_lstnew_malloc(size);
	else
		tmp = ft_lstnew_add(addr);
	if (!tmp)
		return (ft_putstr_fd("Error malloc\n", 2), status = 130, NULL);
	printf("MALLOC %p\n", tmp->content);
	if (*mlc)
	{
		while ((*mlc)->next)
			(*mlc) = (*mlc)->next;
		(*mlc)->next = tmp;
		*mlc = head;
	}
	else
		*mlc = tmp;
	return (tmp->content);
}

void	*ft_magic_malloc(int rule, size_t size, void *addr)
{
	static t_list	*mlc;
	t_list			*tmp;

	if (rule == MALLOC || rule == ADD)
		return (ft_magic_add_malloc(&mlc, rule, size, addr));
	else if (rule == FREE)
		ft_list_remove_if(&mlc, addr, ft_check);
	else
	{
		while (mlc)
		{
			tmp = mlc->next;
			printf("FLUSH %p\n", mlc->content);
			free(mlc->content);
			free(mlc);
			mlc = tmp;
		}
	}
	return (NULL);
}
