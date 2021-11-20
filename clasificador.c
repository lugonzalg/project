#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "libft.h"
#include "clasificador.h"

/*la funcion recolector se encarga de recibir tanto el file descriptor de archivo a leer como el dato que se quiere extraer, lee todo el archivo y lo almacena en la variable full, que mas tarde se inspecciona en profundidad para poder obtener la informacion necesaria, para lo cual uso la funcion substr */
static char	*get_text(int fd)
{
	char		*raw_text;

	if (fd < 0 || fd == 2)
		return (NULL);
	raw_text = get_next_line(fd);
	if (!raw_text)
		return (NULL);
	return (raw_text);
}

static char	*get_data(char *raw_ptr, char *data)
{
	char	*reference;
	char	*tmp;
	size_t	i;

	raw_ptr = ft_strnstr(raw_ptr, data, 2048);
	raw_ptr = ft_strchr(raw_ptr, ':');
	i = 1;
	while (raw_ptr[i] && raw_ptr[i] != ',')
		i++;
	reference = ft_substr(raw_ptr, 0, i);
	tmp = ft_strtrim(reference, "\":,}");
	free(reference);
	raw_ptr += i;
	return (tmp);
}

static void	set_data(t_datos *datos)
{
	int	fd;
	char	*raw_text;
	char	*raw_ptr;

	ft_memset(datos, 0, sizeof(t_datos));
	fd = open("response.txt", O_RDONLY);
	raw_text = get_text(fd);
	raw_ptr = raw_text;
	if (!raw_text)
		return ;
	datos->puntuacion = get_data(raw_ptr, "rating");
	datos->lenguaje = get_data(raw_ptr, "language");
	datos->fecha = get_data(raw_ptr, "created_at"); 
	datos->perfil = get_data(raw_ptr, "alias");
	datos->foto = get_data(raw_ptr, "location");
	datos->seguidores = get_data(raw_ptr, "followers_count");
	datos->seguidos = get_data(raw_ptr, "followings_count");
	datos->puntuados_total = get_data(raw_ptr, "ratings_count");
	datos->compras_total = get_data(raw_ptr, "reviews_count");
	datos->publicaciones = get_data(raw_ptr, "purchase_order_count");
}

static void	free_data(t_datos *datos)
{
	free(datos->perfil);
	free(datos->lenguaje);
	free(datos->foto);
	free(datos->lenguaje);
	free(datos->seguidores);
	free(datos->seguidos);
	free(datos->puntuados_total);
	free(datos->compras_total);
	free(datos->publicaciones);
	free(datos->fecha);
}

int	main()
{
	t_datos	datos;

	set_data(&datos);
	free_data(&datos);
	//Connect to MarioDB
/*aqui habria que realizar una operacion que pasara estos datos a MariaDB, tras lo que se deberian igualar a NULL las variables de la estructura y hacer una recursiva sobre el propio main mientras que haya algo que leer.*/

	//QUE ESTA PENDIENTE LUKAS, QUE YA SE QUE ESOS FREES DAN CRINGE//
	
	return (0);
}
	//printf("Perfil: %s\nLenguaje: %s\nFoto: %s\nSeguidores: %s\nSeguidos: %s\nPuntuados: %s\nPublicaciones: %s\nCompras: %s\nPuntuacion: %s\nFecha: %s", datos.perfil, datos.lenguaje, datos.foto, datos.seguidores, datos.seguidos, datos.puntuados_total, datos.compras_total, datos.publicaciones, datos.puntuacion, datos.fecha); 
