/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:31:49 by galambey          #+#    #+#             */
/*   Updated: 2023/12/05 17:16:37 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
TO DO : BIEN CHECKER LES FREE ET UNLINK
*/

int	ft_check_unique_name(char *tmp, t_list **heredoc)
{
	t_list *head;

	head = *heredoc;
	while (*heredoc)
	{
		if (ft_strcmp((*heredoc)->content, tmp) == 0)
			return (1);
		*heredoc = (*heredoc)->next;
	}
	*heredoc = head;
	return (0);
}

/*
Create a random name for the here_doc and at it at the end of the linked list
	(after the limiter added)
*/
void	create_here_doc(t_split *av, t_list **heredoc)
{
	t_list	*new;
	char *tmp;
	
	/*Add the limiter in the linked list*/
	tmp = ft_magic_malloc(ADD, 0, ft_strdup(av->data), NO_ENV);
	new = ft_magic_malloc(ADD, 0, ft_lstnew(tmp), NO_ENV);
	if (!new)
		(write(2, "malloc: error\n", 15), exit(EXIT_FAILURE)); //PENSER A FREE 
	ft_lstadd_back(heredoc, new);
	/* Create and add the name of the here_doc in the linked list*/
	while (1)
	{
		tmp = ft_magic_malloc(ADD, 0, ft_random_filename("/tmp/", 8), NO_ENV);
		if (!tmp)
			(write(2, "malloc: error\n", 15), exit(EXIT_FAILURE)); //PENSER A FREE 
		if (ft_check_unique_name(tmp, heredoc) == 0)
			break;
		else
			ft_magic_malloc(FREE, 0, tmp, NO_ENV);
	}
	new = ft_magic_malloc(ADD, 0, ft_lstnew(tmp), NO_ENV);
	if (!new)
		(write(2, "malloc: error\n", 15), exit(EXIT_FAILURE)); //PENSER A FREE 
	ft_lstadd_back(heredoc, new);
}

/*
Function called in a loop in the parent function
Copy into the file create for an heredoc, the stdin line entered by the user. 
*/
static int	ft_recover_prompt(int fd_temp, char *lim, t_list *here_doc)
{
	char	*line;

	ft_signal_handler_msh_ter();
	write(1, "> ", 2);
	line = get_next_line(STDIN_FILENO);

	// write(1, "> ", 2);
	// line = ft_magic_malloc(ADD, 0, readline(">"), NO_ENV);
	if (status == 130)
	{
		free(line);
		open("/dev/stdout", O_RDONLY);
		return (130);
	}
	if (!line)
	{
		free(line);
		(close(fd_temp), write(2, "get_next_line: error\n", 21), ft_magic_malloc(FREE, 0, lim, NO_ENV));
		(unlink(here_doc->content)/*, ft_magic_malloc(FREE, 0, p->name_here_doc, NO_ENV)*/); //Penser a free
		exit(EXIT_FAILURE);
	}
	ft_magic_malloc(ADD, 0, line, NO_ENV);
	if (ft_strcmp(lim, line) == 0)
	{
		ft_magic_malloc(FREE, 0, line, NO_ENV);
		return (1);
	}
	(write(fd_temp, line, ft_strlen(line)), ft_magic_malloc(FREE, 0, line, NO_ENV));
	return (0);
}

/*
Function called in a loop in the parent function
For each limiter encountered, create a heredoc and recover the stdin to copy it
	into the file created
*/
int	ft_prompt(t_msh *msh, t_split *av, t_list **here_doc)
{
	int		fd_temp;
	char	*lim;
	t_list *head;

	create_here_doc(av, here_doc);
	head = *here_doc;
	while ((*here_doc)->next)
		*here_doc = (*here_doc)->next;
	fd_temp = open((*here_doc)->content, O_CREAT | O_APPEND | O_WRONLY, 0744);
	if (fd_temp == -1)
		(perror((*here_doc)->content), /*free(p->name_here_doc),*/ exit(EXIT_FAILURE)); //PENSER A FREE 
	lim = ft_magic_malloc(ADD, 0, ft_strjoin(av->data, "\n"), NO_ENV);
	if (!lim)
	{
		(close(fd_temp), write(2, "ft_strjoin: error\n", 18));
		unlink((*here_doc)->content)/* ,free(p->name_here_doc)*/;
		exit(EXIT_FAILURE);  //PENSER A FREE 
	}
	
	while (1)
	{
		int err ;

		err = ft_recover_prompt(fd_temp, lim, *here_doc);
		if (err == 1)
			break ;
		if (err == 130)
		{
			close(fd_temp);
			return (130);
		}
	}
	// open("/dev/stdout", O_RDONLY);
	ft_signal_handler_msh();
	msh->p.prompt = 1;// (SUPPRIMER PROMPT PAS BESOIN)
	*here_doc = head;
 	(close(fd_temp), ft_magic_malloc(FREE, 0, lim, NO_ENV));
	return (0);
}

/*
In a loop, create all the here_doc and storage in a linked list the limiter
	followed by the name of the here_doc created
*/
int	ft_heredoc(t_msh *msh)
{
	t_split *head;
	
	head = msh->av;
	msh->p.here_doc = NULL;
	while (msh->av)  // A METTRE DANS FT_EXEC EN PREMIER LIEU
	{
		if (msh->av->token == HERE_DOC)
		{
			if (ft_prompt(msh, msh->av, &msh->p.here_doc) == 130)
				return (130);
		}
		msh->av = msh->av->next;
	}
	msh->av = head;
	return (0);
}

void	ft_unlink_heredoc(t_list *heredoc)
{
	int i;

	i = 0;
	while (heredoc)
	{
		// if (i % 2 == 1)
		unlink(heredoc->content);
		heredoc = heredoc->next;
		i++;
	}
}
