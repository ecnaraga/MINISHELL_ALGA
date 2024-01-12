/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 12:17:56 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 11:15:18 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAGIC_MALLOC_H
# define MAGIC_MALLOC_H

/*
ft_magic_malloc.c
*/
void	*mcgic(t_magic *p, int rule, int lst, t_msh *msh);
t_magic	*mlcp(void *addr, size_t size);

/*
ft_magic_malloc_utils.c
*/

t_list	*ft_lstnew_malloc(size_t size);
t_list	*ft_lstnew_add(void *addr);

/*
ft_magic_malloc_utils_bis.c
*/
void	del(void *content);
int		ft_check(void *data, void *data_ref);
void	ft_list_remove_if(t_list **begin_list, t_magic *p, int (*cmp)());

#endif