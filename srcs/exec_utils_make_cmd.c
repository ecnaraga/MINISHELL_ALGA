/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_make_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:13:19 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 10:57:52 by galambey         ###   ########.fr       */
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

// static int	ft_countwords(const char *s)
// {
// 	int	c_wd;
// 	int	i;

// 	c_wd = 0;
// 	i = 0;
// 	while (s[i])
// 	{
// 		while (s[i] && (s[i] == ' ' || (s[i] >= '\t' && s[i] <= '\r')))
// 			i++;
// 		if (s[i] && (s[i] != ' ' && (s[i] < '\t' || s[i] > '\r')))
// 		{
// 			c_wd++;
// 			i++;
// 		}
// 		while (s[i] && (s[i] != ' ' && (s[i] < '\t' || s[i] > '\r')))
// 			i++;
// 	}
// 	return (c_wd);
// }

char **ft_realloc_cmd(char **cmd_opt, int *cmd_nb, int *i)
{
	char 	**cmd;
	char 	**tmp;
	int		j;
	int		k;

	cmd = ft_magic_malloc(MALLOC, sizeof(char *) * (*cmd_nb + 1), NULL, PIP);
	if (!cmd)
		return (NULL); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	j = 0;
	while (j < *i)
	{
		cmd[j] = cmd_opt[j];
		j++;
	}
	tmp = ft_split_isspace_magic_malloc(cmd_opt[j]);
	if (!tmp)
		return (NULL); // IF ERREUR MALLOC DANS FT_SPLIT_ISSPACE_MAGIC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	k = 0;
	while (tmp[k])
		cmd[j++] = tmp[k++];
	cmd[j] = NULL; // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	*i = j - 1;
	ft_magic_malloc(FREE, 0, cmd_opt, PIP);
	ft_magic_malloc(FREE, 0, tmp, PIP);
	return (cmd);
}

int ft_copy_cmd(t_msh *msh, t_head *save, int *i, int *cmd_nb)
{
	int	c_wd;
	
	if (msh->av->type)
	{
		if (*i == 0)
			msh->p.cmd_opt[*i] = ft_expand(msh, msh->p.cmd_opt[*i], CMD); // IF ERROR MALLOC, EXPAND RETURN (NULL)
		else
			msh->p.cmd_opt[*i] = ft_expand(msh, msh->p.cmd_opt[*i], OTHER); // IF ERROR MALLOC, EXPAND RETURN (NULL)
		if (status == 255)
			return (255); // IF ERREUR MALLOC DANS EXPAND ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
		if (msh->av->quote && !msh->p.cmd_opt[0][0])
			write(2, "minishell: : command not found\n", 32);
		c_wd = ft_countword(msh->p.cmd_opt[*i]);
		if (c_wd > 1)
		{
			*cmd_nb = *cmd_nb + c_wd;
			msh->p.cmd_opt = ft_realloc_cmd(msh->p.cmd_opt, cmd_nb, i);
			if (!msh->p.cmd_opt)
				return (255); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
		}
	}
	else
	{
		msh->p.cmd_opt[*i] = ft_magic_malloc(ADD, 0, ft_strdup(msh->av->data), PIP);
		if (!msh->p.cmd_opt[*i])
			return (255); // IF ERREUR MALLOC ON QUITTE LE PROCESS EN COURS DANS FT_MAKE_CMD
	}
	(*i)++;
	msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
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
		if (msh->av->quote)
			write(2, "minishell: : command not found\n", 32);
		return (NULL); // OK GERE DANS LE PARENT
	}
	msh->p.cmd_opt = ft_magic_malloc(MALLOC, sizeof(char *) * (cmd_nb + 1), NULL, PIP);
	if (!msh->p.cmd_opt)
		ft_exit_bis(msh, sub, fd1, fd2); // SI MALLOC KO ON QUITTE LE PROCESS ACTUEL
	if (ft_build_cmd(msh, &save, &cmd_nb) == 255)
		ft_exit_bis(msh, sub, fd1, fd2); // SI MALLOC KO DANS FT_COPY_CMD ON QUITTE LE PROCESS ACTUEL
	msh->p.cmd_opt[cmd_nb] = NULL;
	msh->av = save.head;
	return (msh->p.cmd_opt);
}

/*
Count the numbers of elements that we will put in the cmd (cmd + option +
	arguments)
*/
// int		ft_count_cmd(t_msh *msh)
// {
// 	t_split	*head;
// 	int		count;

// 	head = msh->av;
// 	count = 0;
// 	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR)
// 	{
// 		if (msh->av->token == CMD)
// 			count++;
// 		msh->av = msh->av->next;
// 	}
// 	msh->av = head;
// 	printf("count %d\n", count);
// 	if (count == 0)
// 		return (-1);
// 	return (count);
// }

// char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2)
// {
// 	t_head	save;
// 	char	**cmd;
// 	int		i;
// 	int		cmd_nb;

// 	printf("FT_MAKECMD\n");
// 	save.head = msh->av;
// 	save.prev = NULL;
// 	cmd_nb = ft_count_cmd(msh);
// 	if (cmd_nb == -1)
// 	{
// 		if (msh->av->quote)
// 			write(2, "minishell: : command not found\n", 32);
// 		return (NULL); // OK GERE DANS LE PARENT
// 	}
// 	cmd = ft_magic_malloc(MALLOC, sizeof(char *) * (cmd_nb + 1), NULL, PIP);
// 	if (!cmd)
// 		ft_exit_bis(msh, sub, fd1, fd2); // SI MALLOC KO ON QUITTE LE PROCESS ACTUEL
// 	i = 0;
// 	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR)
// 	{
// 		// if (ft_strcpy_cmd(msh, cmd, &i, &save) == NULL)
// 		// 	return (NULL);
// 	// A EFFACER UNNIQUEMENT SI SUR!!!!
// 		if (msh->av->token == CMD) // normalement besoin de cette condition quand on implementera les parentheses mais pas certaine => a verifier
// 		{
// 			if (msh->av->type /* && msh->av->type->expnd != 2 */)
// 			{
// 				printf("EXPAND\n");
// 				if (i == 0)
// 					cmd[i] = ft_expand(msh, cmd[i], CMD); // IF ERROR MALLOC, EXPAND RETURN (NULL)
// 				else
// 					cmd[i] = ft_expand(msh, cmd[i], OTHER); // IF ERROR MALLOC, EXPAND RETURN (NULL)
// 				if (status == 255)
// 					ft_exit_bis(msh, sub, fd1, fd2); // IF ERREUR MALLOC DANS EXPAND ON QUITTE LE PROCESS EN COURS
// 				// if (err != 0)
// 				// 	ft_exit(-1, -1, -1);
// 				/* dprintf(2, "cmd[*i] %s cmd[*i][0] |%c| i %d\n", cmd[i], cmd[i][0], i);
// 				printf("cmd[i] %s\n", cmd[i]); */
// 				// j = 0;
// 				// while (cmd[i][j] && ft_isspace(cmd[i][j] == 0))
// 				// 	j++;
// 				// if (!cmd[i][j])
// 				// 	cmd[i][0] = '\0';
// 				if (msh->av->quote && !cmd[0][0])
// 					write(2, "minishell: : command not found\n", 32);
// 			}
// 			else
// 			{
// 				cmd[i] = ft_magic_malloc(ADD, 0, ft_strdup(msh->av->data), PIP);
// 				if (!cmd[i])
// 					ft_exit_bis(msh, sub, fd1, fd2); // IF ERREUR MALLOC DANS EXPAND ON QUITTE LE PROCESS EN COURS
// 			}
// 			i++;
// 			msh->av = ft_lstdel_and_relink_split(msh->av, save.prev, &save.head);
// 		}
// 		else
// 		{
// 			save.prev = msh->av;
// 			msh->av = msh->av->next;
// 		}
// 	}
// 	cmd[i] = NULL;
// 	msh->av = save.head;
// 	return (cmd);
// }