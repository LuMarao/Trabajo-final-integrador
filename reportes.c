#include <stdio.h>
#include <stdarg.h>
#include "artemis.h"
#include "historial.h"
#include "reportes.h"

static void emitir_formato(EmisorLinea emitir, void *contexto, const char *formato, ...)
{
    char texto[LARGO_LINEA_REPORTE];
    va_list argumentos;

    va_start(argumentos, formato);
    vsnprintf(texto, sizeof(texto), formato, argumentos);
    va_end(argumentos);
    emitir(contexto, texto);
}

void reporte_parametros(const Simulacion *sim, EmisorLinea emitir, void *contexto)
{
    emitir_formato(emitir, contexto, "Datos actualizados. Use la opcion de calcular la orbita");
    emitir_formato(emitir, contexto, "para obtener los resultados con estos valores.");
    emitir_formato(emitir, contexto, "");
    emitir_formato(emitir, contexto, "  Masa de la Tierra  MT = %.3e kg", sim->orbita.masa_tierra);
    emitir_formato(emitir, contexto, "  Radio terrestre    RT = %.0f m", sim->orbita.radio_tierra);
    emitir_formato(emitir, contexto, "  Altitud orbital    h  = %.0f m", sim->orbita.altitud);
    emitir_formato(emitir, contexto, "  Masa de Orion      m  = %.2f kg", sim->orbita.masa_nave);
    emitir_formato(emitir, contexto, "  Pulso lateral      a  = %.3f m/s^2, dt = %.2f s",
                   sim->propulsor.aceleracion, sim->propulsor.duracion);
}

void reporte_orbita(const Simulacion *sim, EmisorLinea emitir, void *contexto)
{
    const ResultadosOrbita *r = &sim->res_orbita;
    double fc = calcular_fuerza_centripeta(&sim->orbita);
    double diferencia = 0.0;

    if (r->fuerza_gravitatoria != 0.0) {
        diferencia = (r->fuerza_gravitatoria - fc) / r->fuerza_gravitatoria;
        if (diferencia < 0.0) {
            diferencia = -diferencia;
        }
    }

    emitir_formato(emitir, contexto, "h = %.0f m | m = %.2f kg | r = RT + h = %.0f m",
                   sim->orbita.altitud, sim->orbita.masa_nave, r->radio_orbital);
    emitir_formato(emitir, contexto, "");
    emitir_formato(emitir, contexto, "A - La gravedad actua como fuerza centripeta (Fg = Fc):");
    emitir_formato(emitir, contexto, "    Fg = G*MT*m / r^2     = %.2f N", r->fuerza_gravitatoria);
    emitir_formato(emitir, contexto, "    Fc = m*v^2 / r        = %.2f N", fc);
    emitir_formato(emitir, contexto, "    v  = sqrt(G*MT / r)   = %.2f m/s (%.0f km/h)",
                   r->velocidad_orbital, r->velocidad_orbital * 3.6);
    emitir_formato(emitir, contexto, "    Diferencia relativa |Fg - Fc| / Fg = %.1e", diferencia);
    if (diferencia < 1e-9) {
        emitir_formato(emitir, contexto, "    => Fg y Fc coinciden; la masa de la nave se cancela.");
    }
    emitir_formato(emitir, contexto, "");
    emitir_formato(emitir, contexto, "B - Periodo orbital:  T = 2*pi*r / v");
    emitir_formato(emitir, contexto, "    T = %.2f s = %.2f min", r->periodo_s, r->periodo_min);
    emitir_formato(emitir, contexto, "");
    emitir_formato(emitir, contexto, "C - Aceleracion centripeta:  ac = v^2 / r");
    emitir_formato(emitir, contexto, "    ac = %.3f m/s^2", r->aceleracion_centripeta);
    emitir_formato(emitir, contexto, "    (igual a la gravedad g = G*MT/r^2 a esta altura)");
}

void reporte_pulso(const Simulacion *sim, EmisorLinea emitir, void *contexto)
{
    const ResultadosPropulsor *p = &sim->res_propulsor;

    emitir_formato(emitir, contexto, "Pulso: a = %.3f m/s^2 durante dt = %.1f s",
                   sim->propulsor.aceleracion, sim->propulsor.duracion);
    emitir_formato(emitir, contexto, "Velocidad orbital antes del pulso: v = %.2f m/s",
                   sim->res_orbita.velocidad_orbital);
    emitir_formato(emitir, contexto, "");
    emitir_formato(emitir, contexto, "1) Velocidad lateral:  dv = a*dt = %.3f m/s", p->velocidad_lateral);
    emitir_formato(emitir, contexto, "2) Desviacion lateral: d = 1/2*a*dt^2 = %.3f m", p->desviacion_lateral);
    emitir_formato(emitir, contexto, "3) Nueva rapidez: v' = sqrt(v^2 + dv^2) = %.4f m/s", p->velocidad_nueva);
    emitir_formato(emitir, contexto, "4) Giro del plano orbital: atan(dv/v) = %.5f grados", p->giro_grados);
    emitir_formato(emitir, contexto, "");
    emitir_formato(emitir, contexto, "Trayectoria: la nave sigue en orbita (la gravedad sigue");
    emitir_formato(emitir, contexto, "actuando como fuerza centripeta). La velocidad lateral se");
    emitir_formato(emitir, contexto, "suma a la orbital: el plano de la orbita se inclina %.5f", p->giro_grados);
    emitir_formato(emitir, contexto, "grados y deja de ser perfectamente circular");
    emitir_formato(emitir, contexto, "(excentricidad %.3e; el apoapsis sube %.2f m).",
                   p->excentricidad, p->aumento_apoapsis);
    emitir_formato(emitir, contexto, "Un cuarto de vuelta despues, la nave queda a ~%.0f m de", p->separacion_maxima);
    emitir_formato(emitir, contexto, "su orbita original.");
    emitir_formato(emitir, contexto, "(Se supone un pulso lateral, perpendicular a la velocidad.)");
}

void reporte_historial(const Simulacion *sim, EmisorLinea emitir, void *contexto)
{
    char texto[LARGO_LINEA_REPORTE];
    int i;

    if (sim->cantidad_historial == 0) {
        emitir_formato(emitir, contexto, "Todavia no hay calculos registrados.");
        emitir_formato(emitir, contexto, "Calcule la orbita o simule un pulso lateral para llenarlo.");
        return;
    }
    emitir_formato(emitir, contexto, "Ultimos %d calculos (el mas reciente al final):",
                   sim->cantidad_historial);
    emitir_formato(emitir, contexto, "");
    for (i = 0; i < sim->cantidad_historial; i++) {
        historial_formatear(&sim->historial[i], i + 1, texto, sizeof(texto));
        emitir_formato(emitir, contexto, "%s", texto);
    }
}

void reporte_resumen(const Simulacion *sim, EmisorLinea emitir, void *contexto)
{
    if (!sim->orbita_calculada && !sim->pulso_calculado) {
        emitir_formato(emitir, contexto, "Todavia no hay resultados.");
        emitir_formato(emitir, contexto, "Calcule la orbita y simule el pulso lateral primero.");
        return;
    }
    if (sim->orbita_calculada) {
        emitir_formato(emitir, contexto, "ORBITA: h = %.0f m, m = %.2f kg",
                       sim->orbita.altitud, sim->orbita.masa_nave);
        emitir_formato(emitir, contexto, "  Fuerza gravitatoria   Fg = %.2f N", sim->res_orbita.fuerza_gravitatoria);
        emitir_formato(emitir, contexto, "  Velocidad orbital     v  = %.2f m/s", sim->res_orbita.velocidad_orbital);
        emitir_formato(emitir, contexto, "  Periodo               T  = %.2f s = %.2f min",
                       sim->res_orbita.periodo_s, sim->res_orbita.periodo_min);
        emitir_formato(emitir, contexto, "  Aceleracion centripeta ac = %.3f m/s^2",
                       sim->res_orbita.aceleracion_centripeta);
    }
    if (sim->pulso_calculado) {
        emitir_formato(emitir, contexto, "");
        emitir_formato(emitir, contexto, "PULSO LATERAL: a = %.3f m/s^2, dt = %.1f s",
                       sim->propulsor.aceleracion, sim->propulsor.duracion);
        emitir_formato(emitir, contexto, "  Velocidad lateral     dv = %.3f m/s", sim->res_propulsor.velocidad_lateral);
        emitir_formato(emitir, contexto, "  Desviacion lateral    d  = %.3f m", sim->res_propulsor.desviacion_lateral);
        emitir_formato(emitir, contexto, "  Nueva rapidez         v' = %.4f m/s", sim->res_propulsor.velocidad_nueva);
        emitir_formato(emitir, contexto, "  Giro del plano orbital    = %.5f grados", sim->res_propulsor.giro_grados);
    }
}

/* ---------------------------- Salida por consola ---------------------------- */

void ui_titulo(const char *titulo)
{
    printf("\n==== %s ====\n", titulo);
}

static void emitir_en_consola(void *contexto, const char *texto)
{
    (void)contexto;
    if (texto[0] == '\0') {
        printf("\n");
    } else {
        printf("  %s\n", texto);
    }
}

void imprimir_reporte(const char *titulo, FuncionReporte reporte, const Simulacion *sim)
{
    ui_titulo(titulo);
    reporte(sim, emitir_en_consola, NULL);
}
