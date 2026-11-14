#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "propulsor.h"
#include "historial.h"
#include "reportes.h"
#include <string.h>
#include "validaciones.h"

static int fallos = 0;

/* Emisor de prueba: cuenta lineas y recuerda la mas larga. */
typedef struct {
    int lineas;
    int largo_max;
} MedidaReporte;

static void medir_linea(void *contexto, const char *texto)
{
    MedidaReporte *m = (MedidaReporte *)contexto;
    int largo = (int)strlen(texto);

    m->lineas++;
    if (largo > m->largo_max)
        m->largo_max = largo;
}

static void check(const char *nombre, double obtenido, double esperado, double tol)
{
    double err = (esperado != 0.0) ? fabs(obtenido - esperado) / fabs(esperado) : fabs(obtenido);
    int ok = (err <= tol);
    printf("  [%s] %-30s obtenido = %-14.8g esperado = %-14.8g\n",
           ok ? "PASS" : "FAIL", nombre, obtenido, esperado);
    if (!ok)
        fallos++;
}

static void check_entero(const char *nombre, int obtenido, int esperado)
{
    int ok = (obtenido == esperado);
    printf("  [%s] %-30s obtenido = %-14d esperado = %-14d\n",
           ok ? "PASS" : "FAIL", nombre, obtenido, esperado);
    if (!ok)
        fallos++;
}

int main(void)
{
    DatosOrbita orbita;
    DatosPropulsor prop;
    Simulacion sim = {0};
    char texto[120];
    int i;
    MedidaReporte medida;
    ResultadosOrbita o;
    ResultadosPropulsor p;
    double valor = 0.0;

    cargar_valores_originales(&orbita, &prop);

    printf("== Orbita de la consigna (h = 192 km)\n");
    calcular_orbita(&orbita, &o);
    check("radio orbital [m]", o.radio_orbital, 6563000.0, 1e-12);
    check("fuerza gravitatoria [N]", o.fuerza_gravitatoria, 305362.0, 1e-3);
    check("velocidad orbital [m/s]", o.velocidad_orbital, 7792.95, 1e-4);
    check("periodo [s]", o.periodo_s, 5291.52, 1e-4);
    check("periodo [min]", o.periodo_min, 88.19, 1e-3);
    check("aceleracion centripeta", o.aceleracion_centripeta, 9.253, 1e-3);

    printf("== Datos de la consigna (a = 0.5 m/s^2, dt = 10 s)\n");
    propulsor_valores_defecto(&prop);
    check("a por defecto", prop.aceleracion, 0.5, 1e-12);
    check("dt por defecto", prop.duracion, 10.0, 1e-12);
    calcular_pulso_lateral(&o, &prop, &p);
    check("velocidad lateral dv [m/s]", p.velocidad_lateral, 5.0, 1e-12);
    check("desviacion 1/2 a dt^2 [m]", p.desviacion_lateral, 25.0, 1e-12);
    check("nueva rapidez [m/s]", p.velocidad_nueva,
          sqrt(o.velocidad_orbital * o.velocidad_orbital + 25.0), 1e-12);
    check("giro del plano [grados]", p.giro_grados, 0.03676, 1e-3);

    printf("== Otro evento (a = 2 m/s^2, dt = 30 s)\n");
    prop.aceleracion = 2.0;
    prop.duracion = 30.0;
    calcular_pulso_lateral(&o, &prop, &p);
    check("dv [m/s]", p.velocidad_lateral, 60.0, 1e-12);
    check("desv [m]", p.desviacion_lateral, 900.0, 1e-12);

    printf("== Sin pulso (a = 0): no hay efecto\n");
    prop.aceleracion = 0.0;
    calcular_pulso_lateral(&o, &prop, &p);
    check("dv", p.velocidad_lateral, 0.0, 1e-12);
    check("giro", p.giro_grados, 0.0, 1e-12);

    printf("== Validacion de texto (convertir_real)\n");
    check_entero("'12.5' en rango", convertir_real("12.5", 0, 100, &valor), VALIDACION_OK);
    check_entero("'12,5' (coma decimal)", convertir_real("12,5", 0, 100, &valor), VALIDACION_OK);
    check_entero("'abc' no es numero", convertir_real("abc", 0, 100, &valor), VALIDACION_FORMATO);
    check_entero("'' vacio", convertir_real("", 0, 100, &valor), VALIDACION_FORMATO);
    check_entero("'12abc' mezcla", convertir_real("12abc", 0, 100, &valor), VALIDACION_FORMATO);
    check_entero("'150' fuera de rango", convertir_real("150", 0, 100, &valor), VALIDACION_RANGO);
    check_entero("'-1' fuera de rango", convertir_real("-1", 0, 100, &valor), VALIDACION_RANGO);

    printf("== Historial\n");
    cargar_valores_originales(&sim.orbita, &sim.propulsor);
    check_entero("historial vacio", sim.cantidad_historial, 0);
    calcular_orbita(&sim.orbita, &sim.res_orbita);
    historial_agregar_orbita(&sim);
    calcular_pulso_lateral(&sim.res_orbita, &sim.propulsor, &sim.res_propulsor);
    historial_agregar_pulso(&sim);
    check_entero("2 registros guardados", sim.cantidad_historial, 2);
    check_entero("1.er registro es orbita", (int)sim.historial[0].tipo, REGISTRO_ORBITA);
    check_entero("2.o registro es pulso", (int)sim.historial[1].tipo, REGISTRO_PULSO);
    check("dv guardado [m/s]", sim.historial[1].velocidad_lateral, 5.0, 1e-12);
    for (i = 0; i < MAX_HISTORIAL + 5; i++) {
        sim.propulsor.aceleracion = 1.0 + i;      /* se distingue cada registro */
        calcular_pulso_lateral(&sim.res_orbita, &sim.propulsor, &sim.res_propulsor);
        historial_agregar_pulso(&sim);
    }
    check_entero("historial no supera el maximo", sim.cantidad_historial, MAX_HISTORIAL);
    check("el ultimo es el mas reciente", sim.historial[MAX_HISTORIAL - 1].aceleracion, 1.0 + MAX_HISTORIAL + 4, 1e-12);
    check("el mas antiguo se descarto", sim.historial[0].aceleracion, 1.0 + 5, 1e-12);
    historial_formatear(&sim.historial[0], 1, texto, sizeof(texto));
    check_entero("linea del historial cabe en pantalla", (int)strlen(texto) <= 60, 1);

    printf("== Reportes (deben entrar en la pantalla, tambien con valores extremos)\n");
    sim.orbita.altitud = ALTITUD_MAX;
    sim.orbita.masa_nave = MASA_NAVE_MAX;
    sim.propulsor.aceleracion = ACEL_PULSO_MAX;
    sim.propulsor.duracion = DT_PULSO_MAX;
    calcular_orbita(&sim.orbita, &sim.res_orbita);
    calcular_pulso_lateral(&sim.res_orbita, &sim.propulsor, &sim.res_propulsor);
    sim.orbita_calculada = 1;
    sim.pulso_calculado = 1;
    {
        FuncionReporte reportes[5] = { reporte_parametros, reporte_orbita, reporte_pulso,
                                       reporte_historial, reporte_resumen };
        const char *nombres[5] = { "parametros", "orbita", "pulso", "historial", "resumen" };
        char nombre[40];

        for (i = 0; i < 5; i++) {
            medida.lineas = 0;
            medida.largo_max = 0;
            reportes[i](&sim, medir_linea, &medida);
            snprintf(nombre, sizeof(nombre), "%s: lineas <= maximo", nombres[i]);
            check_entero(nombre, medida.lineas > 0 && medida.lineas <= MAX_LINEAS_REPORTE, 1);
            snprintf(nombre, sizeof(nombre), "%s: sin truncar", nombres[i]);
            check_entero(nombre, medida.largo_max < LARGO_LINEA_REPORTE - 1, 1);
        }
    }

    printf("\n%s (%d fallos)\n", fallos == 0 ? "TODAS LAS PRUEBAS PASARON" : "HAY PRUEBAS FALLIDAS", fallos);
    return fallos != 0;
}
