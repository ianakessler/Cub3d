/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_check_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/* Porta, fechada ('D') ou aberta ('d', so em tempo de execucao). */
int	is_door(char c)
{
	return (c == DOOR_CLOSED || c == DOOR_OPEN);
}

static int	is_wall(const t_map *m, int r, int c)
{
	if (r < 0 || c < 0 || r >= m->rows || c >= m->cols)
		return (0);
	return (m->grid[r][c] == '1');
}

/*
** Bonus (fase17): toda porta fica entre duas paredes alinhadas - parede a
** esquerda e a direita (passagem N-S) ou acima e abaixo (passagem L-O).
** Assim a porta e sempre uma passagem de um corredor, com as faces
** laterais encostadas em parede. Chamado depois da checagem de
** vizinhanca (porta na borda ou ao lado do vazio ja deu erro).
*/
int	check_doors(const t_map *map)
{
	int	r;
	int	c;

	r = -1;
	while (++r < map->rows)
	{
		c = -1;
		while (++c < map->cols)
		{
			if (!is_door(map->grid[r][c]))
				continue ;
			if (!(is_wall(map, r, c - 1) && is_wall(map, r, c + 1))
				&& !(is_wall(map, r - 1, c) && is_wall(map, r + 1, c)))
				return (error_at("door must be between two aligned walls",
						r + 1, c + 1));
		}
	}
	return (0);
}

/*
** O identificador DO (textura da porta) e opcional no .cub bonus - um
** .cub da parte obrigatoria continua valido -, mas obrigatorio se o
** mapa tiver ao menos uma porta.
*/
int	check_door_texture(const t_scene *scene)
{
	int	r;

	if (scene->door_path)
		return (0);
	r = -1;
	while (++r < scene->map.rows)
	{
		if (ft_strchr(scene->map.grid[r], DOOR_CLOSED))
			return (error_msg2("missing identifier",
					"DO (door texture; the map has doors)"));
	}
	return (0);
}
