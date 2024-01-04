/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:56:04 by garance           #+#    #+#             */
/*   Updated: 2024/01/04 17:23:24 by galambey         ###   ########.fr       */
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

t_split	*ft_lstnew_split(t_msh *msh, t_quote q)
{
	t_split	*temp;

	temp = NULL;
	temp = mcgic(mlcp(NULL, sizeof(t_split)), MLC, NO_ENV, msh);
	if (temp == NULL)
		ft_exit(-1, -1, -1, msh);
	temp->dollar = q.dollar;
	temp->wildcard = q.wildcard;
	temp->type = NULL;
	temp->next = NULL;
	return (temp);
}

int	ft_lstsize_split(t_split *lst)
{
	int	count;

    // printf("LSTSIZE\n");
	count = 0;
	while (lst)
	{
		count ++;
		lst = lst -> next;
	}
	return (count);
}

void    del_two(t_msh *msh, t_split *lst)
{
	lst->data = mcgic(mlcp(lst->data, 0), FREE, NO_ENV, msh);
    // lst->data = ft_magic_malloc(FREE, 0, lst->data, NO_ENV);
    if (lst->type)
		lst->type = mcgic(mlcp(lst->type, 0), FREE, NO_ENV, msh);
        // lst->type = ft_magic_malloc(FREE, 0, lst->type, NO_ENV);
}

void	ft_lstdelone_split(t_msh *msh, t_split *lst, void (*del)(t_msh *msh, t_split *))
{
	if (!lst || !del)
		return ;
	del_two(msh, lst);
	lst = mcgic(mlcp(lst, 0), FREE, NO_ENV, msh);
	// lst = ft_magic_malloc(FREE, 0, lst, NO_ENV);
}

// void    del_three(t_env *lst)
// {
// 	lst->name = mcgic(mlcp(lst->name, 0), FREE, NO_ENV, msh);
//     // lst->name = ft_magic_malloc(FREE, 0, lst->name, NO_ENV);
// 	lst->content = mcgic(mlcp(lst->content, 0), FREE, NO_ENV, msh);
//     // lst->content = ft_magic_malloc(FREE, 0, lst->content, NO_ENV);
// }

// void	ft_lstdelone_magic(t_env*lst, void (*del)(t_env*))
// {
// 	if (!lst || !del)
// 		return ;
// 	del_three(lst);
// 	lst = mcgic(mlcp(lst, 0), FREE, NO_ENV, msh);
// 	// lst = ft_magic_malloc(FREE, 0, lst, NO_ENV);
// }

t_split	*lstdel_relink_split(t_msh *msh, t_split *av, t_split *prev, t_split **head)
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

// t_env	*ft_lstdel_and_relink(t_msh *msh, t_env *lst, t_env *prev, t_env **head)
// {
// 	t_env	*tmp;
	
// 	if (!lst)
// 		return (NULL);
// 	if (prev != NULL)
// 		prev->next = lst->next;
// 	else
// 		*head = lst->next;
// 	tmp = lst->next;
// 	ft_lstdelone_magic(msh, lst, del_three);
// 	return (tmp);
// }
