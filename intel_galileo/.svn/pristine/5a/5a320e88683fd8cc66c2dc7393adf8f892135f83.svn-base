#include <mraa/aio.h>
#include <mraa.h>
#include <unistd.h>


mraa_result_t escribir(int puerto, int valor);

int main(int argc, char *argv[])
{
   if( argc != 4 )
   {
      fprintf(stdout, "La cantidad de argumentos recibido es incorrecto [%d]\n", (argc-1));
      fprintf(stdout, "La forma correcta de ejecucion es: \n $%s {puerto} {valor a escribir} {tiempo en milisegundos}.\n", argv[0]);
      fprintf(stdout, "\tDonde:\n\t\t{puerto} es el puerto digital al cual esta conectado el relay\n");
      fprintf(stdout, "\t\t{valor} es el valor a escribir en el puerto digital\n");
      fprintf(stdout, "\t\t{tiempo en milisegundos} es el tiempo que se escribira el {valor} en el {puerto},\n");
      fprintf(stdout, "\t\t\tluego se volvera a escribir el valor que tenia antes. Si en este parametro\n");
      fprintf(stdout, "\t\t\tse indica el valor 0 no se volvera a escribir el valor que ya disponia.\n");
      exit (1);
   }

   mraa_result_t estado;
   int puerto = 2;
   int valor  = 0;
   int tiempo = 0;

   int valor_negado = 0;

   puerto = atoi(argv[1]);
   valor  = atoi(argv[2]);
   tiempo = atoi(argv[3]);

   if( !((puerto >= 2) && (puerto <=13)) )
   {
      fprintf(stdout, "El valor del puerto debe ser entre 2 y 13 \n");
      exit (1);
   }

   if( (valor != 0) && (valor != 1) )
   {
      fprintf(stdout, "El valor debe ser 0 o bien 1.\n");
      exit (1);
   }

   fprintf(stdout, "puerto: %d.\n", puerto);
   fprintf(stdout, "valor : %d.\n" , valor);
   fprintf(stdout, "tiempo: %d milisegundo/s.\n", tiempo);

   (valor==0)? (valor_negado=1):(valor_negado=0);

   estado = escribir(puerto, valor);
   fprintf(stdout, "Valor escrito [%d] en el puerto [%d] con resultado [%d].\n", valor, puerto, estado);

   if( tiempo != 0 )
   {
      usleep((tiempo*1000));

      estado = escribir(puerto, valor_negado);
      fprintf(stdout, "Valor escrito [%d] en el puerto [%d] con resultado [%d].\n", valor_negado, puerto, estado);
   }

   mraa_deinit();

   return 0;
}

mraa_result_t escribir(int puerto, int valor)
{
  mraa_result_t estado;

  estado = mraa_init();
  if( estado != MRAA_SUCCESS && estado != MRAA_ERROR_PLATFORM_ALREADY_INITIALISED )
  {
     fprintf(stdout, "La placa no esta lista %d\n", estado);
     return estado;
  }

  mraa_gpio_context gpio = mraa_gpio_init (puerto);
  if( gpio != NULL )
  {
     if( mraa_gpio_dir( gpio, MRAA_GPIO_OUT ) != MRAA_SUCCESS )
     {
        fprintf (stderr, "No se puedo establecer el pin digital como salida!");
        return MRAA_ERROR_NO_RESOURCES;
     }

     estado = mraa_gpio_write(gpio, valor);
     mraa_gpio_close (gpio);

     return MRAA_SUCCESS;
  }

  return MRAA_ERROR_NO_RESOURCES;
}

