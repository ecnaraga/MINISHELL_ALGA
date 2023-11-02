/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:42:52 by galambey          #+#    #+#             */
/*   Updated: 2023/10/31 14:27:59 by galambey         ###   ########.fr       */
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

#endif