/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:25:02 by galambey          #+#    #+#             */
/*   Updated: 2024/01/11 15:07:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

/*
buitin.c
*/
int			ft_search_builtin(t_msh *msh);

/*
env
*/
void		builtin_env(t_msh *minish);

/*
echo
*/
void		builtin_echo(t_msh *msh);

/*
pwd
*/
int			builtin_pwd(t_msh *msh);

/*
cd
*/
int			builtin_cd(t_msh *minish);

int			change_env(char *old_pwd, t_msh *msh, int statut);
char		*get_path(char **str);
char		*get_old_pwd(t_env **env, t_msh *msh);
int			pwd_exist(t_env **env);

/*
exit
*/
int			builtin_exit(t_msh *minish);
long long	ft_atol(char *nptr, int *err);

/*
get_env
*/
t_env		**get_env(char **str, t_msh *msh);
int			get_name_size(char *str);

t_env		*ft_lstlast_env(t_env *lst);
int			ft_lstsize_env(t_env *lst);
void		ft_lstadd_back_env(t_env **lst, t_env *new);
t_env		*ft_lst_new_malloc(t_msh *msh, size_t size, size_t size2);
int			check_env(char **str);

/*
get_export_env
*/
t_env		**get_export_env(t_msh *msh, char **str);
void		ft_exstrlcpy(char *dst, const char *src, size_t size);
void		order_export_env(t_env **export_env);

/*
export
*/
void		builtin_export(t_msh *minish);
int			new_env_node_env(t_msh *msh, char *str, int statut, t_env **env);
int			new_env_node_export(t_msh *msh, char *str, int statut, t_env **env);

void		ft_print_export(t_msh *minish);
int			get_statut(char *cmd);
int			valide_key(char *key);
int			node_exist(t_env **env, char *str, int size, t_msh *msh);
void		error_export(t_msh *msh, char *str);

size_t		ft_strlen2(const char *s);
char		*ft_strjoin2(char const *s1, char const *s2, t_msh *msh);
char		*ft_exstrjoin(t_msh *msh, char const *s1, char const *s2);
void		strlcpy_enjoyer(char *str, t_env *node, int statut, int size);
void		change_env_export(t_env *node, t_env **env, int info);

int			doublon_handler(t_msh *m, char *str, t_env **env, t_intel i);
/*
unset
*/
void		builtin_unset(t_msh *minish, int rule);
void		del_env(char *cmd, t_msh *minish, int rule);

#endif