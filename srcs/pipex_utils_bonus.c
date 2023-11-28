/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/11/28 11:59:41 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

char	**ft_research_path(t_list **env)
{
	// t_list	*head; // BESOIN D UN HEAD ICI ? PAS SUR

	if (!env)
		return (NULL);
	// head = *env;
	while (env)
	{
		if (ft_strncmp(env->content, "PATH=", 5) == 0)
			return (ft_split(env->content + 5, ':')); //implementer ft_magic ici
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
		*good_path = ft_strjoin(path[i], "/");
		if (!good_path)
			return (E_STRJOIN);
		tmp = *good_path;
		*good_path = ft_strjoin(*good_path, cmd);
		if (!*good_path)
			return (E_STRJOIN);
		free(tmp);
		accss = access(*good_path, F_OK | X_OK);
		if (accss == 0)
			return (E_OK);
		free(*good_path);
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
		tmp = ft_strjoin(cmd, "/");
		if (!tmp)
			return (E_STRJOIN);
		accss = access(tmp, F_OK | X_OK);
		if (accss == 0)
			return (free(tmp), E_NO_CMD);
		*good_path = ft_strdup(cmd);
		if (!*good_path)
			return (E_STRDUP);
		return (E_OK);
	}
	if (!path)
		return (E_NO_CMD);
	return (ft_find_good_path(path, good_path, cmd, accss));
}

void	redef_stdin(char **av, t_pipex p)
{
	int		fd_infile;
	char	*str;

	if (p.prompt == 0)
	{
		fd_infile = open(av[1], O_RDONLY);
		if (fd_infile == -1)
		{
			str = ft_strjoin("bash: ", av[1]);
			(perror(str), free(str), ft_exit(&p, p.fd_p[0][1], -1, -1));
		}
	}
	else
	{
		fd_infile = open(p.name_here_doc, O_RDONLY);
		if (fd_infile == -1)
		{
			str = ft_strjoin("bash: ", p.name_here_doc);
			(perror(str), free(str), ft_exit(&p, p.fd_p[0][1], -1, -1));
		}
	}
	if (dup2(fd_infile, STDIN_FILENO) == -1)
		(perror("dup2"), ft_exit(&p, p.fd_p[0][1], fd_infile, -1));
	close(fd_infile);
}

void	redef_stout(char **av, t_pipex p, int *fd_outfile, int j)
{
	char	*str;

	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		*fd_outfile = open(av[j + 4], O_CREAT | O_APPEND | O_WRONLY, 0744);
		if (*fd_outfile == -1)
		{
			str = ft_strjoin("bash: ", av[j + 4]);
			(perror(str), free(str), ft_exit(&p, -1, -1, -1));
		}
	}
	else
		*fd_outfile = open(av[j + 3], O_CREAT | O_TRUNC | O_WRONLY, 0744);
	if (*fd_outfile == -1)
	{
		str = ft_strjoin("bash: ", av[j + 3]);
		(perror(str), free(str), ft_exit(&p, -1, -1, -1));
	}
}
