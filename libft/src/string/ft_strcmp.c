/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 11:59:12 by galambey          #+#    #+#             */
/*   Updated: 2023/12/01 19:42:45 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	// dprintf(2, "s1 %s\n", s1);
	// dprintf(2, "s2 %s\n", s2);
	if (!s1[0] || !s2[0])
		return (1);
	while ((((char *)s1)[i] != '\0' || ((char *)s2)[i] != '\0')
		&& ((unsigned char *)s1)[i] == ((unsigned char *)s2)[i])
		i++;
	return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}
