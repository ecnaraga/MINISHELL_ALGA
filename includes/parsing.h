/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:43:27 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 16:43:57 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// # include "../libft/includes/libft.h"
# include "../includes/minishell.h"

#ifndef PARSING_H
# define PARSING_H

/*
ft_parse.c
*/
int		ft_parse_line(t_msh *minish);
char	*add_spaces(t_msh *msh, char *str);
int	is_operator(char c);
char	*ft_parse_bis_bis(t_msh *msh, char *str);

/*
ft_parse_bis.c
*/
int		ft_parse_bis(t_msh *minish);

/*
ft_parse_bis_utils.c
*/
int	ft_count_char(char *str);
void	ft_pass_quote(t_msh *msh, t_par *p, int *i, char c);
int	ft_other_char(t_msh *msh, t_par *p, int *i);

/*
ft_parse_bis_storage.c
*/
t_storage	ft_storage(int c);

/*
ft_parse_ter.c
*/
int	ft_parse_ter(t_msh *msh);

/*
ft_parse_utils.c
*/
int		ft_isspace(char c);
void	ft_inc_quote(char c, int *d_q, int *s_q);

/*
ft_error_message_syntax.c
*/
int 	err_syntax(char *str);
char	*ft_error_message(char *str, t_msh *msh);

/*
ft_split_minish.c
*/
t_split	*ft_split_msh(char const *s, t_msh *msh); //MALLOC

/*
ft_split_minish_utils.c
*/
// void	*ft_free_strs(t_split *strs, int j);
int		ft_test(char c, const char *c1, const char *cm1, t_quote *q);
int		ft_test_bis(char c, int d_q, int s_q);
void	ft_alloc_type(t_split *new, t_msh *msh, int l);

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
lst_split.c
*/
t_split	*ft_lstlast_split(t_split *lst);
void	ft_lstadd_back_split(t_split **lst, t_split *new);
t_split	*ft_lstnew_split(t_msh *msh, t_quote q);
int	ft_lstsize_split(t_split *lst);
void    del_two(t_msh *msh, t_split *lst);
void	ft_lstdelone_split(t_msh *msh, t_split *lst, void (*del)(t_msh *msh, t_split *));
t_split	*lstdel_relink_split(t_msh *msh, t_split *av, t_split *prev, t_split **head);
// t_env	*ft_lstdel_and_relink(t_msh *msh, t_env *lst, t_env *prev, t_env **head);


/*
ft_token.c
*/
void	ft_token(t_msh *msh);



#endif