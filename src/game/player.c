/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** dir unitario conforme o spawn (A.5-11: y cresce para baixo, logo
** N = (0, -1)). Qualquer outro valor nao chega aqui (validado na fase04).
*/
static void	spawn_dir(char c, double *dx, double *dy)
{
	*dx = 0.0;
	*dy = 0.0;
	if (c == 'N')
		*dy = -1.0;
	else if (c == 'S')
		*dy = 1.0;
	else if (c == 'E')
		*dx = 1.0;
	else if (c == 'W')
		*dx = -1.0;
}

/*
** Jogador no centro da celula de spawn, olhando para player_dir.
** plane = (-dir_y * k, dir_x * k), k = PLANE_LEN = tan(FOV/2):
** N -> plane (+k, 0): coluna esquerda da tela (camera_x = -1) olha
** para Oeste (D.3). Tabela: N (0,-1)/(k,0), S (0,1)/(-k,0),
** E (1,0)/(0,k), W (-1,0)/(0,-k).
*/
void	player_init(t_player *p, const t_map *map)
{
	p->x = map->player_col + 0.5;
	p->y = map->player_row + 0.5;
	spawn_dir(map->player_dir, &p->dir_x, &p->dir_y);
	p->plane_x = 0.0 - p->dir_y * PLANE_LEN;
	p->plane_y = p->dir_x * PLANE_LEN;
}
