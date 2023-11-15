/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_ter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:16:50 by galambey          #+#    #+#             */
/*   Updated: 2023/11/15 16:53:27 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

char	*ft_error_message_bis(char *str, int len_token)
{
	int	i;
	char *tmp;
	char *message;

	i = -1;
	if (!str || !str[0])
		message = ft_strdup("minishell: syntax error near unexpected token `newline'\n");
	//IF ERROR
	else
	{
		printf("str |%s| len_token %d\n", str, len_token);
		str[len_token] = '\0';
		printf("str |%s|\n", str);
		message = ft_strjoin("minishell: syntax error near unexpected token `", str);
		//IF ERROR
		tmp = message;
		message = ft_strjoin(message, "'\n");
		//IF ERROR
		free(tmp);
	}
	printf("message |%s|\n", message);
	return (message);
}



int	ft_parse_ter(t_msh *msh)
{
	int	i;
	int	j;
	char chev;

	i = -1;
	j = 0;
	if (msh->av[0].token == CHEVRON && msh->ac == 1)
	{
		chev = msh->av[0].data[0];
		if (ft_strlen(msh->av[0].data) <= 2 || (chev == '<' && ft_strlen(msh->av[0].data) == 3))
			return (ft_error_syntax(ft_error_message_bis(NULL, 0), 2, 1));
		else
		{
			printf("test1\n");
			while (msh->av[0].data[j] == chev && j < 6)
				j++;
			if (j == 3 && chev == '>')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 1), 2, 1);
			if (j > 3 && chev == '>')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 2), 2, 1);
			if (j == 4 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 1), 2, 1);
			if (j == 5 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 2), 2, 1);
			if (j > 5 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 3), 2, 1);
			if (j == 2 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 2), 2, 1);
		}
	}
	while (msh->av[++i].data)
	{
		if (msh->av[i].token == CHEVRON && ft_strlen(msh->av[i].data) > 2)
		{
			while (msh->av[0].data[j] == chev && j < 6)
				j++;
			if (j == 3 && chev == '>')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 1), 2, 1);
			if (j > 3 && chev == '>')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 2), 2, 1);
			if (j == 4 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 1), 2, 1);
			if (j == 5 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 2), 2, 1);
			if (j > 5 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 3), 2, 1);
			if (j == 2 && chev == '<')
				ft_error_syntax(ft_error_message_bis(msh->av[0].data, 2), 2, 1);
		}
		// if (msh->av[i + 1].data && msh->av[i].token == CHEVRON && msh->av[i + 1].token == CHEVRON)
		// {
		// 	if (ft_strlen(msh->av[i + 1].data) > 3)
		// 	{
		// 		line = ft_strjoin("minishell: syntax error near unexpected token `", msh->av[i + 1].data);
		// 		temp = line;
		// 		line = ft_strjoin(line, "'\n");
		// 		free(temp);
		// 	}
		// 	else if (msh->av[i + 1].data[0] == '<')
		// 		line = ft_strdup("minishell: syntax error near unexpected token `<<<'");
		// 	return (ft_error_syntax(line, 2, 1));
		// }
		
	}
	return (0);
}
