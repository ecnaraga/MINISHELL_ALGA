/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 16:40:39 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/22 17:56:05 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_sizetab(char **tmp)
{
	int i;
	int count;
	
	i = -1;
	count = 0;
	while (tmp[++i])
		count += 1;
	return (count);
}

char	**ft_expand_wildcard(t_msh *msh, int *cmd_nb, int *i)
{
	char	**tmp;
	char	**cmd;
	int j;
	int k;

	if (msh->p.cmd_opt[0])
		tmp = wildcards(msh->av->data, msh, msh->p.cmd_opt[0]);
	else
		tmp = wildcards(msh->av->data, msh, msh->av->data);
	if (msh->status == 255)
		return (NULL);
	if (!tmp)
	{
		msh->p.cmd_opt[*i] = mlcgic(mlcp(ft_strdup(msh->av->data), 1), ADD, PIP, msh);
		// msh->p.cmd_opt[*i] = ft_magic_malloc(ADD, 0, ft_strdup(msh->av->data), PIP);
		if (!msh->p.cmd_opt[*i])
			return (NULL); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
		return (msh->p.cmd_opt);
	}
	*cmd_nb += ft_sizetab(tmp);
	cmd = mlcgic(mlcp(NULL, sizeof(char *) * (*cmd_nb + 1)), MALLOC, PIP, msh);
	if (!cmd)
		return (NULL); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	j = 0;
	while (j < *i)
	{
		cmd[j] = msh->p.cmd_opt[j];
		j++;
	}
	k = 0;
	while (tmp[k])
		cmd[j++] = tmp[k++];
	cmd[j] = NULL; // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	*i = j - 1;
	mlcgic(mlcp(msh->p.cmd_opt, 0), FREE, PIP, msh);
	mlcgic(mlcp(tmp, 0), FREE, PIP, msh);
	return (cmd);
}