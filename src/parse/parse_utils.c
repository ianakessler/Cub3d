/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Separadores de informacao na configuracao (decisao A.5-3). */
int	is_sep(char c)
{
	return (c == ' ' || c == '\t');
}

char	*skip_sep(const char *s)
{
	while (is_sep(*s))
		s++;
	return ((char *)s);
}

/* Linha vazia ou so com espacos/tabs. */
int	is_blank_line(const char *s)
{
	return (*skip_sep(s) == '\0');
}

/*
** Linha que pode ser do mapa: nao vazia, so com " 01NSEW" e ao menos um
** caractere diferente de espaco. Usada para detectar o inicio do mapa;
** a validacao completa e da fase04.
*/
int	is_map_line(const char *s)
{
	int	has_cell;

	has_cell = 0;
	while (*s)
	{
		if (!ft_strchr(" 01NSEW", *s))
			return (0);
		if (*s != ' ')
			has_cell = 1;
		s++;
	}
	return (has_cell);
}
