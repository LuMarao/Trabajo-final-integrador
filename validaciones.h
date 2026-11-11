#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#define LONGITUD_MAX_ENTRADA 100

int leer_entero(const char *mensaje, int minimo, int maximo);
double leer_real(const char *mensaje, double minimo, double maximo);
int leer_opcion_menu(const char *mensaje, int opcion_minima, int opcion_maxima);
int leer_si_no(const char *mensaje);

#endif
