/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_make_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:13:19 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 16:06:32 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_realloc_cmd(t_msh *msh, int *cmd_nb, int *i, char **tmp)
{
	char	**cmd;
	int		j;
	int		k;

	cmd = mcgic(mlcp(NULL, sizeof(char *) * (*cmd_nb + 1)), MLC, PIP, msh);
	if (!cmd)
		return (NULL);
	j = 0;
	while (j < *i)
	{
		cmd[j] = msh->p.cmd_t[j];
		j++;
	}
	if (!tmp)
	{
		tmp = ft_split_isspace_magic_malloc(msh, msh->p.cmd_t[j]);
		if (!tmp)
			return (NULL);
	}
	k = 0;
	while (tmp[k])
		cmd[j++] = tmp[k++];
	cmd[j] = NULL;
	mcgic(mlcp(msh->p.cmd_t, 0), FREE, PIP, msh);
	return (*i = j - 1, mcgic(mlcp(tmp, 0), FREE, PIP, msh), cmd);
}

char	**ft_handle_expand(t_msh *msh, int *i, int *cmd_nb)
{
	int	c_wd;

	if (*i == 0)
		msh->p.cmd_t[*i] = ft_expand(msh, msh->p.cmd_t[*i], CMD);
	else
		msh->p.cmd_t[*i] = ft_expand(msh, msh->p.cmd_t[*i], OTHER);
	if (msh->status == 255)
		return (NULL);
	if (msh->av->quote && !msh->p.cmd_t[0][0])
		write(2, "minishell: : command not found\n", 32);
	c_wd = ft_countword(msh->p.cmd_t[*i]);
	if ((c_wd > 1 && !msh->p.cmd_t[0]) || (c_wd > 1
			&& ft_strcmp(msh->p.cmd_t[0], "export")))
	{
		*cmd_nb = *cmd_nb + c_wd;
		msh->p.cmd_t = ft_realloc_cmd(msh, cmd_nb, i, NULL);
		if (!msh->p.cmd_t)
			return (NULL);
	}
	return (msh->p.cmd_t);
}

static void	ft_set_wild(t_msh *msh)
{
	int		i;
	t_quote	q;

	i = -1;
	q.s = 0;
	q.d = 0;
	while (msh->av->data[++i])
	{
		ft_inc_quote(msh->av->data[i], &q.d, &q.s);
		if (q.s % 2 == 1 || q.d % 2 == 1 || msh->av->data[i] != '*')
			msh->av->wild[i] = 1;
		else
			msh->av->wild[i] = 0;
	}
}

void	ft_reset_msh_av(t_msh *msh, char *data)
{
	char	*tmp;

	tmp = msh->av->data;
	msh->av->data = mcgic(mlcp(ft_strdup(data), -1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return ;
	msh->av->wild = mcgic(mlcp(NULL, sizeof(int) * ft_strlen(data)), MLC,
			NO_ENV, msh);
	if (msh->status == 255)
		return ;
	ft_set_wild(msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(msh->av->type, 0), FREE, NO_ENV, msh);
	msh->av->type = NULL;
}
