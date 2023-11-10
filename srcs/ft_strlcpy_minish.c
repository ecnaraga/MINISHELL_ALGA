/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy_minish.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:17:26 by galambey          #+#    #+#             */
/*   Updated: 2023/11/10 17:24:23 by galambey         ###   ########.fr       */
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
Incremente les double quotes(d_q) si pas entre single quotes (s_q)
Incremente les single quotes(s_q) si pas entre double quotes (d_q)
Ps : 39 = single quote
*/
static void	ft_inc_quote_strlcpy(char c, int *d_q, int *s_q, char *f_q)
{
	if (*s_q % 2 == 0 && c == '"')
	{
		*d_q += 1;
		*f_q = '"';
	}
	else if (*d_q % 2 == 0 && c == 39)
	{
		*s_q += 1;
		*f_q = '\'';
	}
}

/*
Copie dans dst size - 1 char de src. Saute les quotes non compris entre quotes
	et non suivi/precede d'un isspace selon si fermant ou ouvrant
*/
void	ft_strlcpy_minish(t_split *strs, const char *src, size_t size, int begin)
{
	size_t	i;
	size_t	j;
	int		d;
	int		d_q;
	int		s_q;
	char	f_q; // possiblement pas besoin

	ft_init_var(&i, &j, &d_q, &s_q);
	f_q = 0;
	d = 0;
	if (begin > -1)
		ft_inc_quote_strlcpy(src[0], &d_q, &s_q, &f_q);
	if (size > 0)
	{
		while (src[i] && j < (size - 1))
		{
			ft_inc_quote_strlcpy(src[i], &d_q, &s_q, &f_q);
			if (src[i] == '$')
			{
				if (strs->type[d].expnd != TO_DEFINE)
				{
					if (src[i - 1] == '$')
						strs->type[d].expnd = MULTI_DOLLAR;
					if (src[i + 1] != '$') //revoir donc le comptage des dollars
						d++;
				}
				else if (s_q % 2 == 0 /*&& strs->type[d].expnd == TO_DEFINE*/)
					strs->type[d].expnd = EXPAND;
				else
					strs->type[d].expnd = NO_EXPAND;
				strs->type[d].len_variable = 0;
			}
			if (ft_test_ter(src[i], d_q, s_q) == 0)
			{
				// printf("src[%ld] = %c, d_q %d\n", i, src[i], d_q);
				// ft_inc_quote(src[i], &d_q, &s_q);
				if (strs->dollar > 0)
					printf("src[%ld] = %c, strs->type[%d].expnd = %d\n", i, src[i], d, strs->type[d].expnd);
				if (strs->type && d < strs->dollar && strs->type[d].expnd == EXPAND && (src[i] == '\'' || src[i] == '"' || ft_is_isspace(src[i]) == 0))
					d++;
				else if (strs->type && d < strs->dollar && strs->type[d].expnd == EXPAND)
					strs->type[d].len_variable += 1;
				if (ft_test_bis(src[i], d_q, s_q) == 0)
					strs->data[j++] = src[i++];
				else
				{
					// if (strs->type && d < strs->dollar && strs->type[d].expnd == EXPAND && (src[i] == '\'' || src[i] == '"' || ft_is_isspace(src[i]) == 0))
					// 	d++;
					i++;
				}
			}
			else
			{
				// if (strs->type && d < strs->dollar && strs->type[d].expnd == EXPAND && (src[i] == '\'' || src[i] == '"' || ft_is_isspace(src[i]) == 0))
				// 	d++;
				i++;
			}
		}
		strs->data[j] = '\0';
	}
}
