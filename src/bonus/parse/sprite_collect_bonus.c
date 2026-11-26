/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_collect_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static int	count_sprites(const t_map *m)
{
	int	n;
	int	r;
	int	c;

	n = 0;
	r = -1;
	while (++r < m->rows)
	{
		c = -1;
		while (++c < m->cols)
			n += (m->grid[r][c] == SPRITE_CELL);
	}
	return (n);
}

/* Centro de cada celula SPRITE_CELL, linha a linha. */
static void	fill_sprites(t_scene *s)
{
	int	r;
	int	c;

	r = -1;
	while (++r < s->map.rows)
	{
		c = -1;
		while (++c < s->map.cols)
		{
			if (s->map.grid[r][c] != SPRITE_CELL)
				continue ;
			s->sprites[s->spr_count++] = (t_sprite){c + 0.5, r + 0.5, 0.0,
				c + 2 * r};
		}
	}
}

/*
** Bonus (fase18): monta a lista de sprites a partir das celulas
** SPRITE_CELL do mapa (centro da celula), alocada UMA vez aqui (nada e
** alocado por quadro). O simbolo continua na grade: o minimapa o mostra
** e a colisao o trata como obstaculo. Mapa com sprite exige SP.
** 0 = ok (sem sprites: sprites == NULL); != 0 = erro ja reportado (o
** chamador faz scene_free).
*/
int	collect_sprites(t_scene *s)
{
	int	n;

	n = count_sprites(&s->map);
	if (n == 0)
		return (0);
	if (s->spr_frames == 0)
		return (error_msg2("missing identifier",
				"SP (sprite frames; the map has sprites)"));
	s->sprites = malloc(sizeof(t_sprite) * (size_t)n);
	if (!s->sprites)
		return (error_msg("memory allocation failed"));
	fill_sprites(s);
	return (0);
}
