/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:32:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/24 14:36:11 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*ft_lstnew_malloc_bis(size_t size)
{
	t_list	*temp;

	temp = NULL;
	temp = ft_magic_malloc(MALLOC, sizeof(t_list), NULL);
	if (temp == NULL)
		return (NULL);
	temp->content = ft_magic_malloc(MALLOC, sizeof(char) * size, NULL);
	if (!temp->content)
		return (NULL);
	temp->next = NULL;
	return (temp);
}

int	fill_env(t_list **env, char **str)
{
	int		i;
	t_list	*new;

	i = 0;
	while (str[i])
	{
		new = ft_lstnew_malloc_bis(ft_strlen(str[i]) + 1);
		if (!new)
			return (1);
		ft_strlcpy(new->content, str[i], ft_strlen(str[i]) + 1);
		ft_lstadd_back(env, new);
		i++;
	}
	return (0);
}

t_list	**get_env(char **str)
{
	t_list	**env;

	env = ft_magic_malloc(MALLOC, sizeof(t_list), NULL);
	if (!env)
		return (NULL);
	*env = NULL;
	if (fill_env(env, str) == 1)
		return (NULL);
	return (env);
}
