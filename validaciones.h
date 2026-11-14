#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#define LONGITUD_MAX_ENTRADA 100

/* Codigos que devuelve convertir_real(). */
#define VALIDACION_OK       0
#define VALIDACION_FORMATO  1   /* el texto no es un numero valido */
#define VALIDACION_RANGO    2   /* el numero esta fuera de [minimo, maximo] */

/* Lectura por consola: repiten la consulta hasta recibir un dato valido. */
int    leer_entero(const char *mensaje, int minimo, int maximo);
double leer_real(const char *mensaje, double minimo, double maximo);
int    leer_opcion_menu(const char *mensaje, int opcion_minima, int opcion_maxima);
int    leer_si_no(const char *mensaje);

/* Valida un texto ya ingresado. */
int    convertir_real(const char *texto, double minimo, double maximo, double *resultado);

#endif 
