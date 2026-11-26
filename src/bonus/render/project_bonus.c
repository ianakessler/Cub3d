/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Projecao da parede atingida (D.7, D.8, D.10).
** perp_dist: distancia projetada sobre dir (sem fisheye), pela formula
** exata da celula atingida; nunca menor que MIN_PERP.
** line_h = H / perp_dist (sem clamp); draw_start/draw_end clampados.
** wall_x: ponto de impacto ao longo da parede, fracao em [0, 1).
*/
void	ray_project(t_ray *r, const t_player *p)
{
	if (r->side == 0)
		r->perp_dist = (r->map_x - p->x + (1 - r->step_x) / 2) / r->dir_x;
	else
		r->perp_dist = (r->map_y - p->y + (1 - r->step_y) / 2) / r->dir_y;
	if (!(r->perp_dist >= MIN_PERP))
		r->perp_dist = MIN_PERP;
	r->line_h = (int)(WIN_H / r->perp_dist);
	r->draw_start = WIN_H / 2 - r->line_h / 2;
	r->draw_end = WIN_H / 2 + r->line_h / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end >= WIN_H)
		r->draw_end = WIN_H - 1;
	if (r->side == 0)
		r->wall_x = p->y + r->perp_dist * r->dir_y;
	else
		r->wall_x = p->x + r->perp_dist * r->dir_x;
	r->wall_x -= floor(r->wall_x);
}
