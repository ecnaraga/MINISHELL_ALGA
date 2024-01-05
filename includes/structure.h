/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:42:52 by galambey          #+#    #+#             */
/*   Updated: 2024/01/05 14:25:19 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/includes/libft.h"

#ifndef STRUCTURE_H
# define STRUCTURE_H

# define NO_ENV 0
# define ENV 1
# define PIP 2
# define I 0
# define O 1

// extern int	status;
extern int	sign;

typedef struct s_dollar
{
	int	expnd;
	int	len_variable; // donne la len de la variable d environnement $ inclus
}		t_dollar;

typedef	struct s_split
{
	char		*data;
	int		wildcard;
	int		*wild;
	int		quote;
	int		token;
	int		dollar;
	t_dollar	*type;
	struct s_split		*next;
}		t_split;

typedef	struct s_env
{
	char	*name;
	char	*content;
	int		read;
	struct s_env	*next;
}		t_env;

typedef struct s_pipex
{
	int			**fd_p;
	char		**path;
	char		*good_path;
	char		**cmd_t;
	t_env		*hdoc; // limiter > nom cree > limiter > nom cree
	short int	re_split;
}				t_pipex;

typedef struct s_fdpar
{
	int in;
	int	out;
}		t_fdpar;

typedef	struct s_msh
{
	t_env	**env;
	t_env	**export_env;
	char	*line;	
	int		ac;
	t_split *av;
	t_pipex	p;
	t_fdpar	fd;
	int ambiguous;
	int		status;
	int		previous_status;
	int			sub;
}		t_msh;

typedef	struct s_quote
{
	int d;
	int s;
	int wildcard;
	int mod_dollar;
	int dollar;
}		t_quote;

typedef struct s_letter
{
	int	lt;
	int	k;
}		t_letter;

typedef	struct s_index
{
	size_t	i;
	size_t	j;
	int	d;
	int mod_dollar;
}		t_index;

typedef	struct s_expand
{
	int	i;
	int	j;
	int len;
	char *tmp;
	char *tmp2;
}		t_expand;


typedef struct s_par
{
	int par_o;
	int par_c;
	int prec_iss;
	int chev;
	int prec;
	// int multi_par;
	int multi_cmd;
}		t_par;

typedef struct s_head
{
	t_split *head;
	t_split *prev;
	t_env *head_hd;
	t_env *prev_hd;
}		t_head;

typedef struct s_fd
{
	int file;
	int	old_std;
}		t_fd;

typedef struct s_lpid
{
	pid_t pid;
	struct s_lpid *next;
}		t_lpid;

typedef struct s_magic
{
	void *addr;
	size_t	size;
}		t_magic;

typedef	struct s_intel
{
	int statut;
	int name_size;
}		t_intel;

typedef struct s_spl
{
	int		c_wd;
	char	**strs;
}		t_spl;

typedef int (*t_storage)(t_msh *msh, t_par *p, int *i);

enum	e_token
{
	TO_DEFINE,// 0
	INFILE,// 1
	OUTFILE_TRUNC,// 2
	OUTFILE_NO_TRUNC,// 3
	HDOC,// 4
	OPERATOR,// 5
	PIPE,//6
	PAR_OPEN,// 7
	PAR_CLOSE,// 8
	CMD,// 9
	CHEVRON,// 10
};

enum	e_expand
{
	EXPAND = 1,
	NO_EXPAND = 2,
	MULTI_DOLLAR = 3,
};

enum	e_parenthesis
{
	OTHER = 11,
	ISS = 12,
};

enum	e_malloc
{
	MLC,
	ADD,
	FREE,
	FLUSH,
	QUIT,
	PRINT,//a effacer
};

enum			e_err
{
	E_OK,
	E_STRJOIN,
	E_STRDUP,
	E_NO_CMD
};

enum		e_std
{
	CMD_ALONE,
	FIRST,
	MID,
	LAST,
	CHILD,
};

#endif