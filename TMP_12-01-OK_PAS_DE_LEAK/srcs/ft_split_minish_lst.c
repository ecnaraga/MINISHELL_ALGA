/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_lst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:56:04 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:54:53 by galambey         ###   ########.fr       */
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

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
