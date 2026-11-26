/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/* Um passo do DDA (como em ray_cast): entra na proxima celula do raio. */
static void	dda_step(t_ray *r)
{
	if (r->side_dist_x < r->side_dist_y)
	{
		r->side_dist_x += r->delta_dist_x;
		r->map_x += r->step_x;
	}
	else
	{
		r->side_dist_y += r->delta_dist_y;
		r->map_y += r->step_y;
	}
}

/*
** Porta que o jogador esta olhando: segue o raio central da camera
** (camera_x = 0, direcao = dir, |dir| = 1, entao side_dist e a distancia
** real ate a face da proxima celula) a partir da celula do jogador ate a
** primeira celula que nao e chao. Se for porta (aberta ou fechada) e a
** face dela estiver a no maximo DOOR_REACH, devolve 1 e (col, row).
** Parede, sprite (objeto solido, fase18), fora da grade ou longe demais:
** 0. A celula onde o jogador esta
** nunca e alvo (o raio comeca saindo dela).
*/
static int	door_target(const t_game *g, int *col, int *row)
{
	t_ray		r;
	const t_map	*m;
	char		c;

	m = &g->scene.map;
	ray_init(&r, &g->player, WIN_W / 2);
	while (r.side_dist_x <= DOOR_REACH || r.side_dist_y <= DOOR_REACH)
	{
		dda_step(&r);
		if (r.map_x < 0 || r.map_y < 0
			|| r.map_x >= m->cols || r.map_y >= m->rows)
			return (0);
		c = m->grid[r.map_y][r.map_x];
		if (is_door(c))
		{
			*col = r.map_x;
			*row = r.map_y;
			return (1);
		}
		if (c != '0')
			return (0);
	}
	return (0);
}

/*
** Alterna a porta a frente do jogador (G.3): fechada -> aberta, aberta ->
** fechada. NAO fecha se o quadrado do jogador (COLL_RADIUS) ainda ocupa
** parte da celula da porta: fecha, pergunta a can_move (cell_blocks e o
** unico juiz do que bloqueia) se a posicao atual continua valida e, se
** nao, desfaz. Retorna 1 se a porta mudou de estado.
*/
int	door_toggle(t_game *g)
{
	int		col;
	int		row;
	char	*cell;

	if (!door_target(g, &col, &row))
		return (0);
	cell = &g->scene.map.grid[row][col];
	if (*cell == DOOR_CLOSED)
	{
		*cell = DOOR_OPEN;
		return (1);
	}
	*cell = DOOR_CLOSED;
	if (can_move(g, g->player.x, g->player.y))
		return (1);
	*cell = DOOR_OPEN;
	return (0);
}

/*
** Chamado a cada quadro por handle_input: consome o pedido da tecla E
** (input.key_use, marcado por on_key_press) e alterna a porta, se o
** intervalo DOOR_COOLDOWN desde a ultima alternancia ja passou.
*/
void	door_update(t_game *g, double dt)
{
	t_input	*in;

	in = &g->input;
	if (in->use_cd > 0.0)
		in->use_cd -= dt;
	if (!in->key_use)
		return ;
	in->key_use = 0;
	if (in->use_cd > 0.0)
		return ;
	if (door_toggle(g))
		in->use_cd = DOOR_COOLDOWN;
}
