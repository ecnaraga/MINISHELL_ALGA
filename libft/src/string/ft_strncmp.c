/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:20:39 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 13:00:35 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((((char *)s1)[i] != '\0' || ((char *)s2)[i] != '\0')
				&& ((unsigned char *) s1)[i] == ((unsigned char *)s2)[i]
				&& i + 1 < n)
		i++;
	if (i == n)
		return (0);
	else
		return (((unsigned char *) s1)[i] - ((unsigned char *)s2)[i]);
}
