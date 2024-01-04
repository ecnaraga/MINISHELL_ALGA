/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_message_syntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 12:27:18 by galambey         ###   ########.fr       */
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

char	*ft_build_message(t_msh *msh, char *str, int i)
{
	char *tmp;
	
	while (str[++i])
		if (ft_isspace(str[i]) == 0)
			break ;
	str[i] = '\0';
	tmp = ft_strjoin("minishell: syntax error near unexpected token `", str);
	tmp = mlcgic(mlcp(tmp, 1), ADD, NO_ENV, msh); // SI MALLOC KO ON QUITTE
	if (!tmp)
		ft_exit(-1, -1, -1, msh);
	tmp = mlcgic(mlcp(ft_strjoin(tmp, "'\n"), 1), ADD, NO_ENV, msh);  // SI MALLOC KO ON QUITTE
	if (!tmp)
		ft_exit(-1, -1, -1, msh);
	return (tmp);
}

char	*ft_error_message(char *str, t_msh *msh)
{
	int	i;
	char *message;

	i = -1;
	if (!str[0])
	{
		message = ft_strdup("minishell: syntax error near unexpected token `newline'\n");
		message = mlcgic(mlcp(message, 1), ADD, NO_ENV, msh); // SI MALLOC KO ON QUITTE
		if (!message)
			ft_exit(-1, -1, -1, msh);
	}
	else
		message = ft_build_message(msh, str, i); // SI MALLOC KO PROTEGR A L INTERIEUR
	return (message);
}
