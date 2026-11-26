/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/* Fora da grade ou espaco = vazio exterior (E.5). */
static int	is_void(const t_map *m, int r, int c)
{
	if (r < 0 || c < 0 || r >= m->rows || c >= m->cols)
		return (1);
	return (m->grid[r][c] == ' ');
}

/*
** Bonus: porta (aberta ou fechada, fase17) e sprite (fase18) contam como
** caminhaveis: o mapa precisa ser fechado por paredes em volta deles.
*/
static int	is_walkable(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W'
		|| is_door(c) || c == SPRITE_CELL);
}

/*
** Toda celula caminhavel precisa ter os 4 vizinhos dentro da grade e
** diferentes de espaco. Cobre borda da grade, linha curta completada com
** espaco e espaco no meio do mapa. Diagonais nao contam.
*/
static int	check_neighbors(const t_map *m)
{
	int	r;
	int	c;

	r = -1;
	while (++r < m->rows)
	{
		c = -1;
		while (++c < m->cols)
		{
			if (!is_walkable(m->grid[r][c]))
				continue ;
			if (is_void(m, r - 1, c) || is_void(m, r + 1, c)
				|| is_void(m, r, c - 1) || is_void(m, r, c + 1))
				return (error_at("map is not closed: walkable cell next to "
						"the void or the map edge", r + 1, c + 1));
		}
	}
	return (0);
}

/*
** Valida o mapa de parse_map (fase05): fechado por paredes (R26).
** 1) vizinhanca de toda celula caminhavel (inclusive areas separadas);
** 2) bonus: toda porta entre duas paredes alinhadas (check_doors);
** 3) flood fill a partir do spawn (defesa em profundidade).
** Nao altera o mapa.
** 0 = valido; != 0 = erro ja reportado.
*/
int	validate_map(const t_map *map)
{
	if (!map->grid || map->rows <= 0 || map->cols <= 0)
		return (error_msg("empty map"));
	if (map->player_row < 0 || map->player_row >= map->rows
		|| map->player_col < 0 || map->player_col >= map->cols
		|| !is_walkable(map->grid[map->player_row][map->player_col]))
		return (error_msg("invalid player spawn"));
	if (check_neighbors(map) != 0)
		return (EXIT_ERR);
	if (check_doors(map) != 0)
		return (EXIT_ERR);
	return (check_connected(map));
}
