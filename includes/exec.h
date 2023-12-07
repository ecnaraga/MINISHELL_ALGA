/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:09:00 by galambey          #+#    #+#             */
/*   Updated: 2023/12/07 15:19:35 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

/*exec_cmd.c*/
int	ft_cmd_alone(t_msh *msh);

/*ft_exec.c*/
int	ft_exec(t_msh *msh, int sub);

/*pipex.c*/
int	pipex_multi(t_msh *msh);
void	ft_parse(t_msh *msh);

/*ft_pipex_utils.c*/
void	ft_first_pipe(t_msh *msh);
void	ft_middle_pipe(t_msh *msh, int j);
void	ft_last_pipe(t_msh *msh, int j);
void	ft_child_exec(t_msh *msh);
void	ft_parent(t_msh *msh, int fd_1, int fd_2);

/*exec_utils.c*/
char	**ft_research_path(t_env **env);
// char	**ft_research_path(t_list **env);
int	ft_access_cmd(char **path, char *cmd, char **good_path);
int	redef_stdin(t_msh *msh, int rule, int j);
// void	redef_stout(t_msh *msh, int rule, int j);
int	redef_stout(t_msh *msh, int rule, int j);
char	**ft_make_cmd(t_msh *msh);

/*exec_error.c*/
void	ft_exit(int fd_1, int fd_2, int fd_3);
void	ft_perr(int err, char *cmd);

/*heredoc.c*/
int	ft_heredoc(t_msh *msh);
void	ft_unlink_heredoc(t_list *heredoc);

/*split_magic_malloc.c*/
char	**ft_split_magic_malloc(char const *s, char c);


int	ft_minishell(t_msh *msh, int sub);
char	*ft_strtrim_msh(char **s1);
int ft_exec_par(t_msh *msh, t_split **head, int rule);
char	*ft_expand(t_msh *msh, char *cmd);

#endif