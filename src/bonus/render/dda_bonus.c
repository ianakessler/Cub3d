/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Eixo x do DDA (plano mestre D.4-D.6). Componente nula: delta e
** side_dist ficam "infinitos" (RAY_INF), assim esse eixo nunca e o menor
** e nunca entra numa conta que multiplique por ele.
*/
static void	init_x(t_ray *r, double pos)
{
	r->step_x = 1;
	r->delta_dist_x = RAY_INF;
	r->side_dist_x = RAY_INF;
	if (r->dir_x == 0.0)
		return ;
	r->delta_dist_x = fabs(1.0 / r->dir_x);
	if (r->dir_x < 0.0)
	{
		r->step_x = -1;
		r->side_dist_x = (pos - r->map_x) * r->delta_dist_x;
	}
	else
		r->side_dist_x = (r->map_x + 1.0 - pos) * r->delta_dist_x;
}

static void	init_y(t_ray *r, double pos)
{
	r->step_y = 1;
	r->delta_dist_y = RAY_INF;
	r->side_dist_y = RAY_INF;
	if (r->dir_y == 0.0)
		return ;
	r->delta_dist_y = fabs(1.0 / r->dir_y);
	if (r->dir_y < 0.0)
	{
		r->step_y = -1;
		r->side_dist_y = (pos - r->map_y) * r->delta_dist_y;
	}
	else
		r->side_dist_y = (r->map_y + 1.0 - pos) * r->delta_dist_y;
}

/*
** Raio da coluna x (D.3): camera_x vai de -1 (esquerda) a ~+1 (direita);
** ray_dir = dir + plane * camera_x. Nunca e o vetor nulo (|dir| = 1 e
** plane perpendicular a dir).
*/
void	ray_init(t_ray *r, const t_player *p, int x)
{
	r->camera_x = 2.0 * x / (double)WIN_W - 1.0;
	r->dir_x = p->dir_x + p->plane_x * r->camera_x;
	r->dir_y = p->dir_y + p->plane_y * r->camera_x;
	r->map_x = (int)p->x;
	r->map_y = (int)p->y;
	r->side = 0;
	r->hit = 0;
	init_x(r, p->x);
	init_y(r, p->y);
}

/* Celula que para o raio: parede ou porta fechada (bonus, fase17). */
static int	is_solid(char c)
{
	return (c == '1' || c == DOOR_CLOSED);
}

/*
** Avanca celula a celula (D.5) ate achar '1' ou, no bonus (fase17), uma
** porta fechada ('D'); porta aberta ('d') e transparente. Seguro mesmo com mapa
** aberto: no maximo rows + cols passos e nunca le fora da grade.
** Retorna 1 se bateu numa parede; 0 se saiu da grade / estourou o limite
** (a coluna fica sem parede).
*/
int	ray_cast(t_ray *r, const t_map *m)
{
	int	steps;

	steps = m->rows + m->cols + 2;
	while (steps-- > 0)
	{
		if (r->side_dist_x < r->side_dist_y)
		{
			r->side_dist_x += r->delta_dist_x;
			r->map_x += r->step_x;
			r->side = 0;
		}
		else
		{
			r->side_dist_y += r->delta_dist_y;
			r->map_y += r->step_y;
			r->side = 1;
		}
		if (r->map_x < 0 || r->map_y < 0
			|| r->map_x >= m->cols || r->map_y >= m->rows)
			return (0);
		r->hit = is_solid(m->grid[r->map_y][r->map_x]);
		if (r->hit)
			return (1);
	}
	return (0);
}
