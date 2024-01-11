/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lpid_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 13:20:47 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:44:48 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_lpid	*ft_lpidnew(int content)
{
	t_lpid	*temp;

	temp = NULL;
	temp = malloc(sizeof(t_list));
	if (temp == NULL)
		return (NULL);
	temp->pid = content;
	temp->next = NULL;
	return (temp);
}

static t_lpid	*ft_lpidlast(t_lpid *lst)
{
	t_lpid	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

static int	ft_lpidsize(t_lpid *lst)
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

void	ft_lpidadd_back(t_lpid **lst, t_lpid *new)
{
	t_lpid	*temp;

	if (ft_lpidsize(*lst) == 0)
	{
		*lst = new;
	}
	else
	{
		temp = ft_lpidlast(*lst);
		temp->next = new;
	}
}
