/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Escreve "Error\n" + msg + "\n" em stderr e retorna o codigo de erro (1).
** Uso: return (error_msg("mensagem"));
*/
int	error_msg(const char *msg)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	if (msg)
		ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (EXIT_ERR);
}

/*
** Escreve "Error\n<what>: <detail>\n" em stderr e retorna 1.
** Ex.: error_msg2(path, strerror(errno)).
*/
int	error_msg2(const char *what, const char *detail)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	ft_putstr_fd((char *)what, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)detail, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (EXIT_ERR);
}

/*
** Escreve "Error\n<what> (map row R, col C)\n" em stderr e retorna 1.
** row/col sao 1-indexados, relativos a primeira linha do mapa.
*/
int	error_at(const char *what, int row, int col)
{
	ft_putstr_fd("Error\n", STDERR_FILENO);
	ft_putstr_fd((char *)what, STDERR_FILENO);
	ft_putstr_fd(" (map row ", STDERR_FILENO);
	ft_putnbr_fd(row, STDERR_FILENO);
	ft_putstr_fd(", col ", STDERR_FILENO);
	ft_putnbr_fd(col, STDERR_FILENO);
	ft_putstr_fd(")\n", STDERR_FILENO);
	return (EXIT_ERR);
}
