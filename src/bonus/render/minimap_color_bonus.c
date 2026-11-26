/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_color_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Cor de uma celula no minimapa (fase15): '1' = parede, porta fechada /
** aberta (fase17) = MM_DOOR / MM_DOOR_OPEN, sprite (fase18) = MM_SPRITE,
** ' ' (vazio exterior de mapa nao retangular) = -1 (transparente, nao
** desenha), o resto = chao.
*/
int	mm_cell_color(char c)
{
	if (c == ' ')
		return (-1);
	if (c == '1')
		return (MM_WALL);
	if (c == DOOR_CLOSED)
		return (MM_DOOR);
	if (c == DOOR_OPEN)
		return (MM_DOOR_OPEN);
	if (c == SPRITE_CELL)
		return (MM_SPRITE);
	return (MM_FLOOR);
}
