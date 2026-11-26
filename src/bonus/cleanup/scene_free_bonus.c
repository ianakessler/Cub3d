/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_free_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Libera map->grid. Contrato para a fase04: grid tem `rows` entradas
** (entradas NULL sao toleradas, em falha parcial de montagem).
*/
static void	map_free(t_map *map)
{
	int	i;

	if (map->grid)
	{
		i = 0;
		while (i < map->rows)
			free(map->grid[i++]);
		free(map->grid);
	}
	map->grid = NULL;
	map->rows = 0;
	map->cols = 0;
}

/*
** Idempotente: libera e zera ponteiros; cores voltam a -1 (indefinido).
** Bonus: caminhos DO/SP e a lista de sprites (fase17/18).
*/
void	scene_free(t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < SPR_MAX_FRAMES)
	{
		free(scene->spr_path[i]);
		scene->spr_path[i] = NULL;
	}
	scene->spr_frames = 0;
	free(scene->sprites);
	scene->sprites = NULL;
	scene->spr_count = 0;
	free(scene->north_path);
	scene->north_path = NULL;
	free(scene->south_path);
	scene->south_path = NULL;
	free(scene->west_path);
	scene->west_path = NULL;
	free(scene->east_path);
	scene->east_path = NULL;
	free(scene->door_path);
	scene->door_path = NULL;
	scene->floor_rgb = -1;
	scene->ceil_rgb = -1;
	map_free(&scene->map);
}
