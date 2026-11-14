#include <stdio.h>
#include "artemis.h"
#include "reportes.h"
#include "interfaz.h"

int main(void)
{
    Simulacion sim = {0};
    int estado;

    cargar_valores_originales(&sim.orbita, &sim.propulsor);

    estado = interfaz_ejecutar(&sim);
    if (estado != 0) {
        return estado;
    }

    ui_titulo("REPORTE FINAL DE LA SESION");
    imprimir_reporte("RESUMEN", reporte_resumen, &sim);
    imprimir_reporte("HISTORIAL DE CALCULOS", reporte_historial, &sim);
    printf("\nGracias por usar el simulador Artemis.\n");
    return 0;
}
