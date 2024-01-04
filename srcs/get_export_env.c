/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_export_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:10:17 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/04 16:54:26 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exstrlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (src[j] == '=')
		j++;
	//printf("%c\n", src[j]);
	if (size > 0)
	{	
		dst[i++] = '"';
		while (src[j] && j < (size - 1))
		{
			dst[i] = src[j];
			i++;
			j++;
		}
		dst[i++] = '"';
		dst[i] = '\0';
	}
}

int	get_name_size(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '=' || (str[i] == '+' && str[i + 1] == '='))
			return (i);
	}
	return (i);
}

static int	fill_export_env(t_msh *msh, t_env **export_env, char **str)
{
	int		i;
	int		name_size;
	int		content_size;
	t_env	*new;

	i = -1;
	new = NULL;
	if (check_env(export_env, str) == 1)
		return (0);
	while (str[++i])
	{
		name_size = get_name_size(str[i]);
		content_size = ft_strlen(str[i] + (name_size + 1));
		new = ft_lst_new_malloc(msh, name_size + 1, content_size + 2 + 1);
		if (!new)
		{
			mcgic(NULL, FLUSH, ENV, msh);
			// ft_magic_malloc(FLUSH, 0, NULL, ENV);
			return (1);
		}
		ft_strlcpy(new->name, str[i], name_size + 1);
		ft_exstrlcpy(new->content, str[i] + (name_size + 1), content_size + 2 + 1);
		ft_lstadd_back_env(export_env, new);
	}
	return (0);
}

void	order_export_env(t_env **export_env)
{
	t_env	*node;
	char	*tmp;

	if (!*export_env)
		return ;
	node = *export_env;
	while (node->next)
	{
		if (strcmp(node->name, node->next->name) > 0)
		{
			tmp = node->next->name;
			node->next->name = node->name;
			node->name = tmp;
			tmp = node->next->content;
			node->next->content = node->content;
			node->content = tmp;
			node = *export_env;
		}
		else
			node = node->next;
	}	
}

t_env	**get_export_env(t_msh *msh, char **str)
{
	t_env	**export_env;

	export_env = mcgic(mlcp(NULL, sizeof(t_env)), MLC, ENV, msh);
	// export_env = ft_magic_malloc(MLC, sizeof(t_env), NULL, ENV);
	if (!export_env)
		return (NULL);
	*export_env = NULL;
	if (fill_export_env(msh, export_env, str) == 1)
		return (NULL);
	order_export_env(export_env);
	return (export_env);
}
