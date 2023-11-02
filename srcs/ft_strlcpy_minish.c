/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy_minish.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:17:26 by galambey          #+#    #+#             */
/*   Updated: 2023/11/01 16:17:38 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(size_t *i, size_t *j, int *d_q, int *s_q)
{
	*i = 1;
	*j = 0;
	*d_q = 0;
	*s_q = 0;
}

static int	ft_test_ter(char c, int d_q, int s_q)
{
	if (!c)
		return (1);
	if (d_q % 2 == 1)
		return (0);
	if (s_q % 2 == 1)
		return (0);
	if (c != '"' && c != 39 && ft_is_isspace(c) == 1)
		return (0);
	// printf("retour 1 x src[i] = %c\n", c);
	return (1);
}

/*
Copie dans dst size - 1 char de src. Saute les quotes non compris entre quotes
	et non suivi/precede d'un isspace selon si fermant ou ouvrant
*/
void	ft_strlcpy_minish(char *dst, const char *src, size_t size, int begin)
{
	size_t	i;
	size_t	j;
	int		d_q;
	int		s_q;

	ft_init_var(&i, &j, &d_q, &s_q);
	if (begin > -1)
		ft_inc_quote(src[0], &d_q, &s_q);
	if (size > 0)
	{
		while (src[i] && j < (size - 1))
		{
			ft_inc_quote(src[i], &d_q, &s_q);
			if (ft_test_ter(src[i], d_q, s_q) == 0)
			{
				// printf("src[%ld] = %c, d_q %d\n", i, src[i], d_q);
				// ft_inc_quote(src[i], &d_q, &s_q);
				// printf("src[%ld] = %c, d_q %d\n", i, src[i], d_q);
				if (ft_test_bis(src[i], d_q, s_q) == 0)
					dst[j++] = src[i++];
				else
					i++;
			}
			else
				i++;
		}
		dst[j] = '\0';
	}
}
