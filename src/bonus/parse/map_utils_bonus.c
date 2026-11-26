/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static int	is_spawn(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/* "invalid character 'X' in map" (nao imprimiveis viram '?'). */
static int	bad_char(char c, size_t row, size_t col)
{
	char	msg[32];

	ft_strlcpy(msg, "invalid character '?' in map", sizeof(msg));
	if (ft_isprint((unsigned char)c))
		msg[19] = c;
	return (error_at(msg, (int)row + 1, (int)col + 1));
}

/*
** Mapa so com "01NSEW" e espaco (R24); bonus (fase17): tambem 'D' (porta,
** sempre fechada no inicio) e '2' (sprite, fase18). Tab e 'd' sao
** invalidos no mapa.
*/
int	map_check_chars(const t_lines *l, size_t start, size_t end)
{
	size_t		r;
	size_t		c;
	const char	*s;

	r = start;
	while (r < end)
	{
		s = l->lines[r];
		c = 0;
		while (s[c])
		{
			if (s[c] != ' ' && s[c] != '0' && s[c] != '1' && !is_spawn(s[c])
				&& s[c] != DOOR_CLOSED && s[c] != SPRITE_CELL)
				return (bad_char(s[c], r - start, c));
			c++;
		}
		r++;
	}
	return (0);
}

/*
** Exatamente um spawn (A.5-1). Guarda posicao/orientacao em map e troca
** a celula do spawn por '0' na grade (ver comentario em cub3d.h).
*/
int	map_find_spawn(t_map *m)
{
	int	r;
	int	c;
	int	found;

	found = 0;
	r = -1;
	while (++r < m->rows)
	{
		c = -1;
		while (++c < m->cols)
		{
			if (!is_spawn(m->grid[r][c]))
				continue ;
			if (found++)
				return (error_at("more than one player spawn", r + 1, c + 1));
			m->player_row = r;
			m->player_col = c;
			m->player_dir = m->grid[r][c];
		}
	}
	if (!found)
		return (error_msg("no player spawn (N, S, E or W) in the map"));
	m->grid[m->player_row][m->player_col] = '0';
	return (0);
}
