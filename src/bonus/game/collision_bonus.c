/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Celula (col, row) bloqueia o jogador? Fora da grade, '1', ' ' e porta
** fechada ('D') bloqueiam; so '0' e porta aberta ('d', fase17) sao
** caminhaveis. PONTO DE EXTENSAO: novos simbolos do bonus devem ser
** tratados AQUI. Qualquer simbolo nao tratado bloqueia - inclusive o
** sprite ('2', fase18), que e um objeto solido: o jogador para a
** COLL_RADIUS da celula dele (e nunca chega a "entrar" no sprite).
*/
int	cell_blocks(const t_game *g, int col, int row)
{
	const t_map	*m;

	m = &g->scene.map;
	if (col < 0 || row < 0 || col >= m->cols || row >= m->rows)
		return (1);
	return (m->grid[row][col] != '0' && m->grid[row][col] != DOOR_OPEN);
}

/*
** O jogador e um quadrado [x - r, x + r] x [y - r, y + r], r = COLL_RADIUS.
** A posicao (nx, ny) e valida se nenhuma das 4 quinas cai em celula que
** bloqueia (r < 0.5, entao as quinas cobrem todas as celulas tocadas).
*/
int	can_move(const t_game *g, double nx, double ny)
{
	double	r;
	int		c0;
	int		c1;
	int		r0;
	int		r1;

	r = COLL_RADIUS;
	c0 = (int)floor(nx - r);
	c1 = (int)floor(nx + r);
	r0 = (int)floor(ny - r);
	r1 = (int)floor(ny + r);
	return (!cell_blocks(g, c0, r0) && !cell_blocks(g, c1, r0)
		&& !cell_blocks(g, c0, r1) && !cell_blocks(g, c1, r1));
}

static int	valid_at(const t_game *g, double pos, double other, int is_x)
{
	if (is_x)
		return (can_move(g, pos, other));
	return (can_move(g, other, pos));
}

/*
** Move um eixo: at[0] = coordenada desse eixo, at[1] = a do outro eixo
** (fica fixa); is_x diz qual e qual. Se pos + d bate, encosta o
** quadrado na face da celula que bloqueou (a COLL_EPS dela), desde que
** isso seja um avanco valido; senao fica parado nesse eixo. Como
** |d| <= COLL_STEP < 2r, a celula que bloqueia e sempre a da frente.
*/
static double	slide_axis(const t_game *g, const double at[2], double d,
		int is_x)
{
	double	np;

	if (d == 0.0)
		return (at[0]);
	np = at[0] + d;
	if (valid_at(g, np, at[1], is_x))
		return (np);
	if (d > 0.0)
		np = floor(np + COLL_RADIUS) - COLL_RADIUS - COLL_EPS;
	else
		np = floor(np - COLL_RADIUS) + 1.0 + COLL_RADIUS + COLL_EPS;
	if (((d > 0.0 && np > at[0]) || (d < 0.0 && np < at[0]))
		&& valid_at(g, np, at[1], is_x))
		return (np);
	return (at[0]);
}

/*
** Desloca o jogador por (mx, my) com colisao: eixos independentes
** (primeiro x com o y atual, depois y com o novo x), o que faz deslizar
** ao longo das paredes em vez de travar. O deslocamento e dividido em
** subpassos de no maximo COLL_STEP por eixo: nunca atravessa parede,
** mesmo com dt/velocidade grandes.
*/
void	move_collide(t_game *g, double mx, double my)
{
	t_player	*p;
	int			n;
	int			i;

	p = &g->player;
	n = 1;
	while (fabs(mx) > COLL_STEP * n || fabs(my) > COLL_STEP * n)
		n++;
	i = 0;
	while (i < n)
	{
		p->x = slide_axis(g, (double [2]){p->x, p->y}, mx / n, 1);
		p->y = slide_axis(g, (double [2]){p->y, p->x}, my / n, 0);
		i++;
	}
}
