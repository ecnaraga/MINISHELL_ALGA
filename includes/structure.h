/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:42:52 by galambey          #+#    #+#             */
/*   Updated: 2023/11/10 15:59:05 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

typedef	struct s_node
{
	int nb_infile;
	int nb_outfile;
	
}		t_node;

typedef struct d_dollar
{
	int expnd;
	int len_variable; // donne la len de la variable d environnement $ inclus
}		t_dollar;

typedef	struct s_split
{
	char *data;
	int	 dollar;
	t_dollar	*type;
}		t_split;

typedef	struct s_msh
{
	char **env;
	char *line;	
	int ac;
	t_split *av;
	t_node	*node;
}		t_msh;

typedef	struct s_quote
{
	int d;
	int s;
}		t_quote;

enum	e_split
{
	TO_DEFINE,
	INFILE,
	OUTFILE,
	OPERATOR,
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
	PAR_OPEN = 1,
	PAR_CLOSE = 2,
	OTHER = 4,
	ISS = 5,
};

#endif