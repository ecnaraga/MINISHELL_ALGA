/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:59:15 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 17:02:42 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

/*
signal.c
*/
int	ft_signal_handler_msh(t_msh *msh);
int	ft_signal_handler_msh_child(t_msh *msh);
int	ft_signal_handler_msh_hdoc(t_msh *msh);

#endif