/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_ter.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:53:39 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/11 14:52:33 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	doublon_handler_bis(t_msh *msh, char *str, t_env *tmp, t_intel i)
{
	char	*tmp2;
	int		content_size;

	content_size = ft_strlen(str + (i.name_size + 1));
	if (i.statut == 4)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(NULL, sizeof(char) * (content_size + 1)),
				MLC, ENV, msh);
		if (msh->status == 255)
			return (255);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		ft_strlcpy(tmp->content, str + (i.name_size + 1), content_size + 1);
	}
	if (i.statut == 5)
	{
		tmp2 = tmp->content;
		tmp->content = ft_exstrjoin(msh, tmp->content, str + i.name_size + 1);
		if (msh->status == 255)
			return (255);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
	}
	return (0);
}

static int	doublon_handler_ter(t_env *t, char *str, t_intel i, t_msh *m)
{
	char	*tmp2;
	
	tmp2 = t->content;
	t->content = ft_strjoin2(t->content, str + i.name_size
					+ 1, m);
	if (m->status == 255)
			return (255);
	mcgic(mlcp(tmp2, 0), FREE, ENV, m);
	return (0);
}

static int	doublon_handler_for(t_env *t, int c, t_msh *m)
{
	char	*tmp2;
	
	tmp2 = t->content;
	t->content = mcgic(mlcp(NULL, sizeof(char) * (c + 2 + 1)), MLC, ENV, m);
	if (m->status == 255)
		return (255);
	mcgic(mlcp(tmp2, 0), FREE, ENV, m);
	return (0);
}

int	doublon_handler(t_msh *m, char *str, t_env **env, t_intel i)
{
	t_env	*t;
	int		c;

	t = *env;
	c = ft_strlen(str + (i.name_size + 1));
	while (ft_strncmp(str, t->name, i.name_size))
		t = t->next;
	if (i.statut == 2)
		return (0);
	t->print = 1;
	if (i.statut == 3)
	{
		if(doublon_handler_for(t, c, m) != 0)
			return (255);
		ft_exstrlcpy(t->content, str + i.name_size, c + 2 + 1);
	}
	if(doublon_handler_bis(m, str, t, i) != 0)
		return (255);
	if (i.statut == 6)
	{
		if(doublon_handler_ter(t, str, i, m) != 0)
			return (255);
	}
	return (0);
}
