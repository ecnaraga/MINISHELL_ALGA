/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:25:02 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 10:59:20 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../includes/minishell.h"

/*
env
*/
void	builtin_env(t_msh *minish);

/*
echo
*/
void	builtin_echo(t_msh *msh/*t_split *av , int nb_arg */);

/*
pwd
*/
int	builtin_pwd();

/*
cd
*/
int	builtin_cd(t_msh *minish);

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
void	ft_exstrlcpy(char *dst, const char *src, size_t size);
void	order_export_env(t_env **export_env);

/*
export
*/
void	builtin_export(t_msh *minish);
void	ft_print_export(t_msh *minish);
int	get_statut(char *cmd);
int	valide_key(char *key);
int	new_env_node(char *str, int statut, t_env **env, int info);

/*
unset
*/
void	builtin_unset(t_msh *minish);
void	del_env(char *cmd, t_msh *minish);
#endif