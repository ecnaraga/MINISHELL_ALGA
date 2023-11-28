/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:56:04 by garance           #+#    #+#             */
/*   Updated: 2023/11/28 11:24:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_split	*ft_lstlast_split(t_split *lst)
{
	t_split	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	ft_lstadd_back_split(t_split **lst, t_split *new)
{
	t_split	*temp;

	if (!*lst)
		*lst = new;
	else
	{
		temp = ft_lstlast_split(*lst);
		temp->next = new;
	}
}

t_split	*ft_lstnew_split(void)
{
	t_split	*temp;

	temp = NULL;
	temp = ft_magic_malloc(MALLOC, sizeof(t_split), NULL, NO_ENV);
	if (temp == NULL)
		return (NULL);
	temp->dollar = 0;
	temp->type = NULL;
	temp->next = NULL;
	return (temp);
}

int	ft_lstsize_split(t_split *lst)
{
	int	count;

    printf("LSTSIZE\n");
	count = 0;
	while (lst)
	{
		count ++;
		lst = lst -> next;
	}
	return (count);
}

void    del_two(t_split *lst)
{
    ft_magic_malloc(FREE, 0, lst->data, NO_ENV);
    if (lst->type)
        ft_magic_malloc(FREE, 0, lst->type, NO_ENV);
}

void	ft_lstdelone_split(t_split *lst, void (*del)(t_split *))
{
	if (!lst || !del)
		return ;
	del_two(lst);
	ft_magic_malloc(FREE, 0, lst, NO_ENV);
}
