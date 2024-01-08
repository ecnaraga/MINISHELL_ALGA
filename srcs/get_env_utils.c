/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:43 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/08 14:21:08 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

int	ft_lstsize_env(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count ++;
		lst = lst->next;
	}
	return (count);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (ft_lstsize_env(*lst) == 0)
	{
		*lst = new;
	}
	else
	{
		temp = ft_lstlast_env(*lst);
		temp->next = new;
	}
}

t_env	*ft_lst_new_malloc(t_msh *msh, size_t size, size_t size2)
{
	t_env	*temp;

	temp = NULL;
	temp = mcgic(mlcp(NULL, sizeof(t_env)), MLC, ENV, msh);
	if (temp == NULL)
		return (NULL);
	temp->name = mcgic(mlcp(ft_calloc(sizeof(char), size), 1), ADD, ENV, msh);
	if (!temp->name)
		return (NULL);
	temp->content = mcgic(mlcp(ft_calloc(sizeof(char),
					size2), 1), ADD, ENV, msh);
	if (!temp->content)
		return (NULL);
	temp->next = NULL;
	temp->read = 0;
	return (temp);
}

int	check_env(char **str)
{
	if (!str || str[0] == NULL)
		return (1);
	else
		return (0);
}
