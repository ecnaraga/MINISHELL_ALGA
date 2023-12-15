/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:09:00 by galambey          #+#    #+#             */
/*   Updated: 2023/12/15 14:47:04 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

/*exec_cmd.c*/
int	ft_cmd_alone(t_msh *msh, int sub);

/*ft_exec.c*/
int	ft_exec(t_msh *msh, int sub);

/*pipex.c*/
int	pipex_multi(t_msh *msh, int sub);
void	ft_parse(t_msh *msh, int sub);

/*ft_pipex_utils.c*/
void	ft_first_pipe(t_msh *msh);
void	ft_middle_pipe(t_msh *msh, int j);
void	ft_last_pipe(t_msh *msh, int j);
void	ft_child_exec(t_msh *msh);
void	ft_parent(t_msh *msh, int fd_1, int fd_2, int rule);

/*exec_utils.c*/
char	**ft_research_path(t_msh *msh, t_env **env, int sub);
// char	**ft_research_path(t_list **env);
int	ft_access_cmd(char **path, char *cmd, char **good_path);
char **ft_transcript_env(t_env **env, char *str);
int redef_stdin(t_msh *msh, int rule, int j, int sub);
// void	redef_stout(t_msh *msh, int rule, int j);
int	redef_stdout(t_msh *msh, int rule, int j, int sub);
char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2);

/*exec_error.c*/
void	ft_exit(int fd_1, int fd_2, int fd_3);
void	ft_exit_bis(t_msh *msh, int sub, int fd1, int fd2);
int	ft_perr(int err, char *cmd);

/*heredoc.c*/
int	ft_heredoc(t_msh *msh);
t_env	*ft_copy_heredoc(t_msh *msh, t_env *heredoc, int sub);
void	ft_unlink_heredoc(t_env *heredoc);

/*heredoc_lst_utils.c*/
t_env	*ft_lst_new_heredoc(t_msh *msh, char *lim, int sub);

/*split_magic_malloc.c*/
char	**ft_split_magic_malloc(t_msh *msh, int sub, char const *s, char c);


void	ft_minishell(t_msh *msh, int sub);
char	*ft_strtrim_msh(t_msh *msh, char **s1, int sub);
void ft_exec_par(t_msh *msh, t_split **head, int rule, int sub);
char	*ft_expand(t_msh *msh, char *cmd, int rule);

#endif