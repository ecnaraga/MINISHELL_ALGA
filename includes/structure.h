/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:42:52 by galambey          #+#    #+#             */
/*   Updated: 2023/11/06 14:33:45 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

typedef	struct s_node
{
	int nb_infile;
	int nb_outfile;
	
}		t_node;

typedef	struct s_split
{
	char *data;
	int	type;
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
	EXPAND,
};

enum	e_parenthesis
{
	PAR_OPEN = 1,
	PAR_CLOSE = 2,
	OTHER = 4,
	ISS = 5,
};

#endif