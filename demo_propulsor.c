#include <stdio.h>
#include "artemis.h"
#include "propulsor.h"
#include "reportes.h"
#include "validaciones.h"

int main(void)
{
    Simulacion sim = {0};

    cargar_valores_originales(&sim.orbita, &sim.propulsor);
    calcular_orbita(&sim.orbita, &sim.res_orbita);
    sim.orbita_calculada = 1;

    propulsor_leer_datos(&sim.propulsor);
    calcular_pulso_lateral(&sim.res_orbita, &sim.propulsor, &sim.res_propulsor);
    sim.pulso_calculado = 1;

    imprimir_reporte("PUNTO D - PROPULSOR LATERAL", reporte_pulso, &sim);
    return 0;
}
