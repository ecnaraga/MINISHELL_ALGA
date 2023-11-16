/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_ter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:16:50 by galambey          #+#    #+#             */
/*   Updated: 2023/11/16 15:02:12 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

char	*ft_error_message_chevron(char *str, int skip)
{
	int	i;
	int	j;
	char c;
	char tmp_bis[4];
	char *tmp;
	char *message;

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
			if (str[i] && i == 1)
				i++;
		}
	}
	if (str[i])
		c = str[i];
	while (str[i] && ((c == '>' && str[i] == c && j < 2) || (c == '<' && str[i] == c && j < 3)))
		tmp_bis[j++] = str[i++];
	if (str[i] && str[i] != c && j == 1)
		tmp_bis[j++] = str[i++];
	tmp_bis[j] = '\0';
	if (!tmp_bis[0])
	{
		message = ft_strdup("minishell: syntax error near unexpected token `newline'\n"); //MALLOC
		if (!message)
			return (NULL);
	}
	else
	{
		message = ft_strjoin("minishell: syntax error near unexpected token `", tmp_bis); //MALLOC
		if (!message)
			return (NULL);
		tmp = message;
		message = ft_strjoin(message, "'\n");
		free(tmp);
		if (!message)
			return (NULL);
	}
	return (message);
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
	int	j;

	i = -1;
	j = 0;
	// if (msh->av[0].token == CHEVRON && msh->ac == 1)
	// 	return (ft_error_syntax(ft_error_message_chevron(msh->av[0].data, 1), 2, 1)); // POUR GARANCE : ajouter secu dans ft_error syntax si malloc failed dans message chevron + dans fonction appelent parse_ter
	// if (msh->av[0].token == OPERATOR || msh->av[])
	while (msh->av[++i].data)
	{
		if (msh->av[i].token == CHEVRON && (ft_strlen(msh->av[i].data) > 2 || ft_same_char(msh->av[i].data) == 1))
			return (ft_error_syntax(ft_error_message_chevron(msh->av[i].data, 1), 2, 1));
		if (msh->av[i + 1].data && msh->av[i].token == CHEVRON && msh->av[i + 1].token == CHEVRON)
			return (ft_error_syntax(ft_error_message_chevron(msh->av[i + 1].data, 0), 2, 1));
		if (msh->av[i + 1].data && msh->av[i].token == OPERATOR && msh->av[i + 1].token == OPERATOR)
			return (ft_error_syntax("TO_MUCH OPERATOR\n", 2, 0));
	}
	return (0);
}
