/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2023/11/13 11:40:46 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (strs[i].type)
			free(strs[i].type);
		i++;
	}
	free(strs);
	return (NULL);
}

/*
Teste les conditions de la boucle de ft_count_letter pour compter le nb de k
Renvoie 0 si ok pour rentrer dans la boucle, sinon 1
c : char courant dans la string
c1 : char + 1 dans la string
cm1 : char -1 dans la string
*/
int	ft_test(char c, char c1, char cm1, t_quote *q)
{
	if (!c)
		return (1);
	if (q->d % 2 == 1)
		return (0);
	if (q->s % 2 == 1)
		return (0);
	if (ft_isspace(c) == 0)
		return (1);
	if (c != '"' && c != 39)
		return (0);
	if (c == '"' && ((c1 && ft_isspace(c1) == 1)
			|| (cm1 && ft_isspace(cm1) == 1)))
		return (0);
	if (c == 39 && ((c1 && ft_isspace(c1) == 1)
			|| (cm1 && ft_isspace(cm1) == 1)))
		return (0);
	else
		return (1);
}

// /*
// Teste les conditions de la boucle de ft_count_letter pour compter le nb de k
// Renvoie 0 si ok pour rentrer dans la boucle, sinon 1
// */
// int	ft_test(char c, char c1, int d_q, int s_q)
// {
// 	if (!c)
// 		return (1);
// 	if (d_q % 2 == 1)
// 		return (0);
// 	if (s_q % 2 == 1)
// 		return (0);
// 	if (ft_isspace(c) == 0)
// 		return (1);
// 	if (c != '"' && c != 39)
// 		return (0);
// 	if (c == '"' && (c1 && ft_isspace(c1) == 1))
// 		return (0);
// 	if (c == 39 && (c1 && ft_isspace(c1) == 1))
// 		return (0);
// 	else
// 		return (1);
// }

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
