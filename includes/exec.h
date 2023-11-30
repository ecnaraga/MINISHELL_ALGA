/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:09:00 by galambey          #+#    #+#             */
/*   Updated: 2023/11/30 18:26:48 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

/*ft_exec.c*/
int	ft_exec(t_msh *msh);

/*pipex_multi.c*/
int	pipex_multi(t_msh *msh);

/*ft_pipex_multi_utils.c*/
void	ft_first_pipe(t_msh *msh);
void	ft_middle_pipe(t_msh *msh, int j);
void	ft_last_pipe(t_msh *msh, int j);

/*pipex_utils_bonus.c*/
char	**ft_research_path(t_list **env);
int	ft_access_cmd(char **path, char *cmd, char **good_path);
void	redef_stdin(t_msh *msh, int rule, int j);
void	redef_stout(t_msh *msh, int *fd_outfile);
char	**ft_make_cmd(t_msh *msh);


/*heredoc.c*/
void	ft_heredoc(t_msh *msh);

#endif