/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/12/05 09:59:02 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_research_path(t_list **env)
{
	// t_list	*head; // BESOIN D UN HEAD ICI ? PAS SUR

	if (!env)
		return (NULL);
	// head = *env;
	while (*env)
	{
		if (ft_strncmp((*env)->content, "PATH=", 5) == 0)
			return (ft_split_magic_malloc((*env)->content + 5, ':')); //implementer ft_magic ici
		*env = (*env)->next;
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

/*
Open and close in a loop all the heredoc and infile until a pipe, operator or
	the end of commandline.
The last heredoc or infile encountered is dup2 to stdin_fileno before being
	closed
*/
void	redef_stdin(t_msh *msh, int rule, int j)
{
	int		fd_infile;
	char	*str;
	t_split *head;
	t_split *prev;
	t_list *head_hd;
	// t_list *prev_hd;

	head = msh->av;
	prev = NULL;
	fd_infile = -2;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR && msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == INFILE)
		{
			if (fd_infile != -2)
				close(fd_infile);
			fd_infile = open(msh->av->data, O_RDONLY);
			if (fd_infile > -1)
				msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
		}
		else if (msh->av->token == HERE_DOC)
		{
			if (fd_infile != -2)
				close(fd_infile);
			head_hd = msh->p.here_doc;
			// prev_hd = NULL;
			while (msh->p.here_doc && msh->av && ft_strcmp((char *)msh->p.here_doc->content, msh->av->data) != 0)//
				msh->p.here_doc = msh->p.here_doc->next;
			fd_infile = open(msh->p.here_doc->next->content, O_RDONLY);
			// msh->p.here_doc = ft_lstdel_and_relink(msh->p.here_doc, prev_hd, &head_hd); // A REVOIR
			// msh->p.here_doc = ft_lstdel_and_relink(msh->p.here_doc, prev_hd, &head_hd); // A REVOIRnormalement c est l element suivant
			if (fd_infile > -1)
				msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
			msh->p.here_doc = head_hd;
		}
		else
		{
			prev = msh->av;
			msh->av = msh->av->next;
		}
		if (fd_infile == -1)
		{
			str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", msh->av->data), PIP);
			//IF ERROR MALLOC ? 
			if (str)
			{
				perror(str);
				status = 1;
				msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
				ft_magic_malloc(FREE, 0, str, PIP);
			}
			if (rule == CMD_ALONE)
				ft_exit(-1, -1, -1);
			if (rule == FIRST)
				ft_exit(msh->p.fd_p[0][1], -1, -1);
			if (rule == MID)
				ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1);
			else
				ft_exit(msh->p.fd_p[j - 1][0], -1, -1);
		}
	}
	msh->av = head;
	if (fd_infile == -2 && (rule == FIRST || rule == CMD_ALONE))
		return ;
	if (fd_infile == -2 && (rule == MID || rule == LAST))
	{
		if (dup2(msh->p.fd_p[j - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2");  // EXIT STATUS ?
			if (rule == MID)
				ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1);
			else
				ft_exit(msh->p.fd_p[j - 1][0], -1, -1);
		}
		return ;
	}
	if (dup2(fd_infile, STDIN_FILENO) == -1)
	{
		perror("dup2"); // EXIT STATUS ?
		if (rule == CMD_ALONE)
				ft_exit(fd_infile, -1, -1);
		if (rule == FIRST)
			ft_exit(msh->p.fd_p[0][1], fd_infile, -1);
		if (rule == MID)
			ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], fd_infile);
		else
			ft_exit(msh->p.fd_p[j - 1][0], fd_infile, -1);
	}
	close(fd_infile);
}

void	redef_stout(t_msh *msh, int rule, int j)
{
	char	*str;
	t_split *head;
	t_split *prev;
	int		fd_outfile;
	
	head = msh->av;
	prev = NULL;
	fd_outfile = -2;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR /* && msh->av->token != PAR_CLOSE */)
	{
		if (msh->av->token == OUTFILE_TRUNC)
		{
			if (fd_outfile != -2)
				close(fd_outfile);
			fd_outfile = open(msh->av->data, O_CREAT | O_TRUNC | O_WRONLY, 0744);
			msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
		}
		else if (msh->av->token == OUTFILE_NO_TRUNC)
		{
			if (fd_outfile != -2)
				close(fd_outfile);
			fd_outfile = open(msh->av->data, O_CREAT | O_APPEND | O_WRONLY, 0744);
			msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
		}
		else
		{
			prev = msh->av;
			msh->av = msh->av->next;
		}
		if (fd_outfile == -1)
		{
			str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", msh->av->data), PIP);
			if (str)
			{
				perror(str);
				status = 1;
				// msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
				// ft_magic_malloc(FREE, 0, str, PIP);
			}
			if (rule == CMD_ALONE)
				ft_exit(-1, -1, -1);
			if (rule == FIRST)
				ft_exit(msh->p.fd_p[0][1], -1, -1);
			if (rule == MID)
				ft_exit(msh->p.fd_p[j][1], -1, -1);
			else
				ft_exit(-1, -1, -1);
		}
	}
	msh->av = head;
	if (fd_outfile == -2 && (rule == LAST || rule == CMD_ALONE))
		return ;
	if (fd_outfile == -2 && (rule == FIRST || rule == MID))
	{
		if (dup2(msh->p.fd_p[j][1], STDOUT_FILENO) == -1)
		{
			perror("dup2"); // EXIT STATUS ? 
			if (rule == FIRST)
				ft_exit(msh->p.fd_p[0][1], -1, -1);
			else
				ft_exit(msh->p.fd_p[j][1], -1, -1);
		}
		return ;
	}
	if (dup2(fd_outfile, STDOUT_FILENO) == -1)
	{
		perror("dup2"); // EXIT STATUS ?
		if (rule == CMD_ALONE)
			ft_exit(fd_outfile, -1, -1);
		if (rule == FIRST)
			ft_exit(msh->p.fd_p[0][1], fd_outfile, -1);
		if (rule == MID)
			ft_exit(msh->p.fd_p[j][1], fd_outfile, -1);
		else
			ft_exit(fd_outfile, -1, -1);
	}
	close(fd_outfile);
}
			
// 	if (ft_strcmp(av[1], "here_doc") == 0)
// 	{
// 		*fd_outfile = open(av[j + 4], O_CREAT | O_APPEND | O_WRONLY, 0744);
// 		if (*fd_outfile == -1)
// 		{
// 			str = ft_strjoin("bash: ", av[j + 4]);
// 			(perror(str), free(str), ft_exit(&p, -1, -1, -1));
// 		}
// 	}
// 	else
// 		*fd_outfile = open(av[j + 3], O_CREAT | O_TRUNC | O_WRONLY, 0744);
// 	if (*fd_outfile == -1)
// 	{
// 		str = ft_strjoin("bash: ", av[j + 3]);
// 		(perror(str), free(str), ft_exit(&p, -1, -1, -1));
// 	}
// }

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
		ft_exit(-1, -1, -1);
	return (count);
}

/*
Create the tab of string containing the cmd + option + arguments
*/
char	**ft_make_cmd(t_msh *msh)
{
	t_split	*head;
	t_split *prev;
	char	**cmd;
	int		i;

	head = msh->av;
	prev = NULL;
	cmd = ft_magic_malloc(MALLOC, sizeof(char *) * (ft_count_cmd(msh) + 1), NULL, PIP);
	if (!cmd)
		ft_exit(-1, -1, -1);
	i = 0;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR)
	{
		if (msh->av->token == CMD) // normalement besoin de cette condition quand on implementera les parentheses mais pas certaine => a verifier
		{
			cmd[i] = ft_magic_malloc(ADD, 0, ft_strdup(msh->av->data), PIP);
			if (!cmd[i])
				ft_exit(-1, -1, -1);
			i++;
			msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
		}
		else
		{
			prev = msh->av;
			msh->av = msh->av->next;
		}
	}
	cmd[i] = NULL;
	msh->av = head;
	return (cmd);
}
