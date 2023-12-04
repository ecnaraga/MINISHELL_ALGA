/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:56:04 by garance           #+#    #+#             */
/*   Updated: 2023/12/02 09:45:41 by garance          ###   ########.fr       */
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
    lst->data = ft_magic_malloc(FREE, 0, lst->data, NO_ENV);
    if (lst->type)
        lst->type = ft_magic_malloc(FREE, 0, lst->type, NO_ENV);
}

void	ft_lstdelone_split(t_split *lst, void (*del)(t_split *))
{
	if (!lst || !del)
		return ;
	del_two(lst);
	lst = ft_magic_malloc(FREE, 0, lst, NO_ENV);
}

void    del_three(t_list *lst)
{
    lst->content = ft_magic_malloc(FREE, 0, lst->content, NO_ENV);
}

void	ft_lstdelone_magic(t_list*lst, void (*del)(t_list*))
{
	if (!lst || !del)
		return ;
	del_three(lst);
	lst = ft_magic_malloc(FREE, 0, lst, NO_ENV);
}

t_split	*ft_lstdel_and_relink_split(t_split *av, t_split *prev, t_split **head)
{
	t_split	*tmp;
	
	if (!av)
		return (NULL);
	if (prev != NULL)
		prev->next = av->next;
	else
		*head = av->next;
	tmp = av->next;
	ft_lstdelone_split(av, del_two);
	return (tmp);
}

t_list	*ft_lstdel_and_relink(t_list *lst, t_list *prev, t_list **head)
{
	t_list	*tmp;
	
	if (!lst)
		return (NULL);
	if (prev != NULL)
		prev->next = lst->next;
	else
		*head = lst->next;
	tmp = lst->next;
	ft_lstdelone_magic(lst, del_three);
	return (tmp);
}