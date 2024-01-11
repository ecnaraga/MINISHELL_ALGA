/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:43:27 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 15:56:31 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../includes/minishell.h"

/*
ft_parse.c
*/
char		*add_spaces(t_msh *msh, char *str);
char		*ft_parse_bis_bis(t_msh *msh, char *str);

/*
ft_parse_str_bis
*/
int			is_operator(char c);
int			ft_parse_line(t_msh *minish);

/*
ft_parse_par.c
*/
int			ft_parse_par(t_msh *minish);

/*
ft_parse_par_utils.c
*/
int			ft_count_char(char *str);
void		ft_pass_quote(t_msh *msh, t_par *p, int *i, char c);
int			ft_other_char(t_msh *msh, t_par *p, int *i);

/*
ft_parse_par_storage.c
*/
t_storage	ft_storage(int c);

/*
ft_parse_error_token.c
*/
int			ft_parse_error_token(t_msh *msh);

/*
ft_parse_error_token_utils.c
*/
char		*ft_error_message_final(char *str, t_msh *msh);
char		*e_chev(char *str, int skip, t_msh *msh);
char		*ft_err_op(char *str, int skip, t_msh *msh);
int			ft_same_char(char *str);

/*
ft_parse_utils.c
*/
int			ft_isspace(char c);
void		ft_inc_quote(char c, int *d_q, int *s_q);

/*
ft_error_message_syntax.c
*/
int			e_syntx(char *str);
char		*ft_error_message(char *str, t_msh *msh);

/*
ft_split_minish.c
*/
t_split		*ft_split_msh(char const *s, t_msh *msh);

/*
ft_split_minish_utils.c
*/
int			ft_test(char c, const char *c1, const char *cm1, t_quote *q);
int			ft_test_bis(char c, int d_q, int s_q);
void		ft_alloc_type(t_split *new, t_msh *msh, int l);
void		ft_count_dlw(const char *s, int *i, t_quote *q, t_letter *l);

/*
ft_split_minish_utils_bis.c
*/
t_letter	ft_count_letter(const char *s, t_quote *q, int *i);

/*
ft_strlcpy_minish.c
*/
void		ft_strlcpy_msh(t_split *strs, const char *src, size_t size,
				int begin);

/*
ft_strlcpy_minish_utils.c
*/
int			ft_inc_d(t_split *strs, int *d, char c);
void		ft_dollar(t_split *strs, const char *src, t_index *x, t_quote q);

/*
ft_split_minish_lst.c
*/
t_split		*ft_lstlast_split(t_split *lst);
void		ft_lstadd_back_split(t_split **lst, t_split *new);
t_split		*ft_lstnew_split(t_msh *msh, t_quote q);
int			ft_lstsize_split(t_split *lst);

/*
ft_split_minish_lst_bis.c
*/
void		del_two(t_msh *msh, t_split *lst);
void		ft_lstdelone_split(t_msh *msh, t_split *lst, void (*del)(t_msh *msh,
					t_split *));
t_split		*lstdel_relink_split(t_msh *msh, t_split *av, t_split *prev,
				t_split **head);

/*
ft_parse_token.c
*/
void		ft_token(t_msh *msh);

#endif
