/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:43:27 by galambey          #+#    #+#             */
/*   Updated: 2023/11/23 17:35:42 by galambey         ###   ########.fr       */
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
char	*add_spaces(char *str);
int	is_operator(char c);

//char	*ft_parse_bis_bis(char *str);

/*
ft_parse_bis
*/
int		ft_parse_bis(t_msh *minish);
int 	ft_error_syntax(char *str);
char	*ft_error_message(char *str);

/*
ft_parse_ter.c
*/
int	ft_parse_ter(t_msh *msh);

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
int		ft_test(char c, const char *c1, const char *cm1, t_quote *q);
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

/*
ft_token.c
*/
void	ft_token(t_msh *msh);

/*
ft_magic_malloc.c
*/
void *ft_magic_malloc(int rule, size_t size, void *addr);

#endif