/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:25:05 by garance           #+#    #+#             */
/*   Updated: 2023/06/23 12:57:17 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strmapi(char const *s, char (*ft)(unsigned int, char))
{
	char	*mapi;
	int		i;

	if (!s)
		return (NULL);
	mapi = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (mapi == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		mapi[i] = ft(i, s[i]);
		i++;
	}
	mapi[i] = '\0';
	return (mapi);
}
