/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:43 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/18 14:28:18 by galambey         ###   ########.fr       */
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

t_env	*ft_lst_new_malloc(size_t size, size_t size2)
{
	t_env	*temp;

	temp = NULL;
	temp = ft_magic_malloc(MALLOC, sizeof(t_env), NULL, ENV);
	if (temp == NULL)
		return (NULL);
	temp->name = ft_magic_malloc(ADD, 0, ft_calloc(sizeof(char), size), ENV);
	if (!temp->name)
		return (NULL);
	temp->content = ft_magic_malloc(ADD, 0, ft_calloc(sizeof(char), size2), ENV);
	if (!temp->content)
		return (NULL);
	temp->next = NULL;
	temp->read = 0;
	return (temp);
}

int	check_env(t_env **env, char **str)
{
	(void) env;
	if (!str || str[0] == NULL)
	{
		// ft_magic_malloc(FREE, 0, env, ENV);
		return (1);
	}
	else
		return (0);
}
