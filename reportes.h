#ifndef REPORTES_H
#define REPORTES_H

#include "artemis.h"

#define LARGO_LINEA_REPORTE  72   /* maximo de caracteres por linea (con el '\0') */
#define MAX_LINEAS_REPORTE   19   /* maximo de lineas que entran en pantalla */

/* Recibe cada linea ya formateada. */
typedef void (*EmisorLinea)(void *contexto, const char *texto);

/* Un reporte: arma sus lineas a partir de la simulacion y las manda al emisor. */
typedef void (*FuncionReporte)(const Simulacion *sim, EmisorLinea emitir, void *contexto);

void reporte_parametros(const Simulacion *sim, EmisorLinea emitir, void *contexto);
void reporte_orbita(const Simulacion *sim, EmisorLinea emitir, void *contexto);
void reporte_pulso(const Simulacion *sim, EmisorLinea emitir, void *contexto);
void reporte_historial(const Simulacion *sim, EmisorLinea emitir, void *contexto);
void reporte_resumen(const Simulacion *sim, EmisorLinea emitir, void *contexto);


void ui_titulo(const char *titulo);
void imprimir_reporte(const char *titulo, FuncionReporte reporte, const Simulacion *sim);

#endif
