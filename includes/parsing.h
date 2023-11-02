/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:43:27 by galambey          #+#    #+#             */
/*   Updated: 2023/11/02 12:24:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

int		ft_structtablen(t_split *tab);
int		ft_expand(t_msh *minish);
void	*ft_free_split_minish(t_split *strs);

/*
ft_parse.c
*/
int		ft_parse_line(t_msh *minish);

/*
ft_parse_bis
*/
int		ft_parse_bis(t_msh *minish);
// int		ft_parse_bis(char *str);

/*
ft_parse_utils.c
*/
int		ft_is_isspace(char c);
void	ft_inc_quote(char c, int *d_q, int *s_q);

/*
ft_split_minish.c
*/
t_split	*ft_split_minish(char const *s); //MALLOC

/*
ft_split_minish_utils.c
*/
void	*ft_free_strs(t_split *strs, int j);
int		ft_test(char c, char c1, int d_q, int s_q);
int		ft_test_bis(char c, int d_q, int s_q);

/*
ft_strlcpy_minish.c
*/
void	ft_strlcpy_minish(char *dst, const char *src, size_t size, int begin);

#endif