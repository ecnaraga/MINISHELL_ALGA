/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/12/18 10:13:34 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Return a tab of strings of the the known paths.
If malloc ko, the process is quit in ft_split.
Return NULL if there is no env or if the path has been unset or contain nothing.
*/
char	**ft_research_path(t_msh *msh, t_env **env, int sub)
{
	t_env	*node;

	if (!env)
		return (NULL);
	node = *env;
	while (node)
	{
		if (ft_strncmp(node->name, "PATH", 4) == 0)
			// return (ft_split_magic_malloc(node->content, ':')); Quand merge avec alix qui a change l env > plus de = 
			return (ft_split_magic_malloc(msh, sub, node->content + 1, ':')); // IF ERROR MALLOC ON QUITTE LE PROCESS ACTUEL A L INTERIEUR
		node = node->next;
	}
	return (NULL);
}

static int	ft_find_good_path(char **path, char **good_path, char *cmd,
		int accss)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path[i] && accss != 0)
	{
		*good_path = ft_magic_malloc(ADD, 0, ft_strjoin(path[i], "/"), PIP);
		if (!good_path)
			return (E_STRJOIN);
		tmp = *good_path;
		*good_path = ft_magic_malloc(ADD, 0, ft_strjoin(*good_path, cmd), PIP);
		if (!*good_path)
			return (E_STRJOIN);
		ft_magic_malloc(FREE, 0, tmp, PIP);
		accss = access(*good_path, F_OK | X_OK);
		if (accss == 0)
			return (E_OK);
		ft_magic_malloc(FREE, 0, *good_path, PIP);
		*good_path = NULL;
		i++;
	}
	return (E_NO_CMD);
}

int	ft_access_cmd(char **path, char *cmd, char **good_path)
{
	int		accss;
	char	*tmp;

	accss = access(cmd, F_OK | X_OK);
	if (accss == 0)
	{
		tmp = ft_magic_malloc(ADD, 0, ft_strjoin(cmd, "/"), PIP);
		if (!tmp)
			return (E_STRJOIN);
		accss = access(tmp, F_OK | X_OK);
		if (accss == 0)
			return (ft_magic_malloc(FREE, 0, tmp, PIP), E_NO_CMD);
		*good_path = ft_magic_malloc(ADD, 0, ft_strdup(cmd), PIP);
		if (!*good_path)
			return (E_STRDUP);
		return (E_OK);
	}
	if (!path)
		return (E_NO_CMD);
	return (ft_find_good_path(path, good_path, cmd, accss));
}
void	ft_inc_shlvl(char **env_tab, char *str)//IF ERROR MALLOC?
{
	int	i;
	int shlvl;
	char *tmp;

	i = -1;
	if (ft_strcmp(str, "/usr/bin/bash") != 0 && ft_strcmp(str, "/usr/bin/sh") != 0 && ft_strcmp(str, "/usr/bin/zsh") != 0 && ft_strcmp(str, "./minishell")!= 0 )
		return;
	while (env_tab[++i])
	{
		if (ft_strncmp(env_tab[i], "SHLVL", 5) == 0)
		{
			shlvl = ft_atoi(env_tab[i] + 6) + 1;
			tmp = ft_magic_malloc(ADD, 0, ft_itoa(shlvl), PIP);
			ft_magic_malloc(FREE, 0, env_tab[i], PIP);
			env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin("SHLVL=", tmp), PIP);
			ft_magic_malloc(FREE, 0, tmp, PIP);
			return ;
		}
	}
	env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin("SHLVL=", "0"), PIP);
	env_tab[++i] = NULL;
}

char **ft_transcript_env(t_env **env, char *str)
{
	t_env	*head;
	char	**env_tab;
	int i;
	
	head = *env;
	env_tab = ft_magic_malloc(MALLOC, sizeof(char *) * (ft_lstsize_env(*env) + 2), NULL, PIP);
	// if (ERROR MALLOC)
	i = 0;
	while (*env)
	{
		env_tab[i] = ft_magic_malloc(MALLOC, sizeof(char) * (ft_strlen((*env)->name) + ft_strlen((*env)->content) + 1), NULL, PIP);
		// if (ERROR MALLOC)
		env_tab[i] = ft_magic_malloc(ADD, 0, ft_strjoin((*env)->name, (*env)->content), PIP);
		i++;
		*env = (*env)->next;
	}
	env_tab[i] = NULL;
	*env = head;
	ft_inc_shlvl(env_tab, str);
	return (env_tab);
}

// /*
// Count the numbers of elements that we will put in the cmd (cmd + option +
// 	arguments)
// */
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

// // char *ft_strcpy_cmd(t_msh *msh, char **cmd, int *i, t_head *save)
// // {
// // 	if (msh->av->token == CMD) // normalement besoin de cette condition quand on implementera les parentheses mais pas certaine => a verifier
// // 	{
// // 		if (msh->av->type && msh->av->type->expnd != 2)
// // 		{
// // 			cmd[*i] = ft_expand(msh, cmd[*i]); //if error return null
// // 			// if (err != 0)
// // 			// 	ft_exit(-1, -1, -1);
// // 		}
// // 		else
// // 		{
// // 			cmd[*i] = ft_magic_malloc(ADD, 0, ft_strdup(msh->av->data), PIP);
// // 			if (!cmd[*i])
// // 				return (NULL);
// // 		}
// // 		(*i)++;
// // 		msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
// // 		return (cmd[*i - 1]);
// // 	}
// // 	else // UTILE ? 1 je ne crois pas + 2 je pense que si oui le retour apres le else va creer un conditional jump et pas possible de return (NULL) car sinon on ne passe pas a la suite...
// // 	{
// // 		save->prev = msh->av;
// // 		msh->av = msh->av->next;
// // 	}
// // 	return (cmd[*i]);
// // }

// /*
// Create the tab of string containing the cmd + option + arguments
// */
// char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2)
// {
// 	t_head	save;
// 	// char	**cmd;
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
// 	msh->p.cmd_opt = ft_magic_malloc(MALLOC, sizeof(char *) * (cmd_nb + 1), NULL, PIP);
// 	if (!msh->p.cmd_opt)
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
// 					msh->p.cmd_opt[i] = ft_expand(msh, msh->p.cmd_opt[i], CMD); // IF ERROR MALLOC, EXPAND RETURN (NULL)
// 				else
// 					msh->p.cmd_opt[i] = ft_expand(msh, msh->p.cmd_opt[i], OTHER); // IF ERROR MALLOC, EXPAND RETURN (NULL)
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
// 				if (msh->av->quote && !msh->p.cmd_opt[0][0])
// 					write(2, "minishell: : command not found\n", 32);
// 			}
// 			else
// 			{
// 				msh->p.cmd_opt[i] = ft_magic_malloc(ADD, 0, ft_strdup(msh->av->data), PIP);
// 				if (!msh->p.cmd_opt[i])
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
// 	msh->p.cmd_opt[i] = NULL;
// 	msh->av = save.head;
// 	return (msh->p.cmd_opt);
// }
