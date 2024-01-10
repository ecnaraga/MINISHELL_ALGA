/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_error_token_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 12:36:22 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 13:34:26 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_error_message_final(char *str, t_msh *msh)
{
	char	*e;
	char	*t;

	t = ft_strdup("minishell: syntax error near unexpected token `newline'\n");
	if (!str[0])
	{
		e = mcgic(mlcp(t, 1), ADD, NO_ENV, msh);
		if (!e)
			ft_exit(-1, -1, -1, msh);
	}
	else
	{
		free(t);
		e = ft_strjoin("minishell: syntax error near unexpected token `", str);
		e = mcgic(mlcp(e, 1), ADD, NO_ENV, msh);
		if (!e)
			ft_exit(-1, -1, -1, msh);
		e = mcgic(mlcp(ft_strjoin(e, "'\n"), 1), ADD, NO_ENV, msh);
		if (!e)
			ft_exit(-1, -1, -1, msh);
	}
	return (e);
}

char	*e_chev(char *str, int skip, t_msh *msh)
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
	return (tmp[j] = '\0', ft_error_message_final(tmp, msh));
}

char	*ft_err_op(char *str, int skip, t_msh *msh)
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
	return (ft_error_message_final(str + i, msh));
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
