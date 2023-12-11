/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2023/12/11 17:43:25 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// TO DO : ATTENTION A CE TEST : echo $=HOME => Pour l instant traite dans parsing comme si variable d environnement a voir...
//PENSER A EXPAND DANS LES RETOUR D ERREUR DE L EXEC ex $USER CMD NORT FOUNT
// PENSER A GERE $? => represente exitstatus
// IMPLEMENTER EXPAND DANS OUTFILE INFILE HEREDOC

char *get_value(t_env **env, char *str, int rule)
{
	t_env	*node;

	node = *env;
	while (node)
	{
		if (strcmp(node->name, str) == 0)
		{
			ft_magic_malloc(FREE, 0, str, NO_ENV);
			if (rule == CMD)
				return (ft_magic_malloc(ADD, 0, ft_strtrim(node->content + 1 , " \b\t\n\v\f\r"), NO_ENV));
			if (rule == OTHER)
				return (ft_magic_malloc(ADD, 0, ft_strtrim_except_tips(node->content + 1 , " \b\t\n\v\f\r"), NO_ENV));
			if (rule == INFILE || rule == OUTFILE_NO_TRUNC || rule == OUTFILE_TRUNC)
				return (ft_magic_malloc(ADD, 0, ft_strdup(node->content + 1), NO_ENV));
		}
		else
			node = node->next;
	}
	ft_magic_malloc(FREE, 0, str, NO_ENV);
	return (ft_magic_malloc(ADD, 0, ft_strdup(""), NO_ENV));
}

/*
Pour savoir s'il comment traiter le dollar, se referer aux instructions dans le
	tableau de structure strs.type (plus de precisions dans ft_strlcpy_msh)
1. Si (expnd = EXPAND = 1 et len variable = 1) ou si (expnd = MULTIDOLLAR = 3)
	=> afficher 1 $
2. Si (expnd = NO_EXPAND = 2) => afficher les char tel quel et ne pas expand
3. Si (expnd = EXPAND = 1 et len_variable = 0) => ne pas afficher le dollar
4. Si (expnd = EXPAND = 1 et len_variable > 1) => expand : remplacer la
	variable d environnement par sa valeur dans l 'env
	exemple : si expnd = 1 && len_variable = 5 => remplacer $ + 4 char
		(ex : $USER) par la variable d environnement dans env

REGLES A SUIVRE :
- Si plusieurs dollars d affilee : afficher 1 dollar
- Si 1 seul dollar suivi de quote : ne pas afficher le dollar
- Si un seul dollar entre des quotes single ou double : afficher 1 dollar
- Si dollar pas entre quote : expand
- Si dollar entre double_quote : expand
- Si dollar entre de single_quote : ne pas expand
*/
char	*ft_expand(t_msh *msh, char *cmd, int rule)
{
	char *tmp;
	char *tmp2;
	int i;
	int j;
	
	// if (msh->av->type->expnd == 3 || (msh->av->type->expnd == 1 && msh->av->type->len_variable == 1))
	// 	msh->av->data[1] = '\0';
	// else if (msh->av->type->expnd == 1 && msh->av->type->len_variable == 0)
	// 	msh->av->data[0] = '\0';
	// else if (msh->av->type->expnd == 1 && msh->av->type->len_variable > 1)
	// {
	// 	tmp = ft_magic_malloc(ADD, 0, ft_substr(msh->av->data, 1, msh->av->type->len_variable - 1), NO_ENV);
	// 	msh->av->data = get_value(msh->env, tmp);
	// 	ft_magic_malloc(FREE, 0, tmp, NO_ENV); 
	// }
	i = 0;
	j = 0;
	cmd = NULL;
	while (msh->av->data[i])
	// while (j < msh->av->dollar)
	{
		if (msh->av->data[i] == '$')
		{
			if (msh->av->type[j].expnd == 3)
			{
				tmp2 = cmd;
				cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
				if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
					ft_exit(-1, -1, -1);
				if (tmp2)
					ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
				while (msh->av->data[i] && msh->av->data[i] == '$')
					i++;
				j++;
			}
			else if (msh->av->type[j].expnd == 1 && msh->av->type[j].len_variable == 1)
			{
				tmp2 = cmd;
				cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
				if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
					ft_exit(-1, -1, -1);
				if (tmp2)
					ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
				i++;
				j++;
			}
			else if (msh->av->type[j].expnd == 1 && msh->av->type[j].len_variable > 1)
			{
				if (msh->av->data[i + 1] == '?')
				{
					tmp = ft_magic_malloc(ADD, 0, ft_itoa(msh->previous_status), NO_ENV);
					if (status == 255)
						ft_exit(-1, -1, -1);
					tmp2 = cmd;
					cmd = ft_magic_malloc(ADD, 0, ft_strjoin(cmd, tmp), NO_ENV);
					if (status == 255)
						ft_exit(-1, -1, -1);
					if (tmp2)
						ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
					ft_magic_malloc(FREE, 0, tmp, NO_ENV);
					i += 2;
					j++;
				}
				else
				{
					tmp = ft_magic_malloc(ADD, 0, ft_substr(msh->av->data, i + 1, msh->av->type[j].len_variable - 1), NO_ENV);
					if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
						ft_exit(-1, -1, -1);
					tmp = get_value(msh->env, tmp, rule);
					if (status == 255)
						ft_exit(-1, -1, -1);
					tmp2 = cmd;
					cmd = ft_magic_malloc(ADD, 0, ft_strjoin(cmd, tmp), NO_ENV);
					if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
						ft_exit(-1, -1, -1);
					if (tmp2)
						ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
					ft_magic_malloc(FREE, 0, tmp, NO_ENV);
					i += msh->av->type[j].len_variable;
					j++;
				}
			}
			else if (msh->av->type[j].expnd == 2)
			{
				tmp2 = cmd;
				cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
				if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
					ft_exit(-1, -1, -1);
				if (tmp2)
					ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
				i++;
				j++;
			}
		}
		else
		{
			tmp2 = cmd;
			cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
			if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
				ft_exit(-1, -1, -1);
			if (tmp2)
				ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
			i++;
		}
	}
	return (cmd);
}
