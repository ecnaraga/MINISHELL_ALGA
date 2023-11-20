/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_ter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:16:50 by galambey          #+#    #+#             */
/*   Updated: 2023/11/20 15:27:29 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

char	*ft_error_message_final(char *str)
{
	char *tmp;
	char *message;

	if (!str[0])
		message = ft_strdup("minishell: syntax error near unexpected token `newline'\n"); //MALLOC
	else
	{
		message = ft_strjoin("minishell: syntax error near unexpected token `", str); //MALLOC
		if (!message)
			return (NULL);
		tmp = message;
		message = ft_strjoin(message, "'\n");
		free(tmp);
	}
	return (message); //en cas d erreur de malloc du IF ou du dernier malloc du ELSE message == NULL dans pas de secu
}

char	*ft_error_message_chevron(char *str, int skip)
{
	int	i;
	int	j;
	char c;
	char tmp[4];

	i = 0;
	j = 0;
	c = str[0];
	if (skip == 1)
	{
		if (c == '>')
			while(str[++i] && str[i] == c && i < 2);
		else if (c == '<')
		{
			while(str[++i] && str[i] == c && i < 3);
			// if (str[i] && i == 1)
			// 	i++;
		}
	}
	if (str[i])
		c = str[i];
	while (str[i] && ((c == '>' && str[i] == c && j < 2) || (c == '<' && str[i] == c && j < 3)))
		tmp[j++] = str[i++];
	if (str[i] && str[i] != c && j == 1)
		tmp[j++] = str[i++];
	tmp[j] = '\0';
	return (ft_error_message_final(tmp)); // retour NULL de malloc foire dans error_message_final a proteger dans ft_error syntax
}

char	*ft_error_message_operator(char *str, int skip)
{
	int i;

	i = 0;
	if (skip == 1)
	{
		if (str[0] == str[1])
			i = 2;
		else
			i = 1;
	}
	if (!str[i + 1] || (str[i + 1] && ((skip == 0 && str[i] == '&' && str[i + 1] == '|') || (skip == 1 && str[i] != str[i + 1]))))
		str[i + 1] = '\0';
	else if (str[i + 1])
		str[i + 2] = '\0';
	return (ft_error_message_final(str + i)); // retour NULL de malloc foire dans error_message_final a proteger dans ft_error syntax
}

int	ft_same_char(char *str)
{
	char 	c;
	int		i;

	c = str[0];
	i = -1;
	while (str[++i] && str[i] == c);
	if (str[i])
		return (1);
	return (0);
}



int	ft_parse_ter(t_msh *msh)
{
	int	i;

	i = -1;
	printf("PARSE_TER\n");
	if (msh->av[0].token == CHEVRON && msh->ac == 1)
		return (ft_error_syntax(ft_error_message_chevron(msh->av[0].data, 1), 2, 1)); // POUR GARANCE : ajouter secu dans ft_error syntax si malloc failed dans message chevron + dans fonction appelent parse_ter
	if (msh->av[0].token == OPERATOR /*|| msh->av[msh->ac - 1].token == OPERATOR*/)
		return (ft_error_syntax(ft_error_message_operator(msh->av[0].data, 0), 2, 1));
	while (msh->av[++i].data)
	{
		/*CHEVRON*/
		if (msh->av[i].token == CHEVRON && (ft_strlen(msh->av[i].data) > 2 || ft_same_char(msh->av[i].data) == 1))
			return (ft_error_syntax(ft_error_message_chevron(msh->av[i].data, 1), 2, 1));
		if (msh->av[i + 1].data && msh->av[i].token == CHEVRON && msh->av[i + 1].token == CHEVRON)
			return (ft_error_syntax(ft_error_message_chevron(msh->av[i + 1].data, 0), 2, 1));
		if (msh->av[i + 1].data && msh->av[i].token == CHEVRON && msh->av[i + 1].token == OPERATOR)
			return (ft_error_syntax(ft_error_message_operator(msh->av[i + 1].data, 0), 2, 1));
		
		/*OPERATOR*/
		if (msh->av[i].token == OPERATOR && (ft_strlen(msh->av[i].data) > 2 || ft_same_char(msh->av[i].data) == 1))
			return (ft_error_syntax(ft_error_message_operator(msh->av[i].data, 1), 2, 1));
		if (msh->av[i + 1].data && msh->av[i].token == OPERATOR && msh->av[i + 1].token == OPERATOR)
			return (ft_error_syntax(ft_error_message_operator(msh->av[i].data, 0), 2, 1));
	}
	// if (msh->av[msh->ac - 1].token == OPERATOR) // si se termine par un operateur -> rester a l ecoute
	// 	return (ft_error_syntax(ft_error_message_operator(msh->av[msh->ac - 1].data, 0), 2, 1));
	if (msh->av[msh->ac - 1].token == CHEVRON)
		return (ft_error_syntax(ft_error_message_chevron(msh->av[msh->ac - 1].data, 1), 2, 1));

	if (msh->av[msh->ac - 1].token == OPERATOR) // si se termine par un operateur -> rester a l ecoute
	{
		ft_free_split_msh(msh->av);
		char *line = readline("> ");
		char *tmp = msh->line;
		msh->line = ft_strjoin(msh->line, " "); // MALLOC
		// IF ERROR MALLOC
		free(tmp);
		tmp = msh->line;
		msh->line = ft_strjoin(msh->line, line); // MALLOC
		// IF ERROR MALLOC
		free(tmp);
		free(line);
		add_history(msh->line); //voir comment supprimer derniere lg de l historique et remplacer par la nouvelle ici
		ft_parse_line(msh); // MALLOC
		// IF ERROR MALLOC
		ft_parse_bis(msh); // MALLOC
		// IF ERROR MALLOC
		msh->av = ft_split_msh(msh->line); //malloc
		if (!msh->av)
		{
			free(msh->line);
			return (128 + 6); //6 = SIGABRT =>Verifier si signal ok
		}
		msh->ac = ft_structtablen(msh->av); // A DAPTER A ft_split_minishell qui renvoie un tableau de struct dont la derniere data == NULL
		if (msh->ac == 0)
		{
			free(msh->line);
			ft_free_split_msh(msh->av);
			return (1) ; //Verif quel exitstatus
		}
		ft_token(msh);
		// printf("msh->ac = %d\n", msh->ac);
		if (ft_parse_ter(msh) != 0)
		{
			free(msh->line);
			ft_free_split_msh(msh->av);
			return (1) ; //Verif quel exitstatus
		}
	}
	return (0);
}



