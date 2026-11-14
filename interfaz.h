#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "artemis.h"

/*
 * Abre la ventana del simulador y atiende el menu principal hasta que el
 * usuario elige "Salir" o cierra la ventana. Trabaja sobre la simulacion que
 * recibe (ya inicializada por main) y deja en ella los resultados calculados.
 * Devuelve 0 si termino bien y 1 si no pudo iniciar la interfaz.
 */
int interfaz_ejecutar(Simulacion *simulacion);

#endif /* INTERFAZ_H */
