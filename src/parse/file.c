/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Dobra a capacidade do buffer. Em falha libera o buffer antigo e o
** deixa NULL, para que o chamador so precise reportar o erro.
*/
static int	grow_buffer(char **buf, size_t len, size_t *cap)
{
	char	*bigger;

	bigger = malloc(*cap * 2);
	if (!bigger)
	{
		free(*buf);
		*buf = NULL;
		return (1);
	}
	ft_memcpy(bigger, *buf, len);
	free(*buf);
	*buf = bigger;
	*cap *= 2;
	return (0);
}

/*
** Le o fd ate read() devolver 0 (EOF). Leituras parciais sao normais e
** apenas acumulam no buffer. read() < 0 (ex.: EISDIR) e erro.
** Em erro o buffer ja foi liberado e a mensagem ja foi escrita.
*/
static int	read_all(int fd, const char *path, char **buf, size_t *len)
{
	size_t	cap;
	ssize_t	ret;

	cap = READ_CHUNK;
	*len = 0;
	*buf = malloc(cap);
	if (!*buf)
		return (error_msg("memory allocation failed"));
	ret = 1;
	while (ret > 0)
	{
		if (cap - *len < READ_CHUNK && grow_buffer(buf, *len, &cap))
			return (error_msg("memory allocation failed"));
		ret = read(fd, *buf + *len, READ_CHUNK);
		if (ret > 0)
			*len += ret;
	}
	if (ret < 0)
	{
		ret = errno;
		free(*buf);
		*buf = NULL;
		return (error_msg2(path, strerror(ret)));
	}
	return (0);
}

/*
** Le o arquivo inteiro e o divide em linhas (sem o '\n' final, sem trim;
** linhas vazias viram ""). O fd e sempre fechado. Retorna 0 em sucesso;
** != 0 em erro ja reportado, com out zerado (lines_free e seguro).
*/
int	read_lines(const char *path, t_lines *out)
{
	int		fd;
	char	*buf;
	size_t	len;
	int		ret;

	out->lines = NULL;
	out->count = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg2(path, strerror(errno)));
	ret = read_all(fd, path, &buf, &len);
	close(fd);
	if (ret != 0)
		return (ret);
	if (ft_memchr(buf, '\0', len))
		ret = error_msg2(path, "invalid file: contains a NUL byte");
	else if (split_lines(buf, len, out) != 0)
		ret = error_msg("memory allocation failed");
	free(buf);
	return (ret);
}
