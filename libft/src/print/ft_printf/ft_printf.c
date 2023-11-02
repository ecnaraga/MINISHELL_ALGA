/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 13:51:27 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:33:23 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static void	print_wrong_format(char *s, int i)
{
	int	j;

	j = 0;
	write(1, "%", 1);
	while (j < i)
	{
		write(1, &s[j], 1);
		if (s[j] == '.')
			write(1, "0", 1);
		j++;
	}
}

static int	ck_str(char *s, int *point)
{
	int	i;

	i = 0;
	while (ck_char("-0# +", s[i]) > 0)
		i++;
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (s[i] == '.')
	{
		i += 1;
		*point += 1;
	}
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (ck_char("cpsdiuxX%", s[i]) == 0)
	{
		print_wrong_format(s, i);
		return (i + 1 + *point);
	}
	return (0);
}

int	found_percent(va_list ap, const char *s, int *count, int *i)
{
	int	result;
	int	j;
	int	point;

	point = 0;
	result = ck_str((char *)s + 1, &point);
	j = 0;
	if (result == 0)
	{
		j = ft_arg(ap, (char *)s, count);
		if (j == -2)
			return (j);
		*i += (j + 1);
	}
	else
	{
		*i += result - 1 - point;
		*count += result;
	}
	return (0);
}

static int	ft_print_s(const char *s, va_list ap, int *count)
{
	int	i;
	int	j;

	i = -1;
	while (s[++i])
	{
		if (s[i] != '%')
			*count += ft_print_char((void *)&s[i]);
		else if (s[i + 1] && s[i] == '%')
		{
			j = found_percent(ap, s + i, count, &i);
			if (j == -2)
				return (-1);
		}
		else if (!s[i + 1] && s[i] == '%')
			*count = -1;
	}
	return (*count);
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	int		count;

	count = 0;
	if (!s)
		return (-1);
	va_start(ap, s);
	count = ft_print_s(s, ap, &count);
	va_end(ap);
	return (count);
}
