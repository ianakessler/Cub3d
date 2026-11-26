/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	clamp(double v, double lo, double hi)
{
	if (v < lo)
		return (lo);
	if (v > hi)
		return (hi);
	return (v);
}

/*
** Gira dir pela matriz de rotacao (y para baixo: angulo positivo gira
** para a DIREITA na tela; N -> E). dir e renormalizado e plane e
** recalculado a partir dele (= girar plane pela mesma matriz), assim
** |dir| = 1, |plane| = PLANE_LEN e plane perpendicular a dir nunca
** acumulam erro de arredondamento.
*/
static void	rotate(t_player *p, double angle)
{
	double	c;
	double	s;
	double	old;
	double	len;

	c = cos(angle);
	s = sin(angle);
	old = p->dir_x;
	p->dir_x = old * c - p->dir_y * s;
	p->dir_y = old * s + p->dir_y * c;
	len = sqrt(p->dir_x * p->dir_x + p->dir_y * p->dir_y);
	p->dir_x /= len;
	p->dir_y /= len;
	p->plane_x = -p->dir_y * PLANE_LEN;
	p->plane_y = p->dir_x * PLANE_LEN;
}

/*
** Translacao SEM colisao (colisao e bonus, fase14): o jogador pode
** atravessar paredes, mas a posicao e sempre mantida dentro da grade
** ([POS_EDGE, cols - POS_EDGE] x [POS_EDGE, rows - POS_EDGE]), entao
** (int)x / (int)y sao sempre indices validos e o DDA nunca le fora.
*/
static void	translate(t_player *p, const t_map *m, double mx, double my)
{
	p->x = clamp(p->x + mx, POS_EDGE, m->cols - POS_EDGE);
	p->y = clamp(p->y + my, POS_EDGE, m->rows - POS_EDGE);
}

/*
** Aplica as teclas pressionadas a g->player, proporcional a dt (s):
** setas giram ROT_SPEED rad/s; W/S andam ao longo de dir e D/A ao longo
** da direita da camera (-dir_y, dir_x), a MOVE_SPEED celulas/s. Teclas
** opostas se anulam; a diagonal (ex.: W + D) e normalizada para nao
** andar mais rapido que em linha reta.
*/
void	handle_input(t_game *g, double dt)
{
	t_input		*in;
	t_player	*p;
	double		fwd;
	double		side;
	double		len;

	in = &g->input;
	p = &g->player;
	if (in->key_right != in->key_left)
		rotate(p, ROT_SPEED * dt * (in->key_right - in->key_left));
	fwd = in->key_w - in->key_s;
	side = in->key_d - in->key_a;
	if (fwd == 0.0 && side == 0.0)
		return ;
	len = sqrt(fwd * fwd + side * side);
	fwd *= MOVE_SPEED * dt / len;
	side *= MOVE_SPEED * dt / len;
	translate(p, &g->scene.map, p->dir_x * fwd - p->dir_y * side,
		p->dir_y * fwd + p->dir_x * side);
}
