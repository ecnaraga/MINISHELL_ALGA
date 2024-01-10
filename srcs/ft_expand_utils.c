/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 10:59:26 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Return 0 if all char in key is valid
Otherwise return the index of the first non valid char
*/
int	valide_expand(char *key)
{
	int	i;

	i = 0;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		while (key[i])
		{
			if (ft_isalnum(key[i]) || key[i] == '_')
				i++;
			else
				return (i);
		}
		return (0);
	}
	return (-1);
}

char	*get_value(t_msh *msh, t_env **env, char *str, int rule)
{
	t_env	*node;

	node = *env;
	while (node)
	{
		if (strcmp(node->name, str) == 0)
		{
			mcgic(mlcp(str, 0), FREE, NO_ENV, msh);
			if (rule == CMD)
				return (mcgic(mlcp(ft_strtrim(node->content, " \b\t\n\v\f\r"),
							1), ADD, NO_ENV, msh));
			if (rule == OTHER)
				return (mcgic(mlcp(ft_strtrim_except_tips(node->content,
								" \b\t\n\v\f\r"), 1), ADD, NO_ENV, msh));
			if (rule == INFILE || rule == OUTFILE_NO_TRUNC
				|| rule == OUTFILE_TRUNC || rule == HDOC)
				return (mcgic(mlcp(ft_strdup(node->content), 1), ADD, NO_ENV,
						msh));
		}
		else
			node = node->next;
	}
	mcgic(mlcp(str, 0), FREE, NO_ENV, msh);
	return (mcgic(mlcp(ft_strdup(""), 1), ADD, NO_ENV, msh));
}

char	*ft_add_char(t_msh *msh, char *cmd, t_expand *e, int *j)
{
	e->tmp = cmd;
	cmd = mcgic(mlcp(ft_strjoin_char(cmd, msh->av->data[e->i]), 1), ADD, NO_ENV,
			msh);
	if (msh->status == 255)
		return (NULL);
	if (e->tmp)
		mcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	(e->i)++;
	if (j)
		(*j)++;
	return (cmd);
}

char	*ft_do_expand(t_msh *msh, char *tmp, char *cmd, int rule)
{
	char	*tmp2;

	tmp = get_value(msh, msh->env, tmp, rule);
	if (msh->status == 255)
		return (NULL);
	tmp2 = cmd;
	cmd = mcgic(mlcp(ft_strjoin(cmd, tmp), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	if (tmp2)
		mcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	return (cmd);
}

char	*ft_expand_valid_var(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	e->tmp2 = e->tmp;
	e->tmp = mcgic(mlcp(ft_substr(e->tmp, 0, e->len), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	mcgic(mlcp(e->tmp2, 0), FREE, NO_ENV, msh);
	cmd = ft_do_expand(msh, e->tmp, cmd, rule);
	if (msh->status == 255)
		return (NULL);
	return (cmd);
}
