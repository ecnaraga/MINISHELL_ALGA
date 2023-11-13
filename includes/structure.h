/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:42:52 by galambey          #+#    #+#             */
/*   Updated: 2023/11/13 17:13:38 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

typedef	struct s_node
{
	int	nb_infile;
	int	nb_outfile;
	
}		t_node;

typedef struct d_dollar
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
}		t_split;

typedef	struct s_msh
{
	char	**env;
	char	*line;	
	int		ac;
	t_split *av;
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

enum	e_token
{
	TO_DEFINE,
	INFILE,
	OUTFILE,
	HERE_DOC,
	OPERATOR,
	PAR_OPEN,
	PAR_CLOSE,
	CMD,
	ARG,
};

enum	e_expand
{
	EXPAND = 1,
	NO_EXPAND = 2,
	MULTI_DOLLAR = 3,
};

enum	e_parenthesis
{
	OTHER = 9,
	ISS = 10,
};

#endif