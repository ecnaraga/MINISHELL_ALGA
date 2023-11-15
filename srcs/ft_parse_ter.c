/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_ter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:16:50 by galambey          #+#    #+#             */
/*   Updated: 2023/11/15 14:03:48 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

char	*ft_error_message_bis(char *str, int len_token)
{
	int	i;
	char *tmp;
	char *message;

	i = -1;
	if (!str[i])
		message = ft_strdup("minishell: syntax error near unexpected token `newline'\n");
	//IF ERROR
	else
	{
		str[len_token] = '\0';
		message = ft_strjoin("minishell: syntax error near unexpected token `", str);
		//IF ERROR
		tmp = message;
		message = ft_strjoin(message, "'\n");
		//IF ERROR
		free(tmp);
	}
	return (message);
}



int	ft_parse_ter(t_msh *msh)
{
	int	i;
	int	j;
	char *line;
	char *temp;
	char chev;

	i = -1;
	j = 0;
	if (msh->av[0].token == CHEVRON && msh->ac == 0)
	{
		if (ft_strlen(msh->av[0].data) == 2)
			return (ft_error_syntax(ft_error_message_bis(NULL, 0), 2, 1));
		else
		{
			chev = msh->av[0].data[0];
			while (msh->av[0].data[j] == chev && j < 3)
				j++;
			if (j >= 2 && chev == '>')
				ft_error_syntax(ft_error_message_bis(">>", 2), 2, 1);
			if (j >= 3 && chev == '<')
				ft_error_syntax(ft_error_message_bis("<<<", 3), 2, 1);
			if (j == 2 && chev == '<')
				ft_error_syntax(ft_error_message_bis("<<", 2), 2, 1);
			// if (j == 1 && )
		}
	}
	// while (msh->av[++i].data)
	// {
	// 	if (msh->av[i].token == CHEVRON && ft_strlen(msh->av[i].data) > 2)
	// 	{
	// 		// if (msh->av[i].data[0] == '<')
	// 		// {
	// 		// 	while (j < 3 && msh->av[i].data[j] == '<')
	// 		// 		j++;
	// 		// 	return (ft_error_syntax(ft_error_message_bis(msh->av[i].data, j), 2, 1));
	// 		// }
	// 		// if (msh->av[i].data[0] == '>')
	// 		// {
	// 		// 	while (j < 2 && msh->av[i].data[j] == '>')
	// 		// 		j++;
	// 		// 	return (ft_error_syntax(ft_error_message_bis(msh->av[i].data, j), 2, 1));
	// 		// }
	// 	}
	// 	if (msh->av[i + 1].data && msh->av[i].token == CHEVRON && msh->av[i + 1].token == CHEVRON)
	// 	// {
	// 	// 	if (ft_strlen(msh->av[i + 1].data) > 3)
	// 	// 	{
	// 	// 		line = ft_strjoin("minishell: syntax error near unexpected token `", msh->av[i + 1].data);
	// 	// 		temp = line;
	// 	// 		line = ft_strjoin(line, "'\n");
	// 	// 		free(temp);
	// 	// 	}
	// 	// 	else if (msh->av[i + 1].data[0] == '<')
	// 	// 		line = ft_strdup("minishell: syntax error near unexpected token `<<<'");
	// 	// 	return (ft_error_syntax(line, 2, 1));
	// 	// }
		
	// }
	return (0);
}
