/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 16:40:39 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/11 15:04:16 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_sizetab(char **tmp)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (tmp[++i])
		count += 1;
	return (count);
}

char	**ft_expand_wildcard(t_msh *msh, int *cmd_nb, int *i)
{
	char	**tmp;

	if (msh->p.cmd_t[0])
	{
		tmp = wildcards(msh->av->data, msh, msh->p.cmd_t[0], msh->p.cmd_t[1]);
	}
	else
		tmp = wildcards(msh->av->data, msh, msh->av->data, NULL);
	if (msh->status == 255)
		return (NULL);
	if (!tmp)
	{
		msh->p.cmd_t[*i] = mcgic(mlcp(ft_strdup(msh->av->data), 1), ADD, PIP,
				msh);
		if (!msh->p.cmd_t[*i])
			return (NULL);
		return (msh->p.cmd_t);
	}
	*cmd_nb += ft_sizetab(tmp);
	return (ft_realloc_cmd(msh, cmd_nb, i, tmp));
}
