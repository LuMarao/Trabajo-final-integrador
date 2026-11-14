#include <stdio.h>
#include "artemis.h"
#include "historial.h"

/* Reserva un registro al final; si el historial esta lleno, descarta el mas antiguo. */
static RegistroHistorial *reservar_registro(Simulacion *sim)
{
    RegistroHistorial *registro;
    int i;

    if (sim->cantidad_historial >= MAX_HISTORIAL) {
        for (i = 1; i < MAX_HISTORIAL; i++) {
            sim->historial[i - 1] = sim->historial[i];
        }
        sim->cantidad_historial = MAX_HISTORIAL - 1;
    }
    registro = &sim->historial[sim->cantidad_historial];
    sim->cantidad_historial++;

    *registro = (RegistroHistorial){ (TipoRegistro)0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    return registro;
}

void historial_agregar_orbita(Simulacion *sim)
{
    RegistroHistorial *registro = reservar_registro(sim);

    registro->tipo              = REGISTRO_ORBITA;
    registro->altitud           = sim->orbita.altitud;
    registro->velocidad_orbital = sim->res_orbita.velocidad_orbital;
    registro->periodo_min       = sim->res_orbita.periodo_min;
}

void historial_agregar_pulso(Simulacion *sim)
{
    RegistroHistorial *registro = reservar_registro(sim);

    registro->tipo               = REGISTRO_PULSO;
    registro->aceleracion        = sim->propulsor.aceleracion;
    registro->duracion           = sim->propulsor.duracion;
    registro->velocidad_lateral  = sim->res_propulsor.velocidad_lateral;
    registro->desviacion_lateral = sim->res_propulsor.desviacion_lateral;
}

void historial_formatear(const RegistroHistorial *registro, int numero,
                         char *destino, size_t tamano)
{
    if (registro->tipo == REGISTRO_ORBITA) {
        snprintf(destino, tamano, "%2d) Orbita h=%.0f m v=%.2f m/s T=%.2f min",
                 numero, registro->altitud, registro->velocidad_orbital,
                 registro->periodo_min);
    } else {
        snprintf(destino, tamano, "%2d) Pulso a=%.2f dt=%.1f dv=%.2f m/s d=%.0f m",
                 numero, registro->aceleracion, registro->duracion,
                 registro->velocidad_lateral, registro->desviacion_lateral);
    }
}
