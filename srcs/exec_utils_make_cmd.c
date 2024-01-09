/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_make_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:13:19 by galambey          #+#    #+#             */
/*   Updated: 2024/01/06 10:27:43 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Count the numbers of elements that we will put in the cmd (cmd + option +
	arguments)
*/
int		ft_count_cmd(t_msh *msh)
{
	t_split	*head;
	int		count;

	head = msh->av;
	count = 0;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR)
	{
		if (msh->av->token == CMD)
			count++;
		msh->av = msh->av->next;
	}
	msh->av = head;
	if (count == 0)
		return (-1);
	return (count);
}

char **ft_realloc_cmd(t_msh *msh, int *cmd_nb, int *i, char **tmp)
{
	char 	**cmd;
	int		j;
	int		k;

	cmd = mcgic(mlcp(NULL, sizeof(char *) * (*cmd_nb + 1)), MLC, PIP, msh);
	if (!cmd)
		return (NULL); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
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
			return (NULL); // IF ERREUR MALLOC DANS FT_SPLIT_ISSPACE_MAGIC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	}
	k = 0;
	while (tmp[k])
		cmd[j++] = tmp[k++];
	cmd[j] = NULL; // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	mcgic(mlcp(msh->p.cmd_t, 0), FREE, PIP, msh);
	return (*i = j - 1, mcgic(mlcp(tmp, 0), FREE, PIP, msh), cmd);
}

char	**ft_handle_expand(t_msh *msh, int *i, int *cmd_nb)
{
	int	c_wd;
	
	if (*i == 0)
		msh->p.cmd_t[*i] = ft_expand(msh, msh->p.cmd_t[*i], CMD); // IF ERROR MLC, EXPAND RETURN (NULL)
	else
		msh->p.cmd_t[*i] = ft_expand(msh, msh->p.cmd_t[*i], OTHER); // IF ERROR MLC, EXPAND RETURN (NULL)
	if (msh->status == 255)
		return (NULL); // IF ERREUR MALLOC DANS EXPAND ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	if (msh->av->quote && !msh->p.cmd_t[0][0])
		write(2, "minishell: : command not found\n", 32);
	c_wd = ft_countword(msh->p.cmd_t[*i]);
	if ((c_wd > 1 && !msh->p.cmd_t[0]) || (c_wd > 1 && ft_strcmp(msh->p.cmd_t[0], "export")))
	{
		*cmd_nb = *cmd_nb + c_wd;
		msh->p.cmd_t = ft_realloc_cmd(msh, cmd_nb, i, NULL);
		if (!msh->p.cmd_t)
			return (NULL); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	}
	return (msh->p.cmd_t);
}

void	ft_set_wild(t_msh *msh)
{
	int i;
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
	char *tmp;

	tmp = msh->av->data;
	msh->av->data = mcgic(mlcp(ft_strdup(data), -1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return;
	msh->av->wild = mcgic(mlcp(NULL, sizeof(int) * ft_strlen(data)), MLC, NO_ENV, msh);
	if (msh->status == 255)
		return;
	ft_set_wild(msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(msh->av->type, 0), FREE, NO_ENV, msh);
	msh->av->type = NULL;
}

int ft_copy_cmd(t_msh *msh, t_head *save, int *i, int *cmd_nb)
{
	if (msh->av->type)
	{
		msh->p.cmd_t = ft_handle_expand(msh, i, cmd_nb);
		if (msh->status == 255)
			return (255);
		if (ck_char(msh->p.cmd_t[*i], '*'))
		{
			ft_reset_msh_av(msh, msh->p.cmd_t[*i]);
			if (msh->status == 255)
				return (255);
			return (0);
		}
	}
	else if (ck_char(msh->av->data, '*'))
		msh->p.cmd_t = ft_expand_wildcard(msh, cmd_nb, i);
	else
		msh->p.cmd_t[*i] = mcgic(mlcp(ft_strdup(msh->av->data), 1), ADD, PIP, msh);
	if (msh->status == 255) // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
		return (255);
	(*i)++;
	msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

int	ft_build_cmd(t_msh *msh, t_head *save, int *cmd_nb)
{
	int i;

	i = 0;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR)
	{
		if (msh->av->token == CMD) // normalement besoin de cette condition quand on implementera les parentheses mais pas certaine => a verifier
		{
			if (ft_copy_cmd(msh, save, &i, cmd_nb) == 255)
				return (255); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
		}
		else
		{
			save->prev = msh->av;
			msh->av = msh->av->next;
		}
	}
	return (0);
}

/*
Create the tab of string containing the cmd + option + arguments
*/
char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2)
{
	t_head	save;
	int		cmd_nb;

	save.head = msh->av;
	save.prev = NULL;
	cmd_nb = ft_count_cmd(msh);
	if (cmd_nb == -1)
	{
		if (msh ->av && msh->av->quote)
			write(2, "minishell: : command not found\n", 32);
		return (NULL); // OK GERE DANS LE PARENT
	}
	msh->p.cmd_t = mcgic(mlcp(NULL, sizeof(char *) * (cmd_nb + 1)), MLC, PIP, msh);
	if (!msh->p.cmd_t)
		ft_exit_bis(msh, sub, fd1, fd2); // SI MALLOC KO ON QUITTE LE PROCESS ACTUEL
	msh->p.cmd_t[0] = NULL;
	if (ft_build_cmd(msh, &save, &cmd_nb) == 255)
		ft_exit_bis(msh, sub, fd1, fd2); // SI MALLOC KO DANS FT_COPY_CMD ON QUITTE LE PROCESS ACTUEL
	msh->p.cmd_t[cmd_nb] = NULL;
	msh->av = save.head;
	return (msh->p.cmd_t);
}
