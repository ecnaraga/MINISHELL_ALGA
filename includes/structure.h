/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:42:52 by galambey          #+#    #+#             */
/*   Updated: 2023/11/27 22:12:04 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/includes/libft.h"

#ifndef STRUCTURE_H
# define STRUCTURE_H

# define NO_ENV 0
# define ENV 1

extern int	status;

typedef	struct s_node
{
	int	nb_infile;
	int	nb_outfile;
	
}		t_node;

typedef struct s_dollar
{
	int	expnd;
	int	len_variable; // donne la len de la variable d environnement $ inclus
}		t_dollar;

typedef	struct s_split
{
	char		*data;
	int			token;
	int			dollar;
	t_dollar	*type;
	struct s_split		*next;
}		t_split;

typedef	struct s_msh
{
	t_list	**env;
	char	*line;	
	int		ac;
	t_split *av;
	int		previous_status;
	t_node	*node;
}		t_msh;

typedef	struct s_quote
{
	int d;
	int s;
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
}		t_index;

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

typedef struct	s_shell
{
	t_list	*infile;
	t_list	*outfile;
	t_list	*cmd;
}		t_shell;

typedef int (*t_storage)(t_msh *msh, t_par *p, int *i);

enum	e_token
{
	TO_DEFINE,// 0
	INFILE,// 1
	OUTFILE_TRUNC,// 2
	OUTFILE_NO_TRUNC,// 3
	HERE_DOC,// 4
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
	MALLOC,
	ADD,
	FREE,
	FLUSH,
	QUIT,
	PRINT,//a effacer
};

#endif