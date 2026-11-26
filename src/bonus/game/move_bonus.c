/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Gira dir pela matriz de rotacao (y para baixo: angulo positivo gira
** para a DIREITA na tela; N -> E). dir e renormalizado e plane e
** recalculado a partir dele (= girar plane pela mesma matriz), assim
** |dir| = 1, |plane| = PLANE_LEN e plane perpendicular a dir nunca
** acumulam erro de arredondamento.
*/
void	rotate(t_player *p, double angle)
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
** Aplica as teclas pressionadas a g->player, proporcional a dt (s):
** setas giram ROT_SPEED rad/s; W/S andam ao longo de dir e D/A ao longo
** da direita da camera (-dir_y, dir_x), a MOVE_SPEED celulas/s. Teclas
** opostas se anulam; a diagonal (ex.: W + D) e normalizada para nao
** andar mais rapido que em linha reta.
** Bonus: o deslocamento passa por move_collide (game/collision_bonus.c):
** o jogador nao atravessa paredes e desliza ao longo delas.
** Bonus (fase16): o deslocamento do mouse acumulado desde o ultimo
** quadro gira MOUSE_SENS rad/pixel (independe de dt), somado as setas.
** Bonus (fase17): a tecla E abre/fecha a porta a frente (door_update).
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
	if (in->mouse_dx != 0)
		rotate(p, in->mouse_dx * MOUSE_SENS);
	in->mouse_dx = 0;
	door_update(g, dt);
	fwd = in->key_w - in->key_s;
	side = in->key_d - in->key_a;
	if (fwd == 0.0 && side == 0.0)
		return ;
	len = sqrt(fwd * fwd + side * side);
	fwd *= MOVE_SPEED * dt / len;
	side *= MOVE_SPEED * dt / len;
	move_collide(g, p->dir_x * fwd - p->dir_y * side,
		p->dir_y * fwd + p->dir_x * side);
}
