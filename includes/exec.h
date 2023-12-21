/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:09:00 by galambey          #+#    #+#             */
/*   Updated: 2023/12/21 15:20:01 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

/*exec.c*/
int	ft_exec(t_msh *msh, int sub);

/*exec_utils.c*/
void	ft_parse(t_msh *msh, int sub);
char	**ft_research_path(t_msh *msh, t_env **env, int sub);
int	ft_access_cmd(t_msh *msh, char **ok_path);
// int	ft_access_cmd(t_msh *msh, char **path, char *cmd, char **good_path);
char **ft_transcript_env(t_env **env, char *str, t_msh *msh);
int redef_stdin(t_msh *msh, int rule, int j, int sub);
int	redef_stdout(t_msh *msh, int rule, int j, int sub);
char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2);

/* exec_utils_bis */                                      // NORME OK
int		ft_search_builtin(t_msh *msh);
void	ft_child_exec(t_msh *msh);
void	ft_parent(t_msh *msh, int fd_1, int fd_2, int rule);

/*exec_error.c*/
void	ft_exit(int fd_1, int fd_2, int fd_3, t_msh *msh);
void	ft_exit_bis(t_msh *msh, int sub, int fd1, int fd2);
int	ft_perr(t_msh *msh, int err, char *cmd);

/*exec_cmd.c*/
int	ft_cmd_alone(t_msh *msh, int sub);

/*pipex.c*/                                              // NORME OK
int	pipex_multi(t_msh *msh, int sub);

/* pipex_handle_pipe.c */                                // NORME OK
void	ft_first_pipe(t_msh *msh, t_lpid **pid_l);
void	ft_middle_pipe(t_msh *msh, int j, t_lpid **pid_l);
void	ft_last_pipe(t_msh *msh, int j, t_lpid **pid_l);

/* lpid_utils.c */
t_lpid	*ft_lpidnew(int content);
t_lpid	*ft_lpidlast(t_lpid *lst);
void	ft_lpidadd_back(t_lpid **lst, t_lpid *new);

/*heredoc.c*/
int	ft_heredoc(t_msh *msh);
t_env	*ft_copy_heredoc(t_msh *msh, t_env *hdoc, int sub);
void	ft_unlink_heredoc(t_env *hdoc);
char	*get_next_line_magic(int fd);

/*heredoc_lst_utils.c*/
t_env	*ft_lst_new_heredoc(t_msh *msh, char *lim, int sub);

/*split_magic_malloc.c*/
char	**ft_split_magic_malloc(t_msh *msh, int sub, char const *s);

/*split_magic_malloc_isspace*/
int	ft_countword(const char *s);
char	**ft_split_isspace_magic_malloc(t_msh *msh, char const *s);
// char	**ft_split_isspace_magic_malloc(char const *s);

void	ft_minishell(t_msh *msh, int sub);
char	*ft_strtrim_msh(t_msh *msh, char **s1, int sub);
void ft_exec_par(t_msh *msh, t_split **head, int rule, int sub);
char	*ft_expand(t_msh *msh, char *cmd, int rule);

#endif