#ifndef PROPULSOR_H
#define PROPULSOR_H

#include "artemis.h"

void propulsor_valores_defecto(DatosPropulsor *prop);

/* Entrada por consola de los datos del pulso (usa validaciones.h). */
void propulsor_leer_datos(DatosPropulsor *prop);

void calcular_pulso_lateral(const ResultadosOrbita *orbita, const DatosPropulsor *prop,
                            ResultadosPropulsor *res);

#endif
