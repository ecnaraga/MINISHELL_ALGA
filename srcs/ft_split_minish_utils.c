/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2023/11/01 16:16:57 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// /*
// Test si le char envoye est un isspace
// */
// int	ft_is_isspace(char c)
// {
// 	if (c == ' ' || (c >= '\t' && c <= '\r'))
// 		return (0);
// 	else
// 		return (1);
// }

// /*
// Incremente les double quotes(d_q) si pas entre single quotes (s_q)
// Incremente les single quotes(s_q) si pas entre double quotes (d_q)
// Ps : 39 = single quote
// */
// void	ft_inc_quote(char c, int *d_q, int *s_q)
// {
// 	if (*s_q % 2 == 0 && c == '"')
// 		*d_q += 1;
// 	else if (*d_q % 2 == 0 && c == 39)
// 		*s_q += 1;
// }

/*
Free en cas d 'erreur de malloc
*/
void	*ft_free_strs(t_split *strs, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(strs[i].data);
		i++;
	}
	free(strs);
	return (NULL);
}

/*
Teste les conditions de la boucle de ft_count_letter pour compter le nb de k
Renvoie 0 si ok pour rentrer dans la boucle, sinon 1
*/
int	ft_test(char c, char c1, int d_q, int s_q)
{
	if (!c)
		return (1);
	if (d_q % 2 == 1)
		return (0);
	if (s_q % 2 == 1)
		return (0);
	if (ft_is_isspace(c) == 0)
		return (1);
	if (c != '"' && c != 39)
		return (0);
	if (c == '"' && (c1 && ft_is_isspace(c1) == 1))
		return (0);
	if (c == 39 && (c1 && ft_is_isspace(c1) == 1))
		return (0);
	else
		return (1);
}

/*
Teste les conditions de ft_count_letter pour compter le nb de lt
Renvoie 0 si ok pour rentrer dans la boucle, sinon 1
*/
int	ft_test_bis(char c, int d_q, int s_q)
{
	if (c != '"' && c != 39)
		return (0);
	if (c == '"' && s_q % 2 == 1)
		return (0);
	if (c == 39 && d_q % 2 == 1)
		return (0);
	else
		return (1);
}
