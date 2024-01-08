/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_ter.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:53:39 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/08 13:57:04 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	doublon_handler_bis(t_msh *msh, char *str, t_env *tmp, t_intel i)
{
	char	*tmp2;
	int		content_size;

	content_size = ft_strlen(str + (i.name_size + 1));
	if (i.statut == 4)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(NULL, sizeof(char) * (content_size + 1)),
				MLC, ENV, msh);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		ft_strlcpy(tmp->content, str + (i.name_size + 1), content_size + 1);
	}
	if (i.statut == 5)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(ft_exstrjoin(msh, tmp->content, str
						+ i.name_size + 1), 1), ADD, ENV, msh);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
	}
}

void	doublon_handler(t_msh *m, char *str, t_env **env, t_intel i)
{
	t_env	*t;
	char	*tmp2;
	int		c;

	t = *env;
	c = ft_strlen(str + (i.name_size + 1));
	while (ft_strncmp(str, t->name, i.name_size))
		t = t->next;
	if (i.statut == 2)
		return ;
	if (i.statut == 3)
	{
		tmp2 = t->content;
		t->content = mcgic(mlcp(NULL, sizeof(char) * (c + 2 + 1)), MLC, ENV, m);
		mcgic(mlcp(tmp2, 0), FREE, ENV, m);
		ft_exstrlcpy(t->content, str + i.name_size, c + 2 + 1);
	}
	doublon_handler_bis(m, str, t, i);
	if (i.statut == 6)
	{
		tmp2 = t->content;
		t->content = mcgic(mlcp(ft_strjoin2(t->content, str + i.name_size
						+ 1), 1), ADD, ENV, m);
		mcgic(mlcp(tmp2, 0), FREE, ENV, m);
	}
}
