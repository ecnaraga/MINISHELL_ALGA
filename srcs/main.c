/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 10:52:28 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_sign = 0;

/*
In case of Ctrl-D, free all and quit the program
*/
static void	ft_handle_eof(t_msh *msh)
{
	ft_putstr_fd("exit\n", 2);
	rl_clear_history();
	mcgic(NULL, QUIT, 0, msh);
}

static int	ft_readline(t_msh *msh)
{
	msh->line = readline("Minishell$ ");
	if (g_sign == 2)
	{
		msh->status = 130;
		mcgic(NULL, FLUSH, NO_ENV, msh);
		mcgic(NULL, FLUSH, PIP, msh);
		g_sign = 0;
	}
	if (g_sign == 1)
	{
		msh->status = 130;
		g_sign = 0;
	}
	msh->previous_status = msh->status;
	msh->status = 0;
	if (!msh->line)
		(ft_handle_eof(msh), exit(msh->previous_status));
	if (!mcgic(mlcp(msh->line, 1), ADD, NO_ENV, msh))
		(mcgic(NULL, QUIT, 0, msh), exit(msh->status));
	if (!msh->line[0])
		return (mcgic(NULL, FLUSH, NO_ENV, msh), -1);
	return (0);
}

static void	ft_init_var(t_msh *msh, char **env)
{
	msh->status = 0;
	msh->env = get_env(env, msh);
	msh->export_env = get_export_env(msh, env);
	msh->status = 0;
	msh->previous_status = 0;
	msh->sub = 0;
}

void	ft_no_interactive(t_msh *msh, char **env)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		exit(0);
	else
	{
		ft_init_var(msh, env);
		msh->line = mcgic(mlcp(ft_strdup(line), 1), ADD, NO_ENV, msh);
		free(line);
		ft_minishell(msh, 0, NULL);
		mcgic(NULL, QUIT, -1, msh);
		exit(msh->status);
	}
}

/*
if	isatty(0) == 1, in case of the STDOUT_FILENO has already been redirected,
	we open /dev/stdin tu dup2 , to be sure the STDOUT_FILENO of our
	minishell will be the terminal
*/
int	main(int ac, char **av, char **env)
{
	t_msh	msh;
	int		fd;

	(void)fd;
	(void)av;
	if (ac != 1)
		return (write(2, "minishell: too many arguments\n", 31), 1);
	if (isatty(0) == 0)
		ft_no_interactive(&msh, env);
	if (isatty(0) == 1)
	{
		fd = open("/dev/stdin", O_RDWR);
		(dup2(fd, STDOUT_FILENO), close(fd));
	}
	ft_init_var(&msh, env);
	while (1)
	{
		if (ft_signal_handler_msh(&msh) == 255)
			ft_exit(-1, -1, -1, &msh);
		if (ft_readline(&msh) == -1)
			continue ;
		add_history(msh.line);
		ft_minishell(&msh, 0, NULL);
		mcgic(NULL, FLUSH, NO_ENV, &msh);
	}
}
