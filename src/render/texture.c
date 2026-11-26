/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*tex_path(const t_scene *s, int i)
{
	if (i == TEX_N)
		return (s->north_path);
	if (i == TEX_S)
		return (s->south_path);
	if (i == TEX_E)
		return (s->east_path);
	return (s->west_path);
}

/*
** Carrega as 4 texturas XPM com a miniLibX (R23), usando o tamanho real
** de cada uma (nao assume 64x64). Em falha: Error\n + caminho, retorna
** != 0; as ja carregadas ficam em g->tex e game_destroy as libera.
*/
int	textures_load(t_game *g)
{
	t_image	*t;
	char	*path;
	int		i;

	i = -1;
	while (++i < 4)
	{
		t = &g->tex[i];
		path = tex_path(&g->scene, i);
		t->img = mlx_xpm_file_to_image(g->mlx, path, &t->w, &t->h);
		if (!t->img)
			return (error_msg2(path, "cannot load XPM texture"));
		t->addr = mlx_get_data_addr(t->img, &t->bpp, &t->line_len,
				&t->endian);
		if (!t->addr || t->bpp < 8 || t->line_len <= 0
			|| t->w <= 0 || t->h <= 0)
			return (error_msg2(path, "invalid texture image"));
	}
	return (0);
}

/* Idempotente; chamar antes de destruir o display. */
void	textures_destroy(t_game *g)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (g->mlx && g->tex[i].img)
			mlx_destroy_image(g->mlx, g->tex[i].img);
		g->tex[i].img = NULL;
		g->tex[i].addr = NULL;
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
	w = g->tex[r->tex_id].w;
	r->tex_x = (int)(r->wall_x * w);
	if (r->tex_x >= w)
		r->tex_x = w - 1;
	if (r->tex_x < 0)
		r->tex_x = 0;
	if ((r->side == 0 && r->dir_x < 0.0) || (r->side == 1 && r->dir_y > 0.0))
		r->tex_x = w - r->tex_x - 1;
}
