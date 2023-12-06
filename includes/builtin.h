/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:25:02 by galambey          #+#    #+#             */
/*   Updated: 2023/12/06 11:26:36 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../includes/minishell.h"

/*
echo
*/
void	ft_echo(t_msh *msh/*t_split *av , int nb_arg */);

/*
pwd
*/
void	builtin_pwd(t_msh *minish);

/*
cd
*/
void	builtin_cd(t_msh *minish);

/*
get_env
*/
t_env	**get_env(char **str);
int	fill_env(t_env **env, char **str);
int	get_name_size(char *str);

t_env	*ft_lstlast_env(t_env *lst);
int	ft_lstsize_env(t_env *lst);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
t_env	*ft_lst_new_malloc(size_t size, size_t size2);
int	check_env(t_env **export_env, char **str);

/*
get_export_env
*/
t_env	**get_export_env(char **str);

/*
others
*/
void	ft_print_export(t_msh *minish);
int	get_statut(char *cmd);
int	valide_key(char *key);
//int	ft_strcmp(const char *s1, const char *s2);
//size_t	ft_strlen(const char *s);

#endif