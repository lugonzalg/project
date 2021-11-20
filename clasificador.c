#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "libft.h"
#include "clasificador.h"

static char	*get_text(char *file)
{
	char	*raw_text;
	char	*tmp;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0 || fd == 2)
		return (NULL);
	raw_text = get_next_line(fd);
	tmp = get_next_line(fd);
	if (tmp)
		free(tmp);
	if (!raw_text)
		return (NULL);
	return (raw_text);
}

static char	*get_data(char **raw_ptr, char *data)
{
	char	*reference;
	char	*tmp;
	char	*raw_end;

	if (!(*raw_ptr))
		return (NULL);
	(*raw_ptr) = ft_strnstr((*raw_ptr), data, 4096);
	(*raw_ptr) = ft_strchr((*raw_ptr), ':');
	raw_end = ft_strchr((*raw_ptr), ',');
	reference = ft_substr((*raw_ptr), 0, raw_end - (*raw_ptr));
	tmp = ft_strtrim(reference, "\":,}");
	free(reference);
	(*raw_ptr) = raw_end;
	return (tmp);
}

static char	*set_data(t_datos *d, char *raw_ptr)
{
	ft_memset(d, 0, sizeof(t_datos));
	d->puntuacion = get_data(&raw_ptr, "rating\"");
	d->lenguaje = get_data(&raw_ptr, "language");
	d->fecha = get_data(&raw_ptr, "created_at"); 
	d->perfil = get_data(&raw_ptr, "alias");
	d->foto = get_data(&raw_ptr, "location");
	d->seguidores = get_data(&raw_ptr, "followers_count");
	d->seguidos = get_data(&raw_ptr, "followings_count");
	d->puntuados_total = get_data(&raw_ptr, "ratings_count");
	d->compras_total = get_data(&raw_ptr, "reviews_count");
	d->publicaciones = get_data(&raw_ptr, "purchase_order_count");
	return (raw_ptr);
}

static void	free_data(t_datos *d)
{
	free(d->puntuacion);
	free(d->perfil);
	free(d->lenguaje);
	free(d->foto);
	free(d->seguidores);
	free(d->seguidos);
	free(d->puntuados_total);
	free(d->compras_total);
	free(d->publicaciones);
	free(d->fecha);
}

int	main(int argc, char *argv[])
{
	char	*raw_text;
	char	*raw_ptr;
	FILE	*fd;
	t_datos	d;

	if (argc != 2)
		return (1);
	raw_text = get_text(argv[1]);
	if (!raw_text)
		return (2);
	raw_ptr = raw_text;
	while (*raw_ptr)
	{
		raw_ptr = set_data(&d, raw_ptr);
		if (!raw_ptr)
			break ;
		fd = fopen("list.csv", "a");
		fprintf(fd, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", d.puntuacion, d.lenguaje, d.fecha, d.perfil,
				d.foto, d.seguidores, d.seguidos, d.puntuados_total, d.compras_total,
				d.publicaciones);
		fclose(fd);
		free_data(&d);
	}
	free(raw_text);
	return (0);
}
