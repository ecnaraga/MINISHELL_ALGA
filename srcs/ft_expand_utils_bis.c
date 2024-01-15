/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_utils_bis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:10:17 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_value_other(t_msh *msh, t_env *node, char *str)
{
	if (!str)
		return (mcgic(mlcp(ft_strtrim_except_tips(node->content,
						" \b\t\n\v\f\r", 2), 1), ADD, NO_ENV, msh));
	return (mcgic(mlcp(ft_strtrim_except_tips(node->content,
					" \b\t\n\v\f\r", 0), 1), ADD, NO_ENV, msh));
}

static char	*get_value_cmd(t_msh *msh, t_env *node, char *str)
{
	if (!msh->p.cmd_t[0])
	{
		if (msh->first_d == 0 && msh->av->dollar == 1 && !str)
		{
			msh->first_d = 1;
			return (mcgic(mlcp(ft_strtrim(node->content, " \b\t\n\v\f\r"),
						1), ADD, NO_ENV, msh));
		}
		else if (msh->first_d == 0 && !str)
		{
			msh->first_d = 1;
			return (mcgic(mlcp(ft_strtrim_except_tips(node->content,
							" \b\t\n\v\f\r", 2), 1), ADD, NO_ENV, msh));
		}
		else
		{
			return (mcgic(mlcp(ft_strtrim_except_tips(node->content,
							" \b\t\n\v\f\r", 0), 1), ADD, NO_ENV, msh));
		}
	}
	return (mcgic(mlcp(ft_strtrim(node->content, " \b\t\n\v\f\r"),
				1), ADD, NO_ENV, msh));
}

char	*get_value(t_msh *msh, t_env **env, t_expand *e, int rule)
{
	t_env	*node;

	node = *env;
	while (node)
	{
		if (strcmp(node->name, e->tmp) == 0)
		{
			mcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
			if (rule == CMD)
				return (get_value_cmd(msh, node, e->cmd));
			if (rule == OTHER)
				return (get_value_other(msh, node, e->cmd));
			if (rule == INFILE || rule == OUTFILE_NO_TRUNC
				|| rule == OUTFILE_TRUNC || rule == HDOC)
				return (mcgic(mlcp(ft_strdup(node->content), 1), ADD, NO_ENV,
						msh));
		}
		else
			node = node->next;
	}
	mcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	return (mcgic(mlcp(ft_strdup(""), 1), ADD, NO_ENV, msh));
}
