/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_bfr_char.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:58:56 by galambey          #+#    #+#             */
/*   Updated: 2023/09/08 12:15:38 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/* 
Crashe si str == NULL > a proteger dans la fonction parente
Return la len jusqu au char c non compte, si pas de char c retourne la len
totale de la string
*/
size_t	ft_strlen_bfr_char(const char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}
