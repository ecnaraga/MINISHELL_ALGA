/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magic_malloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:18:58 by galambey          #+#    #+#             */
/*   Updated: 2023/11/22 16:20:45 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// typedef struct s_liste
// {
// 	void				*content;
// 	struct s_liste		*next;
// }						t_liste;

void del(void *content)
{
	free(content);
}

int ft_check(void *data, void *data_ref)
{
    return (/**(int *)*/data == /**(int *)*/data_ref);
}

void ft_list_remove_if(t_list **begin_list, void *addr, int (*cmp)())
{
    t_list  *prev;
    t_list  *lst;

    lst = *begin_list;
    prev = NULL;
    while (lst)
    {
        if (cmp(lst->content, addr))
        {
            if (lst->next == NULL)
            {
                if (lst == *begin_list)
                    *begin_list = NULL;
                if (prev)
                    prev->next = NULL;
				printf("FREE mlc->content %p\n", lst->content);
				ft_lstdelone(lst, del);
                // free(lst);
                lst = NULL;
            }
            else if (lst == *begin_list)
            {
                *begin_list = lst->next;
				printf("FREE mlc->content %p\n", lst->content);
				ft_lstdelone(lst, del);
                // free(lst);
                lst = *begin_list;
            }
            else
            {
                prev->next = lst->next;
				printf("FREE mlc->content %p\n", lst->content);
				ft_lstdelone(lst, del);
                // free(lst);
                lst = prev->next;
            }
        }
        else
        {
            prev = lst;
            lst = lst->next;
        }
    }
}

t_list	*ft_lstnew_malloc(size_t size)
{
	t_list	*temp;

	temp = NULL;
	temp = malloc(sizeof(t_list));
	if (temp == NULL)
		return (NULL);
	temp->content = malloc(size);
	temp->next = NULL;
	return (temp);
}

void *ft_magic_malloc(int rule, size_t size, void *addr)
{
	static	t_list *mlc;
	t_list	*head;
	t_list	*tmp;
	int i;
	
	(void) rule;
	(void) size;
	(void) head;
	if (rule == MALLOC || rule == ADD)
	{
		head = mlc;
		if (rule == MALLOC)
			tmp = ft_lstnew_malloc(size);
		// else if (!addr)
		else
			tmp = ft_lstnew(addr);
		// if (!tmp)
		if (mlc)
		{
			while (mlc->next)
				mlc = mlc->next;
			mlc->next = tmp;
			// printf("MALLOC mlc->content %p mlc %p\n", mlc->next->content, mlc);
			mlc = head;
		}
		else
		{
			mlc = tmp;
			// printf("MALLOC mlc->content %p mlc %p\n", mlc->content, mlc);
		}
		return (tmp->content);
	}
	else if (rule == FREE)
		ft_list_remove_if(&mlc, addr, ft_check);
	else if (rule == PRINT)
	{
		head = mlc;
		i = 0;
		while (mlc)
		{
			// printf("PRINT %d %p\n", i, mlc->content);
			i++;
			mlc = mlc->next;
		}
		mlc = head;
	}
	else //rule == FLUSH
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
