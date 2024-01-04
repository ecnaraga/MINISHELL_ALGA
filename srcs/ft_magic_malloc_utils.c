/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magic_malloc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:18:58 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 13:26:17 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	del(void *content)
{
	free(content);
	content = NULL;
}

int	ft_check(void *data, void *data_ref)
{
	return (data == data_ref);
}

t_list	*ft_lstnew_malloc(size_t size)
{
	t_list	*temp;

	temp = malloc(sizeof(t_list));
	if (temp == NULL)
		return (NULL);
	temp->content = malloc(size);
	if (temp->content == NULL)
		return (free(temp), temp = NULL);
	temp->next = NULL;
	return (temp);
}

t_list	*ft_lstnew_add(void *addr)
{
	t_list	*temp;

	if (!addr)
		return (NULL);
	temp = malloc(sizeof(t_list));
	if (temp == NULL)
		return (free(addr), addr = NULL);
	temp->content = addr;
	temp->next = NULL;
	return (temp);
}
