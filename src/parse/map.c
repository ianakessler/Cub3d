/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Fim do mapa (A.5-5): o mapa vai de start ate a primeira linha em branco
** (ou o EOF). Depois dela so podem existir linhas em branco; qualquer
** outra coisa (elemento apos o mapa, mapa partido por linha vazia) e erro.
*/
static int	map_bounds(const t_lines *l, size_t start, size_t *end)
{
	size_t	i;

	i = start;
	while (i < l->count && !is_blank_line(l->lines[i]))
		i++;
	*end = i;
	while (i < l->count)
	{
		if (!is_blank_line(l->lines[i]))
			return (error_msg2("content after the map (empty line inside "
					"the map or element after it)", l->lines[i]));
		i++;
	}
	if (*end - start > MAP_MAX_DIM)
		return (error_msg("map too large"));
	return (0);
}

/* Largura da grade = maior linha do mapa (E.4). */
static int	map_width(const t_lines *l, size_t start, size_t end, int *cols)
{
	size_t	max;
	size_t	len;

	max = 0;
	while (start < end)
	{
		len = ft_strlen(l->lines[start]);
		if (len > max)
			max = len;
		start++;
	}
	if (max > MAP_MAX_DIM)
		return (error_msg("map too large"));
	*cols = (int)max;
	return (0);
}

/*
** Copia as linhas para uma grade rows x cols, completando com ' ' a
** direita (sem trim: espacos a esquerda fazem parte do mapa - A.5-4).
** grid tem rows + 1 entradas (a ultima NULL); rows e definido antes de
** preencher, e entradas ainda NULL sao toleradas por scene_free.
*/
static int	build_grid(const t_lines *l, size_t start, t_map *m)
{
	int		r;
	size_t	len;

	m->grid = ft_calloc((size_t)m->rows + 1, sizeof(char *));
	if (!m->grid)
		return (error_msg("memory allocation failed"));
	r = 0;
	while (r < m->rows)
	{
		m->grid[r] = malloc((size_t)m->cols + 1);
		if (!m->grid[r])
			return (error_msg("memory allocation failed"));
		ft_memset(m->grid[r], ' ', m->cols);
		m->grid[r][m->cols] = '\0';
		len = ft_strlen(l->lines[start + r]);
		ft_memcpy(m->grid[r], l->lines[start + r], len);
		r++;
	}
	return (0);
}

/*
** Le o mapa a partir de l->lines[start] (primeira linha do mapa, dada por
** parse_config) para map. Em erro retorna != 0 (ja reportado); o que foi
** alocado em map->grid fica para scene_free (chamado por parse_scene).
*/
int	parse_map(const t_lines *l, size_t start, t_map *map)
{
	size_t	end;

	if (map_bounds(l, start, &end) != 0)
		return (EXIT_ERR);
	if (map_check_chars(l, start, end) != 0)
		return (EXIT_ERR);
	if (map_width(l, start, end, &map->cols) != 0)
		return (EXIT_ERR);
	map->rows = (int)(end - start);
	if (build_grid(l, start, map) != 0)
		return (EXIT_ERR);
	return (map_find_spawn(map));
}
