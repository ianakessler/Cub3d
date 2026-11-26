/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Uma linha por '\n'; um trecho final sem '\n' tambem conta. */
static size_t	count_lines(const char *buf, size_t len)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	while (i < len)
	{
		if (buf[i] == '\n')
			n++;
		i++;
	}
	if (len > 0 && buf[len - 1] != '\n')
		n++;
	return (n);
}

static char	*dup_range(const char *s, size_t n)
{
	char	*dst;

	dst = malloc(n + 1);
	if (!dst)
		return (NULL);
	ft_memcpy(dst, s, n);
	dst[n] = '\0';
	return (dst);
}

/*
** Copia cada linha de buf para out->lines (ja alocado com count + 1).
** Em falha de malloc libera o que montou e retorna 1.
*/
static int	fill_lines(const char *buf, size_t len, t_lines *out)
{
	size_t	start;
	size_t	end;
	size_t	n;

	start = 0;
	n = 0;
	while (n < out->count)
	{
		end = start;
		while (end < len && buf[end] != '\n')
			end++;
		out->lines[n] = dup_range(buf + start, end - start);
		if (!out->lines[n])
		{
			out->count = n;
			lines_free(out);
			return (1);
		}
		n++;
		start = end + 1;
	}
	out->lines[n] = NULL;
	return (0);
}

/*
** Divide buf (len bytes, sem '\0' interno) em out->lines, terminado por
** NULL. Retorna 0 em sucesso; 1 em falha de malloc, com out zerado.
*/
int	split_lines(const char *buf, size_t len, t_lines *out)
{
	out->count = count_lines(buf, len);
	out->lines = malloc(sizeof(char *) * (out->count + 1));
	if (!out->lines)
	{
		out->count = 0;
		return (1);
	}
	return (fill_lines(buf, len, out));
}

/* Idempotente: pode ser chamada em t_lines zerado ou ja liberado. */
void	lines_free(t_lines *l)
{
	size_t	i;

	if (!l->lines)
	{
		l->count = 0;
		return ;
	}
	i = 0;
	while (i < l->count)
		free(l->lines[i++]);
	free(l->lines);
	l->lines = NULL;
	l->count = 0;
}
