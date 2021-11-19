#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/*la funcion recolector se encarga de recibir tanto el file descriptor de archivo a leer como el dato que se quiere extraer, lee todo el archivo y lo almacena en la variable full, que mas tarde se inspecciona en profundidad para poder obtener la informacion necesaria, para lo cual uso la funcion substr */
char 	*recolector(int fd, char *data)
{
	int		bytes;
	char 		*buffer; 
	char		*full; 
	int		size = 2047; //los bytes que leere cada pasada, para llegar cuanto antes al final del archivo
	int		i; //i y j son contadores que uso para definir los limites de output
	int		j;
	char		*reference;

	if (fd < 0 || fd == 2)
		exit (1);
	if (!(buffer = malloc(size + 1)))
		exit (1);
	bytes = 1;
	while (bytes > 0)	//relleno la variable full con todo el archivo
	{
		bytes = read(fd, buffer, size);
		buffer[bytes] = 0;
		full = ft_strjoin(full, buffer);
	}
	free(buffer);
	i = -1;
	reference = strstr(full, data); //busco la palabra que corresponde al dato que estamos buscando
	i = 0;
	while (reference[i] != ':')
		i++;
	j = 0; 
/* Cada dato tiene sus propias limitaciones en cuanto a simbolos o letras, por lo que he tenido que personalizar la recogida de datos en funcion del dato introducido en funcion de si es un numero o una cadena de caracteres. La fecha y las compras del usuario tienen una condicion especial, puesto que sus limites y display son diferentes de las demas */
	if (data != "followers_count" || data != "followings_count" || data != "reviews_count" || data != "rating")
	{
		while (reference[j] != '"')
			j++;
	}
	if(data == "created_at")
	{
		while (reference[j] != 'T')
			j++;
	}
	j++;
	if (data != "created_at" && data != "publicaciones")
	{
		while (reference[j] != ',')
			j++;
	}
	if (data == "created_at")
		reference = ft_substr(reference, i + 2, j - 1);	//caso especial en caso de que estemos recogiendo la fecha
	else if (data == "purchase_order_count")
		reference = ft_substr(reference, i + 1, j - 1); //caso especial en caso de que estemos recogiendo el numero de compras realizadas por el usuario
	else
		reference = ft_substr(reference, i + 1, j);
	return (reference);
}

static void	set_data(t_datos *datos)
{
	int	fd;

	ft_memset(datos, 0, sizeof(t_datos));
	fd = open("response.txt", O_RDONLY);
	datos.perfil = recolector(fd, "alias");
	datos.lenguaje = recolector(fd, "language");
	datos.foto = recolector(fd, "location");
	datos.seguidores = recolector(fd, "followers_count");
	datos.seguidos = recolector(fd, "followings_count");
	datos.puntuados_total = recolector(fd, "ratings_count");
	datos.compras_total = recolector(fd, "reviews_count");
	datos.publicaciones = recolector(fd, "purchase_order_count");
	datos.puntuacion = recolector(fd, "rating");
	datos.fecha = recolector(fd, "created_at"); 

}

static void	free_data(t_datos *datos)
{
	free(datos.perfil);
	free(datos.lenguaje);
	free(datos.foto);
	free(datos.lenguaje);
	free(datos.seguidores);
	free(datos.seguidos);
	free(datos.puntuados_total);
	free(datos.compras_total);
	free(datos.publicaciones);
	free(datos.fecha);
}

int	main()
{
	/* Ah, y lukas, no consigo hacer que full entre vacio la priemera vez, quiero evitar que si tiene algun contenido no pase por el bucle, pero no lo consigo, he usado if (!full) para la primera vez (ahora borrado) y no me iba */
	t_datos	datos;

	set_data(&datos);
	printf("Perfil: %s\nLenguaje: %s\nFoto: %s\nSeguidores: %s\nSeguidos: %s\nPuntuados: %s\nPublicaciones: %s\nCompras: %s\nPuntuacion: %s\nFecha: %s", datos.perfil, datos.lenguaje, datos.foto, datos.seguidores, datos.seguidos, datos.puntuados_total, datos.compras_total, datos.publicaciones, datos.puntuacion, datos.fecha); 

/*aqui habria que realizar una operacion que pasara estos datos a MariaDB, tras lo que se deberian igualar a NULL las variables de la estructura y hacer una recursiva sobre el propio main mientras que haya algo que leer.*/

	//QUE ESTA PENDIENTE LUKAS, QUE YA SE QUE ESOS FREES DAN CRINGE//
	
	return (0);
}
