/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Ordena os sprites do MAIS DISTANTE para o mais proximo (dist =
** quadrado da distancia ao jogador; sem sqrt). Insercao: de um quadro
** para o outro a ordem quase nao muda, entao custa ~O(n). Reordena o
** proprio array (nada e alocado).
*/
void	sort_sprites(t_sprite *spr, int n, const t_player *p)
{
	t_sprite	cur;
	int			i;
	int			j;

	i = -1;
	while (++i < n)
		spr[i].dist = (spr[i].x - p->x) * (spr[i].x - p->x)
			+ (spr[i].y - p->y) * (spr[i].y - p->y);
	i = 0;
	while (++i < n)
	{
		cur = spr[i];
		j = i - 1;
		while (j >= 0 && spr[j].dist < cur.dist)
		{
			spr[j + 1] = spr[j];
			j--;
		}
		spr[j + 1] = cur;
	}
}

/* [floor(a), floor(b)] recortado a [0, max - 1]; 0 se ficou vazio. */
static int	clip_span(double a, double b, int max, int out[2])
{
	if (b <= 0.0 || a >= max)
		return (0);
	out[0] = 0;
	if (a > 0.0)
		out[0] = (int)ceil(a);
	out[1] = max - 1;
	if (b < max)
		out[1] = (int)ceil(b) - 1;
	return (out[0] <= out[1]);
}

/*
** Projecao (G.4) pela inversa da matriz da camera [plane dir]: o vetor
** sprite - jogador = tx * plane + depth * dir. tx / depth e o camera_x
** onde o sprite aparece (o mesmo dos raios: camera_x = 2x / W - 1, entao
** sprite a direita aparece a direita); depth e a distancia perpendicular,
** comparavel ao z_buffer. Altura = WIN_H / depth (como a parede: o sprite
** ocupa a altura de uma celula); largura = a de 1 celula na mesma
** profundidade. Retorna 0 se nao ha nada a desenhar (atras da camera,
** rente a ela ou fora da tela).
*/
int	sprite_project(const t_player *p, const t_sprite *s, t_spr_view *v)
{
	double	inv;
	double	tx;
	int		span[2];

	inv = 1.0 / (p->plane_x * p->dir_y - p->dir_x * p->plane_y);
	tx = inv * (p->dir_y * (s->x - p->x) - p->dir_x * (s->y - p->y));
	v->depth = inv * (-p->plane_y * (s->x - p->x)
			+ p->plane_x * (s->y - p->y));
	if (v->depth < SPR_MIN_DEPTH)
		return (0);
	v->h = WIN_H / v->depth;
	v->w = WIN_W / (2.0 * PLANE_LEN * v->depth);
	v->left = WIN_W / 2.0 * (1.0 + tx / v->depth) - v->w / 2.0;
	v->top = WIN_H / 2.0 - v->h / 2.0;
	if (!clip_span(v->left, v->left + v->w, WIN_W, span))
		return (0);
	v->x0 = span[0];
	v->x1 = span[1];
	if (!clip_span(v->top, v->top + v->h, WIN_H, span))
		return (0);
	v->y0 = span[0];
	v->y1 = span[1];
	return (1);
}

/*
** Sprites (G.4), chamados por render_frame depois das paredes (z_buffer
** pronto) e antes do minimapa. Frame da animacao pelo RELOGIO: o instante
** do quadro (input.last_time, medido com gettimeofday por frame_dt)
** dividido por SPR_FRAME_MS, mais a fase do sprite, modulo o numero de
** frames - a velocidade da animacao nao depende do fps. Do mais distante
** para o mais proximo (o mais proximo fica por cima).
*/
void	draw_sprites(t_game *g)
{
	t_scene		*s;
	t_spr_view	v;
	long		tick;
	int			i;

	s = &g->scene;
	if (s->spr_count <= 0 || s->spr_frames <= 0)
		return ;
	sort_sprites(s->sprites, s->spr_count, &g->player);
	tick = (g->input.last_time.tv_sec * 1000L
			+ g->input.last_time.tv_usec / 1000) / SPR_FRAME_MS;
	i = -1;
	while (++i < s->spr_count)
	{
		v.tex = &g->spr_tex[(tick + s->sprites[i].phase) % s->spr_frames];
		if (sprite_project(&g->player, &s->sprites[i], &v))
			draw_sprite(g, &v);
	}
}
