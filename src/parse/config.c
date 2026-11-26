/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* s comeca exatamente com id, seguido de separador ou fim de linha. */
static int	match_id(const char *s, const char *id)
{
	size_t	n;

	n = ft_strlen(id);
	if (ft_strncmp(s, id, n) != 0)
		return (0);
	return (s[n] == '\0' || is_sep(s[n]));
}

/*
** Retorna 0 se processou o elemento, 1 em erro ja reportado e -1 se a
** linha nao comeca com um identificador (maiusculas apenas).
*/
static int	parse_element(const char *s, t_scene *sc)
{
	if (match_id(s, "NO"))
		return (parse_texture(s + 2, &sc->north_path, "NO"));
	if (match_id(s, "SO"))
		return (parse_texture(s + 2, &sc->south_path, "SO"));
	if (match_id(s, "WE"))
		return (parse_texture(s + 2, &sc->west_path, "WE"));
	if (match_id(s, "EA"))
		return (parse_texture(s + 2, &sc->east_path, "EA"));
	if (match_id(s, "F"))
		return (parse_color(s + 1, &sc->floor_rgb, "F"));
	if (match_id(s, "C"))
		return (parse_color(s + 1, &sc->ceil_rgb, "C"));
	return (-1);
}

static int	check_all_defined(const t_scene *s)
{
	if (!s->north_path)
		return (error_msg2("missing identifier", "NO"));
	if (!s->south_path)
		return (error_msg2("missing identifier", "SO"));
	if (!s->west_path)
		return (error_msg2("missing identifier", "WE"));
	if (!s->east_path)
		return (error_msg2("missing identifier", "EA"));
	if (s->floor_rgb < 0)
		return (error_msg2("missing identifier", "F"));
	if (s->ceil_rgb < 0)
		return (error_msg2("missing identifier", "C"));
	return (0);
}

/*
** Percorre as linhas da configuracao (ordem livre, linhas vazias opcionais
** entre elementos - A.5-2). Para na primeira linha nao vazia que nao e
** identificador e parece mapa; *map_start recebe o indice dela (ou
** l->count se o arquivo acabou). Exige os seis identificadores.
** Em erro retorna != 0 (ja reportado); o chamador faz scene_free.
*/
int	parse_config(const t_lines *l, t_scene *s, size_t *map_start)
{
	size_t	i;
	int		ret;

	i = 0;
	while (i < l->count)
	{
		if (!is_blank_line(l->lines[i]))
		{
			ret = parse_element(skip_sep(l->lines[i]), s);
			if (ret > 0)
				return (ret);
			if (ret < 0 && is_map_line(l->lines[i]))
				break ;
			if (ret < 0)
				return (error_msg2("invalid line (unknown identifier?)",
						l->lines[i]));
		}
		i++;
	}
	*map_start = i;
	return (check_all_defined(s));
}
