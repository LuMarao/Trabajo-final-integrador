#include <stdio.h>
#include "artemis.h"
#include "propulsor.h"
#include "interfaz.h"

static void mostrar_reporte_final(const Simulacion *sim)
{
    ui_titulo("REPORTE FINAL DE LA SESION");

    if (!sim->orbita_calculada && !sim->pulso_calculado) {
        printf("  No se realizaron calculos en esta sesion.\n");
        return;
    }
    if (sim->orbita_calculada) {
        mostrar_resultados_orbita(&sim->orbita, &sim->res_orbita);
    }
    if (sim->pulso_calculado) {
        mostrar_resultados_propulsor(&sim->res_orbita, &sim->propulsor, &sim->res_propulsor);
    }
}

int main(void)
