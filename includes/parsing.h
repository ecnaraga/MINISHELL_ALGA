/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:43:27 by galambey          #+#    #+#             */
/*   Updated: 2023/11/13 11:44:47 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

int		ft_structtablen(t_split *tab);
int		ft_expand(t_msh *minish);
void	*ft_free_split_msh(t_split *strs);

/*
ft_parse.c
*/
int		ft_parse_line(t_msh *minish);

/*
ft_parse_bis
*/
int		ft_parse_bis(t_msh *minish);
int 	ft_error_syntax(char *str, int exitstatus, int clean);


/*
ft_parse_utils.c
*/
int		ft_is_isspace(char c); // remplacer par ft_isspace x norme (nom trop long)
int		ft_isspace(char c);
void	ft_inc_quote(char c, int *d_q, int *s_q);

/*
ft_split_minish.c
*/
t_split	*ft_split_msh(char const *s); //MALLOC

/*
ft_split_minish_utils.c
*/
void	*ft_free_strs(t_split *strs, int j);
int		ft_test(char c, char c1, char cm1, t_quote *q);
int		ft_test_bis(char c, int d_q, int s_q);

/*
ft_strlcpy_minish.c
*/
void	ft_strlcpy_msh(t_split *strs, const char *src, size_t size, int begin);

/*
ft_strlcpy_minish_utils.c
*/
int		ft_inc_d(t_split *strs, int *d, char c);
void	ft_dollar(t_split *strs, const char *src, t_index *x, t_quote q);

#endif