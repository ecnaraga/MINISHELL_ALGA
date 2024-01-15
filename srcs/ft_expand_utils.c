/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 12:00:28 by galambey         ###   ########.fr       */
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

char	*ft_do_expand(t_msh *msh, t_expand *e, char *cmd, int rule)
{
	char	*tmp2;

	if (cmd)
		e->cmd = mcgic(mlcp(ft_strdup(cmd), 1), ADD, NO_ENV, msh);
	else
		e->cmd = NULL;
	e->tmp = get_value(msh, msh->env, e, rule);
	if (cmd)
		mcgic(mlcp(e->cmd, 0), FREE, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	tmp2 = cmd;
	cmd = mcgic(mlcp(ft_strjoin(cmd, e->tmp), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	if (tmp2)
		mcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	return (cmd);
}

char	*ft_expand_valid_var(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	e->tmp2 = e->tmp;
	e->tmp = mcgic(mlcp(ft_substr(e->tmp, 0, e->len), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	mcgic(mlcp(e->tmp2, 0), FREE, NO_ENV, msh);
	cmd = ft_do_expand(msh, e, cmd, rule);
	if (msh->status == 255)
		return (NULL);
	return (cmd);
}
