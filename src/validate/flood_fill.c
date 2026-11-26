/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Marca e empilha (r, c) se ainda nao visitada e nao for parede.
** Retorna 1 se (r, c) e vazio exterior (fora da grade ou espaco).
*/
static int	fill_push(t_fill *f, int r, int c)
{
	size_t	i;

	if (r < 0 || c < 0 || r >= f->map->rows || c >= f->map->cols)
		return (1);
	if (f->map->grid[r][c] == ' ')
		return (1);
	if (f->map->grid[r][c] == '1')
		return (0);
	i = (size_t)r * f->map->cols + c;
	if (f->visited[i])
		return (0);
	f->visited[i] = 1;
	f->stack[f->top++] = i;
	return (0);
}

/* Flood fill iterativo (pilha explicita, 4 vizinhos) a partir do spawn. */
static int	fill_run(t_fill *f)
{
	size_t	i;
	int		r;
	int		c;

	fill_push(f, f->map->player_row, f->map->player_col);
	while (f->top > 0)
	{
		i = f->stack[--f->top];
		r = (int)(i / f->map->cols);
		c = (int)(i % f->map->cols);
		if (fill_push(f, r - 1, c) || fill_push(f, r + 1, c)
			|| fill_push(f, r, c - 1) || fill_push(f, r, c + 1))
			return (error_at("map is not closed: the player can reach "
					"the void", r + 1, c + 1));
	}
	return (0);
}

/*
** Flood fill a partir do spawn com visited e pilha proprios (capacidade
** rows * cols), liberados aqui em qualquer caso. Falha se a regiao do
** jogador alcanca espaco ou a borda da grade. Areas de '0' separadas do
** spawn NAO sao erro: o proprio exemplo do PDF (p. 12/14) tem uma; elas
** sao cobertas pela checagem de vizinhanca de validate_map.
*/
int	check_connected(const t_map *map)
{
	t_fill	f;
	size_t	n;
	int		ret;

	n = (size_t)map->rows * map->cols;
	f.map = map;
	f.top = 0;
	f.visited = ft_calloc(n, sizeof(char));
	f.stack = ft_calloc(n, sizeof(size_t));
	if (!f.visited || !f.stack)
		ret = error_msg("memory allocation failed");
	else
	{
		ret = fill_run(&f);
	}
	free(f.visited);
	free(f.stack);
	return (ret);
}
