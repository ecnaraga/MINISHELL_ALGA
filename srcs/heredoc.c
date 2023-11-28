/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:31:49 by galambey          #+#    #+#             */
/*   Updated: 2023/11/28 14:37:22 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
TO DO : BIEN CHECKER LES FREE ET UNLINK
*/

/*
Create a random name for the here_doc and at it at the end of the linked list
	(after the limiter added)
*/
void	create_here_doc(t_split *av, t_list **heredoc)
{
	t_list	*new;
	char *tmp;
	
	/*Add the limiter in the linked list*/
	new = ft_lstnew(av->data);
	if (!new)
		(write(2, "malloc: error\n", 15), exit(EXIT_FAILURE)); //PENSER A FREE 
	ft_lstadd_back(heredoc, new);
	/* Create and add the name of the here_doc in the linked list*/
	tmp = ft_magic_malloc(ADD, 0, ft_random_filename("/tmp/", 8), NO_ENV);
	if (!tmp)
	{
		tmp = ft_magic_malloc(ADD, 0, ft_strdup("/tmp/here_doc"), NO_ENV);
		if (!tmp)
			(write(2, "malloc: error\n", 15), exit(EXIT_FAILURE)); //PENSER A FREE 
	}
	new = ft_lstnew(tmp);
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

	write(1, "> ", 2);
	line = ft_magic_malloc(ADD, 0, get_next_line(STDIN_FILENO), NO_ENV);
	if (!line)
	{
		(close(fd_temp), write(2, "get_next_line: error\n", 21), ft_magic_malloc(FREE, 0, lim, NO_ENV));
		(unlink(here_doc->content)/*, ft_magic_malloc(FREE, 0, p->name_here_doc, NO_ENV)*/); //Penser a free
		exit(EXIT_FAILURE);
	}
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
void	ft_prompt(t_msh *msh, t_split *av, t_list **here_doc)
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
		if (ft_recover_prompt(fd_temp, lim, *here_doc) == 1)
			break ;
	}
	msh->p.prompt = 1;
	(close(fd_temp), ft_magic_malloc(FREE, 0, lim, NO_ENV));
}

/*
In a loop, create all the here_doc and storage in a linked list the limiter
	followed by the name of the here_doc created
*/
void	ft_heredoc(t_msh *msh)
{
	t_split *head;
	
	head = msh->av;
	msh->p.here_doc = NULL;
	while (msh->av)  // A METTRE DANS FT_EXEC EN PREMIER LIEU
	{
		if (msh->av->token == HERE_DOC)
			ft_prompt(msh, msh->av, &msh->p.here_doc);
		msh->av = msh->av->next;
	}
	msh->av = head;
}
