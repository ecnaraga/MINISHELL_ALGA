/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_ter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:16:50 by galambey          #+#    #+#             */
/*   Updated: 2023/11/28 11:29:02 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_error_message_final(char *str)
{
	char	*message;

	if (!str[0])
		message = ft_magic_malloc(ADD, 0, ft_strdup
				("minishell: syntax error near unexpected token `newline'\n"), NO_ENV);
	else
	{
		message = ft_magic_malloc(ADD, 0,
				ft_strjoin("minishell: syntax error near unexpected token `",
					str), NO_ENV);
		if (!message)
			return (NULL);
		message = ft_magic_malloc(ADD, 0, ft_strjoin(message, "'\n"), NO_ENV);
	}
	return (message);
}

char	*err_chev(char *str, int skip)
{
	int		i;
	int		j;
	char	c;
	char	tmp[4];

	i = 0;
	j = 0;
	c = str[0];
	if (skip == 1)
	{
		if (c == '>')
			while (str[++i] && str[i] == c && i < 2)
				;
		else if (c == '<')
			while (str[++i] && str[i] == c && i < 3)
				;
	}
	if (str[i])
		c = str[i];
	while (str[i] && ((c == '>' && str[i] == c && j < 2)
			|| (c == '<' && str[i] == c && j < 3)))
		tmp[j++] = str[i++];
	if (str[i] && str[i] != c && j == 1)
		tmp[j++] = str[i++];
	return (tmp[j] = '\0', ft_error_message_final(tmp));
}

char	*ft_err_op(char *str, int skip)
{
	int	i;

	i = 0;
	if (skip == 1)
	{
		if (str[0] == str[1])
			i = 2;
		else
			i = 1;
	}
	if (!str[i + 1] || (str[i + 1]
			&& ((skip == 0 && str[i] == '&' && str[i + 1] == '|')
				|| (skip == 1 && str[i] != str[i + 1]))))
		str[i + 1] = '\0';
	else if (str[i + 1])
		str[i + 2] = '\0';
	return (ft_error_message_final(str + i));
}

/*
Return 0 if all the string's characters are the same, otherwise return 1
*/
int	ft_same_char(char *str)
{
	char	c;
	int		i;

	c = str[0];
	i = -1;
	while (str[++i] && str[i] == c)
		;
	if (str[i])
		return (1);
	return (0);
}

int	ft_check_chev(t_msh *msh)
{
	if (msh->av->token == CHEVRON && (ft_strlen(msh->av->data) > 2
			|| ft_same_char(msh->av->data) == 1))
		return (status = 2, err_syntax(err_chev(msh->av->data, 1)));
	if (msh->av->next->data && msh->av->token == CHEVRON
		&& msh->av->next->token == CHEVRON)
		return (status = 2, err_syntax(err_chev(msh->av->next->data, 0)));
	if (msh->av->next->data && msh->av->token == CHEVRON
		&& (msh->av->next->token == OPERATOR || msh->av->next->token == PIPE))
		return (status = 2, err_syntax(ft_err_op(msh->av->next->data, 0)));
	return (0);
}

int	ft_check_op(t_msh *msh)
{
	if (msh->av->token == OPERATOR && (ft_strlen(msh->av->data) > 2
			|| ft_same_char(msh->av->data) == 1))
		return (status = 2, err_syntax(ft_err_op(msh->av->data, 1)));
	if (msh->av->next->data && (msh->av->token == OPERATOR || msh->av->token == PIPE)
		&& (msh->av->next->token == OPERATOR || msh->av->next->token == PIPE))
		return (status = 2, err_syntax(ft_err_op(msh->av->next->data, 0)));
	return (0);
}

int	ft_handle_chev(t_msh *msh, t_split *prev, t_split **head)
{
	t_split	*tmp;
	
	if (msh->ac == 1)
		return (status = 2, err_syntax(err_chev(msh->av->data, 1)));
	else if (ft_check_chev(msh) == 1)
		return (1);
	else if (prev == NULL)
	{
		tmp = msh->av->next;
		ft_lstdelone_split(msh->av, del_two);
		msh->av = tmp;
		*head = msh->av;
	}
	else
	{
		prev->next = msh->av->next;
		ft_lstdelone_split(msh->av, del_two);
		msh->av = prev->next;
	}
	return (0);
}

// SUPPRIMER LES MAILLONS CHEVRONS SI PAS D ERREUR AVDC FT_LSTDELONE_SPLIT
int	ft_parse_ter(t_msh *msh)
{
	t_split	*head;
	t_split	*tmp;

	head = msh->av;
	tmp = NULL;
	printf("PARSE_TER\n");
	// if (msh->av->token == CHEVRON/* && msh->ac == 1*/)
	// 	ft_handle_chev(msh, tmp);
	// if (msh->av->token == CHEVRON && msh->ac == 1)
	// 	return (status = 2, err_syntax(err_chev(msh->av->data, 1)));
	if (msh->av->token == OPERATOR || msh->av->token == PIPE)
		return (status = 2, err_syntax(ft_err_op(msh->av->data, 0)));
	while (msh->av->next)
	{
		if (msh->av->token == CHEVRON)
		{
			if (ft_handle_chev(msh, tmp, &head) == 1)
				return (1);
		}
		// if (ft_check_chev(msh) == 1)
		// 	return (1);
		// if (ft_check_op(msh) == 1)
		// 	return (1);
		else
		{
			if (ft_check_op(msh) == 1)
				return (1);
			tmp = msh->av;
			msh->av = msh->av->next;
		}
	}
	// if (msh->av[msh->ac - 1].token == OPERATOR) // si se termine par un operateur -> rester a l ecoute
	// 	return (err_syntax(ft_err_op(msh->av[msh->ac - 1].data, 0), 2, 1));
	if (msh->av->token == CHEVRON)
		return (status = 2, err_syntax(err_chev(msh->av->data, 1)));
	if (msh->av->token == OPERATOR || msh->av->token == PIPE) // avoir si on decide de considerer en dernier un operator comme erreur ou non
		return (status = 2, err_syntax(ft_err_op(msh->av->data, 0)));
	msh->av = head;
	// if (msh->av[msh->ac - 1].token == OPERATOR) // si se termine par un operateur -> rester a l ecoute
	// {
	// 	ft_free_split_msh(msh->av);
	// 	char *new_line;
	// 	char *tmp;
		
	// 	new_line = readline("> ");
	// 	new_line = ft_magic_malloc(ADD, 0, new_line);
	// 	if (status == 0)
	// 	{
	// 		tmp = msh->line;
	// 		msh->line = ft_magic_malloc(ADD, 0, ft_strjoin(msh->line, " ")); // MALLOC
	// 		ft_magic_malloc(FREE, 0, tmp);
	// 		// IF ERROR MALLOC
	// 		// free(tmp);
	// 		tmp = msh->line;
	// 		msh->line = ft_magic_malloc(ADD, 0, ft_strjoin(msh->line, new_line)); // MALLOC
	// 		// IF ERROR MALLOC
	// 		// free(tmp);
	// 		ft_magic_malloc(FREE, 0, tmp);
	// 		// tmp = NULL;
	// 		ft_magic_malloc(FREE, 0, new_line);
	// 	}
	// 	else
	// 		msh->line = new_line;
	// 	// free(new_line);
	// 	// new_line = NULL;
	// 	add_history(msh->line); //voir comment supprimer derniere lg de l historique et remplacer par la nouvelle ici
	// 	ft_parse_line(msh); // MALLOC
	// 	// IF ERROR MALLOC
	// 	ft_parse_bis(msh); // MALLOC
	// 	// IF ERROR MALLOC
	// 	msh->av = ft_split_msh(msh->line); //malloc
	// 	if (!msh->av)
	// 	{
	// 		ft_magic_malloc(FLUSH, 0, NULL);
	// 		// free(msh->line);
	// 		return (128 + 6); //6 = SIGABRT =>Verifier si signal ok
	// 	}
	// 	msh->ac = ft_structtablen(msh->av); // A DAPTER A ft_split_minishell qui renvoie un tableau de struct dont la derniere data == NULL
	// 	if (msh->ac == 0)
	// 	{
	// 		// free(msh->line);
	// 		// ft_free_split_msh(msh->av);
	// 		ft_magic_malloc(FLUSH, 0, NULL);
	// 		return (1) ; //Verif quel exitstatus
	// 	}
	// 	ft_token(msh);
	// 	// printf("msh->ac = %d\n", msh->ac);
	// 	if (ft_parse_ter(msh) != 0)
	// 	{
	// 		// free(msh->line);
	// 		// ft_free_split_msh(msh->av);
	// 		ft_magic_malloc(FLUSH, 0, NULL);
	// 		return (1) ; //Verif quel exitstatus
	// 	}
	// }
	return (0);
}



