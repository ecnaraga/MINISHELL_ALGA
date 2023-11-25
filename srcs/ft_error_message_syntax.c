/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_message_syntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2023/11/25 14:18:22 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int err_syntax(char *str)
{
	if (!str)
		return (1);
	ft_putstr_fd(str, 2);
	return (1);
}

char	*ft_error_message(char *str)
{
	int	i;
	char *message;

	i = -1;
	if (!str[0])
		message = ft_magic_malloc(ADD, 0, ft_strdup
				("minishell: syntax error near unexpected token `newline'\n"));
	else
	{
		while (str[++i])
			if (ft_isspace(str[i]) == 0)
				break ;
		str[i] = '\0';
		message = ft_magic_malloc(ADD, 0, ft_strjoin
				("minishell: syntax error near unexpected token `", str));
		if (!message)
			return (NULL);
		message = ft_magic_malloc(ADD, 0, ft_strjoin(message, "'\n"));
	}
	return (message);
}
