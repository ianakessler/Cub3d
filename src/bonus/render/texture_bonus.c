/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static char	*tex_path(const t_scene *s, int i)
{
	if (i == TEX_N)
		return (s->north_path);
	if (i == TEX_S)
		return (s->south_path);
	if (i == TEX_E)
		return (s->east_path);
	if (i == TEX_D)
		return (s->door_path);
	return (s->west_path);
}

/*
** Carrega um XPM com a miniLibX (R23) em t, no tamanho real dele (nao
** assume 64x64). Em falha: Error\n + caminho, retorna != 0; o que ja foi
** carregado fica em g e game_destroy libera.
*/
int	load_xpm(t_game *g, t_image *t, char *path)
{
	t->img = mlx_xpm_file_to_image(g->mlx, path, &t->w, &t->h);
	if (!t->img)
		return (error_msg2(path, "cannot load XPM texture"));
	t->addr = mlx_get_data_addr(t->img, &t->bpp, &t->line_len, &t->endian);
	if (!t->addr || t->bpp < 8 || t->line_len <= 0
		|| t->w <= 0 || t->h <= 0)
		return (error_msg2(path, "invalid texture image"));
	return (0);
}

/*
** As 4 texturas das paredes, a da porta (TEX_D, bonus) se o .cub tiver
** DO (obrigatorio quando o mapa tem porta) e os frames do sprite (SP,
** fase18). Retorna != 0 em erro ja reportado.
*/
int	textures_load(t_game *g)
{
	char	*path;
	int		i;

	i = -1;
	while (++i < TEX_COUNT)
	{
		path = tex_path(&g->scene, i);
		if (i == TEX_D && !path)
			continue ;
		if (load_xpm(g, &g->tex[i], path) != 0)
			return (EXIT_ERR);
	}
	i = -1;
	while (++i < g->scene.spr_frames)
		if (load_xpm(g, &g->spr_tex[i], g->scene.spr_path[i]) != 0)
			return (EXIT_ERR);
	return (0);
}

/* Paredes, porta e frames do sprite. Idempotente; antes do display. */
void	textures_destroy(t_game *g)
{
	t_image	*t;
	int		i;

	i = -1;
	while (++i < TEX_COUNT + SPR_MAX_FRAMES)
	{
		t = &g->tex[i];
		if (i >= TEX_COUNT)
			t = &g->spr_tex[i - TEX_COUNT];
		if (g->mlx && t->img)
			mlx_destroy_image(g->mlx, t->img);
		t->img = NULL;
		t->addr = NULL;
	}
}

/*
** Face (D.9) pela DIRECAO do raio: raio indo para o Norte (y
** decrescente) mostra NO, para o Sul SO, Leste EA, Oeste WE. Ou seja,
** olhando para o Norte voce ve a textura NO.
** tex_x: sem espelhamento, o lado esquerdo da textura fica a esquerda
** na tela. Com x -> direita e y -> baixo, wall_x cresce da esquerda para
** a direita quando o raio vai para N ou E, e da direita para a esquerda
** quando vai para S ou W: nesses dois casos inverte.
** Bonus (fase17): porta fechada usa TEX_D em todas as faces (mesma regra
** de espelhamento: a porta nunca aparece invertida).
*/
void	tex_column(const t_game *g, t_ray *r)
{
	int	w;

	if (r->side == 0 && r->dir_x > 0.0)
		r->tex_id = TEX_E;
	else if (r->side == 0)
		r->tex_id = TEX_W;
	else if (r->dir_y > 0.0)
		r->tex_id = TEX_S;
	else
		r->tex_id = TEX_N;
	if (g->scene.map.grid[r->map_y][r->map_x] == DOOR_CLOSED)
		r->tex_id = TEX_D;
	w = g->tex[r->tex_id].w;
	r->tex_x = (int)(r->wall_x * w);
	if (r->tex_x >= w)
		r->tex_x = w - 1;
	if (r->tex_x < 0)
		r->tex_x = 0;
	if ((r->side == 0 && r->dir_x < 0.0) || (r->side == 1 && r->dir_y > 0.0))
		r->tex_x = w - r->tex_x - 1;
}
