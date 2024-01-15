/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:09:00 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:36:07 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

/*
minishell.c
*/
void	ft_minishell(t_msh *msh, int sub, t_fdpar *fd);

/*
exec.c
*/
int		ft_exec(t_msh *msh, int sub, t_fdpar *fd);

/*
exec_utils
*/
void	ft_child_exec(t_msh *msh);
void	ft_parent(t_msh *msh, int fd_1, int fd_2, int rule);

/*
exec_utils_bis
*/
char	**ft_transcript_env(t_env **env, t_msh *msh);
void	ft_close_fd(t_fdpar *fd, int rule, int fd1, int fd2);
int		ft_dup_fd(t_msh *msh, int rule);

/*
exec_utils_ter
*/
void	ft_parse(t_msh *msh, int sub);
char	**ft_research_path(t_msh *msh, t_env **env, int sub);
int		ft_access_cmd(t_msh *msh, char **ok_path);

/*
exec_utils_four
*/
int		ft_search_pipe(t_msh *msh);

/*
exec_par.c
*/
void	ft_exec_par(t_msh *msh, t_split **head, int sub, t_fdpar *fd);

/*
exec_par_utils.c
*/
void	ft_handle_hdoc(t_msh *msh, t_env *head_hd);
void	ft_skip_subelem(t_msh *msh, t_env *head_hd, t_split **head);

/*
exec_cmd.c
*/
int		ft_cmd_alone(t_msh *msh, int sub);

/*
exec_make_cmd.c
*/
char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2);

/*
exec_make_cmd_utils.c
*/
char	**ft_realloc_cmd(t_msh *msh, int *cmd_nb, int *i, char **tmp);
char	**ft_handle_expand(t_msh *msh, int *i, int *cmd_nb);
void	ft_reset_msh_av(t_msh *msh, char *data);

/*
exec_stdin.c
*/
int		redef_stdin(t_msh *msh, int rule, int j, int sub);

/*
exec_stdin_utils.c
*/
int		ft_invalid_infile(t_msh *msh, int rule, int j, t_head *save);
int		ft_dup_stdin(t_msh *msh, t_fd *fd, int rule, int j);
void	ft_exit_stdin_error_malloc(t_msh *msh, int rule, int j, int sub);

/*
exec_stdout.c
*/
int		redef_stdout(t_msh *msh, int rule, int j, int sub);

/*
exec_stdin_utils.c
*/
int		ft_invalid_outfile(t_msh *msh, int rule, int j, t_head *save);
int		ft_dup_stdout(t_msh *msh, t_fd *fd, int rule, int j);
void	ft_exit_stdout_error_malloc(t_msh *msh, int rule, int j, int sub);

/*
exec_std_utils.c
*/
void	ft_next(t_msh *msh, t_head *save);
void	ft_init_var_std(t_head *save, t_fd *fd, t_msh *msh);

/*
exec_exit.c
*/
void	ft_exit(int fd_1, int fd_2, int fd_3, t_msh *msh);
void	ft_exit_bis(t_msh *msh, int sub, int fd1, int fd2);

/*
exec_error.c
*/
int		ft_perr(t_msh *msh, int err, char *cmd);

/*
ft_expand_wildcard
*/
char	**ft_expand_wildcard(t_msh *msh, int *cmd_nb, int *i);

/*
ft_expand.c
*/
char	*ft_expand(t_msh *msh, char *cmd, int rule);

/*
ft_expand_utils.c
*/
int		valide_expand(char *key);
char	*get_value(t_msh *msh, t_env **env, t_expand *e, int rule);
char	*ft_add_char(t_msh *msh, char *cmd, t_expand *e, int *j);
char	*ft_do_expand(t_msh *msh, t_expand *e, char *cmd, int rule);
char	*ft_expand_valid_var(t_msh *msh, char *cmd, int rule, t_expand *e);

/*
pipex.c
*/
int		pipex_multi(t_msh *msh, int sub);

/*
pipex_handle_pipe.c
*/
void	ft_first_cmd(t_msh *msh, t_lpid **pid_l);
void	ft_middle_cmd(t_msh *msh, int j, t_lpid **pid_l);
void	ft_last_cmd(t_msh *msh, int j, t_lpid **pid_l);

/*
lpid_utils.c
*/
t_lpid	*ft_lpidnew(int content);
void	ft_lpidadd_back(t_lpid **lst, t_lpid *new);

/*
heredoc.c
*/
int		ft_heredoc(t_msh *msh);

/*
heredoc_utils.c
*/
t_env	*ft_copy_heredoc(t_msh *msh, t_env *hdoc, int sub);
void	ft_expand_hdoc(t_msh *msh, char *line, int fd, t_expand *e);

/*
heredoc_utils_bis.c
*/
t_env	*ft_lst_new_heredoc(t_msh *msh, char *lim, int sub);
int		ft_sigint_hdoc(t_msh *msh, int fd);
int		ft_handle_ctrld_hdoc(t_msh *msh, int fd, char *lim);
int		ft_check_unique_name(char *tmp, t_env **hdoc);
void	ft_unlink_heredoc(t_env *hdoc);

/*
split_magic_malloc.c
*/
char	**ft_split_magic_malloc(t_msh *msh, int sub, char const *s, char c);

/*
split_magic_malloc_isspace
*/
int		ft_countword(const char *s);
char	**ft_split_isspace_magic_malloc(t_msh *msh, char const *s);

/*
ft_strtrim_msh.c
*/
char	*ft_strtrim_msh(t_msh *msh, char **s1, int sub);

/*
wildcards_utils.c
*/
char	*ft_strjoin3(t_msh *msh, char *s1, char *s2);
int		is_flag_a(char *cmd);
char	**make_in_order(char **str);

/*
wildcards.c
*/
char	**wildcards(char *pattern, t_msh *msh, char *cmd_0, char *cmd_1);
int		ft_strcmp_cas(char *s1, char *s2);

#endif