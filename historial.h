#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <stddef.h>
#include "artemis.h"

void historial_agregar_orbita(Simulacion *sim);
void historial_agregar_pulso(Simulacion *sim);

/* Arma una linea de texto con el registro. */
void historial_formatear(const RegistroHistorial *registro, int numero,
                         char *destino, size_t tamano);

#endif /* HISTORIAL_H */
