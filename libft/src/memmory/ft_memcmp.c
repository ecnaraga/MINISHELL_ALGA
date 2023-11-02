/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:52:14 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 13:01:57 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((s1 || s2) && ((unsigned char *) s1)[i] == ((unsigned char *)
				s2)[i] && i + 1 < n)
		i++;
	if (i == n)
		return (0);
	else
		return (((unsigned char *) s1)[i] - ((unsigned char *)s2)[i]);
}
