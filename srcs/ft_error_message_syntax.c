/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_message_syntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2023/12/21 15:00:14 by galambey         ###   ########.fr       */
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

char	*ft_error_message(char *str, t_msh *msh)
{
	int	i;
	char *message;

	i = -1;
	if (!str[0])
	{
		message = mlcgic(mlcp(ft_strdup("minishell: syntax error near unexpected token `newline'\n"), 1), ADD, NO_ENV, msh);
		// message = ft_magic_malloc(ADD, 0, ft_strdup("minishell: syntax error near unexpected token `newline'\n"), NO_ENV); // SI MALLOC KO ON QUITTE
		if (!message)
			ft_exit(-1, -1, -1, msh);
	}
	else
	{
		while (str[++i])
			if (ft_isspace(str[i]) == 0)
				break ;
		str[i] = '\0';
		message = mlcgic(mlcp(ft_strjoin("minishell: syntax error near unexpected token `", str), 1), ADD, NO_ENV, msh);
		// message = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: syntax error near unexpected token `", str), NO_ENV);  // SI MALLOC KO ON QUITTE
		if (!message)
			ft_exit(-1, -1, -1, msh);
		message = mlcgic(mlcp(ft_strjoin(message, "'\n"), 1), ADD, NO_ENV, msh);
		// message = ft_magic_malloc(ADD, 0, ft_strjoin(message, "'\n"), NO_ENV);  // SI MALLOC KO ON QUITTE
		if (!message)
			ft_exit(-1, -1, -1, msh);
	}
	return (message);
}
