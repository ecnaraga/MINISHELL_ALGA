/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_afr_char.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:58:56 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 18:42:23 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/* 
Crashe si str == NULL > a proteger dans la fonction parente
Return la len dde la derniere occurence du char c non compte a la fin de la
chaine, si pas de char c retourne la len totale de la string
*/
size_t	ft_strlen_afr_char(const char *str, char c)
{
	int		i;
	size_t	len;

	len = 0;
	i = ft_strlen(str) - 1;
	while (i >= 0 && str[i] && str[i] != c)
	{
		i--;
		len++;
	}
	return (len);
}
