/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:25:02 by galambey          #+#    #+#             */
/*   Updated: 2023/10/31 14:26:31 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

void	ft_echo(t_split *av, int nb_arg);

int	ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlen(const char *s);

#endif