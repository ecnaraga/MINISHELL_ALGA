/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magic_malloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:18:58 by galambey          #+#    #+#             */
/*   Updated: 2023/12/01 13:32:15 by galambey         ###   ########.fr       */
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
		return (write(2, "minishell: Cannot allocate memory\n", 35), status = 255, NULL);
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
/*
Garbagge collector : Store in a linked list all the address malloc
	mlc_env : Contains malloc addresses containing the environment
	mlc : Contains the other malloc addresses in the program
5 rules:
	- Malloc : malloc, adds the address to the linked list and returns the
		address of the malloc element.
	- Add : adds a malloc element to the linked list and returns it
	- Free : free a malloc element and remove it from the linked list
	- Flush : free all except elements containing the environment
	- Quit : free all
rule : choice of the rule
size : if the rule asked is malloc, size of the element to malloc
addr : if the rule is add or free, address of the elemnt sent
nb : if the rule is add or malloc, linked list concerning
*/
void	*ft_magic_malloc(int rule, size_t size, void *addr, int nb)
{
	static t_list	*mlc;
	static t_list	*mlc_env;

	if ((rule == MALLOC || rule == ADD) && nb == NO_ENV)
		return (ft_magic_add_malloc(&mlc, rule, size, addr));
	else if ((rule == MALLOC || rule == ADD) && nb == ENV)
		return (ft_magic_add_malloc(&mlc_env, rule, size, addr));
	else if (rule == FREE && nb == NO_ENV)
		ft_list_remove_if(&mlc, addr, ft_check);
	else if (rule == FREE && nb == ENV)
		ft_list_remove_if(&mlc_env, addr, ft_check);
	else if (rule == FLUSH && nb == NO_ENV)
		ft_lstclear(&mlc, del);
	else if (rule == FLUSH && nb == ENV)
		ft_lstclear(&mlc_env, del);
	else //QUIT
	{
		ft_lstclear(&mlc, del);
		ft_lstclear(&mlc_env, del);
	}
	return (NULL);
}
