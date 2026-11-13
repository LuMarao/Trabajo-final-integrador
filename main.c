#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "artemis.h"
#include "validaciones.h"

#define ANCHO_VENTANA      800
#define ALTO_VENTANA       600
#define CANTIDAD_OPCIONES  6
#define TAM_CARACTER       8.0f
#define ESCALA_TITULO      3.0f
#define ESCALA_TEXTO       2.0f
#define ESCALA_DETALLE     1.5f
#define ALTO_LINEA         20.0f
#define MAX_LINEAS         19
#define LARGO_LINEA        72
#define MAX_CAMPOS         2
#define LARGO_CAMPO        24

#define COLOR_FONDO                18, 24, 32
#define COLOR_BOTON                38, 50, 66
#define COLOR_BOTON_SELECCIONADO   52, 110, 180
#define COLOR_BORDE                120, 160, 210
#define COLOR_TEXTO                255, 255, 255
#define COLOR_TEXTO_CELESTE        150,  190,  235
#define COLOR_TEXTO_CLARO          200, 200,  200
#define COLOR_TEXTO_SUAVE          150, 150, 150
#define COLOR_ERROR                240, 140, 140

enum {
    OPC_PARAMETROS = 1,
    OPC_ORBITA,
    OPC_PULSO,
    OPC_HISTORIAL,
    OPC_REPORTE,
    OPC_SALIR
};

typedef enum {
    PANTALLA_MENU,
    PANTALLA_FORMULARIO,
    PANTALLA_RESULTADO
} Pantalla;

typedef struct {
    Pantalla pantalla;
    int opcion;
    int seleccionado;
    int ejecutando;
} Estado;

typedef struct {
    const char *etiqueta;
    char texto[LARGO_CAMPO];
    double minimo;
    double maximo;
    double original;
} Campo;

typedef struct {
    Campo campos[MAX_CAMPOS];
    int cantidad;
    int activo;
    int reemplazar;
    char error[96];
} Formulario;

typedef struct {
    char lineas[MAX_LINEAS][LARGO_LINEA];
    int cantidad;
} Detalle;
 
typedef struct {
    DatosOrbita orbita;
    DatosPropulsor propulsor;
    ResultadosOrbita res_orbita;
    ResultadosPropulsor res_propulsor;
    int orbita_calculada;
    int pulso_calculado;
} Simulacion;

static const char *textos_menu[CANTIDAD_OPCIONES] = {
    "parametros",
    "Calcular la orbita",
    "Simular pulso lateral",
    "historial",
    "reporte final",
    "Salir"
};

static const char *textos_pregunta[2] = {
    "S - Si",
    "N - No"
};

static const char *textos_formulario[2] = {
    "Aceptar (Enter)",
    "Valores originales"
};

static SDL_FRect botones_menu[CANTIDAD_OPCIONES];
static SDL_FRect botones_pregunta[2];
static SDL_FRect areas_campos[MAX_CAMPOS];
static SDL_FRect botones_formulario[2];

static Simulacion sim;
static Formulario formulario;
static Detalle detalle;

static void armar_botones(void)
{
    int i;

    for (i = 0; i < CANTIDAD_OPCIONES; i++) {
        botones_menu[i].x = 40.0f;
        botones_menu[i].y = 150.0f + (float)i * 60.0f;
        botones_menu[i].w = 720.0f;
        botones_menu[i].h = 48.0f;
    }
    for (i = 0; i < 2; i++) {
        botones_pregunta[i].x = 40.0f + (float)i * 370.0f;
        botones_pregunta[i].y = 530.0f;
        botones_pregunta[i].w = 350.0f;
        botones_pregunta[i].h = 48.0f;

        botones_formulario[i].x = 40.0f + (float)i * 370.0f;
        botones_formulario[i].y = 390.0f;
        botones_formulario[i].w = 350.0f;
        botones_formulario[i].h = 48.0f;
    }
    for (i = 0; i < MAX_CAMPOS; i++) {
        areas_campos[i].x = 40.0f;
        areas_campos[i].y = 140.0f + (float)i * 100.0f;
        areas_campos[i].w = 720.0f;
        areas_campos[i].h = 44.0f;
    }
}

static float ancho_texto(const char *texto, float escala)
{
    return (float)strlen(texto) * TAM_CARACTER * escala;
}

static void dibujar_texto(SDL_Renderer *renderizador, float x, float y, float escala,
                                                                                      const char *texto)
{
    SDL_SetRenderScale(renderizador, escala, escala);
    SDL_RenderDebugText(renderizador, x / escala, y / escala, texto);
    SDL_SetRenderScale(renderizador, 1.0f, 1.0f);
}

static void dibujar_texto_centrado_x(SDL_Renderer *renderizador, float y, float escala,
                                     const char *texto)
{
    float x = ((float)ANCHO_VENTANA - ancho_texto(texto, escala)) / 2.0f;

    dibujar_texto(renderizador, x, y, escala, texto);
}

static void dibujar_franja_paleta(SDL_Renderer *renderizador)
{
    static const unsigned char colores[4][3] = {
        { 38, 50, 66 },
        { 52, 110, 180 },
        { 120, 160, 210 },
        { 150, 190, 235 }
    };
    SDL_FRect area;
    int i;

    area.y = 0.0f;
    area.w = (float)ANCHO_VENTANA / 4.0f;
    area.h = 12.0f;
    for (i = 0; i < 4; i++) {
        area.x = (float)i * area.w;
        SDL_SetRenderDrawColor(renderizador, colores[i][0], colores[i][1], colores[i][2], 255);
        SDL_RenderFillRect(renderizador, &area);
    }
}

static void dibujar_marco(SDL_Renderer *renderizador, const SDL_FRect *area, int resaltado)
{
    SDL_FRect interior;
 
    SDL_SetRenderDrawColor(renderizador, COLOR_BORDE, 255);
    SDL_RenderRect(renderizador, area);
    if (resaltado) {
        interior.x = area->x + 1.0f;
        interior.y = area->y + 1.0f;
        interior.w = area->w - 2.0f;
        interior.h = area->h - 2.0f;
        SDL_RenderRect(renderizador, &interior);
    }
}
static void dibujar_boton(SDL_Renderer *renderizador, const SDL_FRect *area,
                                                                             const char *texto, int seleccionado)
{
    float x = area->x + (area->w - ancho_texto(texto, ESCALA_TEXTO)) / 2.0f;
    float y = area->y + (area->h - TAM_CARACTER * ESCALA_TEXTO) / 2.0f;
    SDL_FRect interior;

    if (seleccionado) {
        SDL_SetRenderDrawColor(renderizador, COLOR_BOTON_SELECCIONADO, 255);
    } else {
        SDL_SetRenderDrawColor(renderizador, COLOR_BOTON, 255);
    }
    SDL_RenderFillRect(renderizador, area);
    dibujar_marco(renderizador, area, seleccionado);
   
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO, 255);
    dibujar_texto(renderizador, x, y, ESCALA_TEXTO, texto);
}

static void dibujar_campo(SDL_Renderer *renderizador, const SDL_FRect *area,
                                                                             const char *texto, int activo)
{
    char visible[LARGO_CAMPO + 2];
    float y = area->y + (area->h - TAM_CARACTER * ESCALA_TEXTO) / 2.0f;
 
    if (activo) {
        SDL_SetRenderDrawColor(renderizador, COLOR_BOTON_SELECCIONADO, 255);
    } else {
        SDL_SetRenderDrawColor(renderizador, COLOR_BOTON, 255);
    }
    SDL_RenderFillRect(renderizador, area);
    dibujar_marco(renderizador, area, activo);
 
    snprintf(visible, sizeof(visible), "%s%s", texto,
             (activo && ((unsigned long long)SDL_GetTicks() / 500ULL) % 2ULL == 0ULL) ? "_" : "");
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO, 255);
    dibujar_texto(renderizador, area->x + 14.0f, y, ESCALA_TEXTO, visible);
}

static int indice_boton(const SDL_FRect *botones, int cantidad, float x, float y)
{
    int i;

    for (i = 0; i < cantidad; i++) {
        if (x >= botones[i].x && x < botones[i].x + botones[i].w &&
            y >= botones[i].y && y < botones[i].y + botones[i].h) {
            return i;
        }
    }
    return -1;
}

static void linea(Detalle *d, const char *formato, ...)
{
    va_list argumentos;
 
    if (d->cantidad >= MAX_LINEAS) {
        return;
    }
    va_start(argumentos, formato);
    vsnprintf(d->lineas[d->cantidad], LARGO_LINEA, formato, argumentos);
    va_end(argumentos);
    d->cantidad++;
}
 
static void armar_datos(Detalle *d, const Simulacion *s)
{
    linea(d, "Datos actualizados. Use la opcion de calcular la orbita");
    linea(d, "para obtener los resultados con estos valores.");
    linea(d, "");
    linea(d, "  Masa de la Tierra  MT = %.3e kg", s->orbita.masa_tierra);
    linea(d, "  Radio terrestre    RT = %.0f m", s->orbita.radio_tierra);
    linea(d, "  Altitud orbital    h  = %.0f m", s->orbita.altitud);
    linea(d, "  Masa de Orion      m  = %.2f kg", s->orbita.masa_nave);
    linea(d, "  Pulso lateral      a  = %.3f m/s^2, dt = %.2f s",
          s->propulsor.aceleracion, s->propulsor.duracion);
}
 
static void armar_orbita(Detalle *d, const Simulacion *s)
{
    const ResultadosOrbita *r = &s->res_orbita;
    double fc = s->orbita.masa_nave * r->velocidad_orbital * r->velocidad_orbital
                / r->radio_orbital;
    double diferencia = (r->fuerza_gravitatoria != 0.0)
                        ? (r->fuerza_gravitatoria > fc ? r->fuerza_gravitatoria - fc
                                                       : fc - r->fuerza_gravitatoria)
                          / r->fuerza_gravitatoria
                        : 0.0;
 
    linea(d, "h = %.0f m | m = %.2f kg | r = RT + h = %.0f m",
          s->orbita.altitud, s->orbita.masa_nave, r->radio_orbital);
    linea(d, "");
    linea(d, "A - La gravedad actua como fuerza centripeta (Fg = Fc):");
    linea(d, "    Fg = G*MT*m / r^2     = %.2f N", r->fuerza_gravitatoria);
    linea(d, "    Fc = m*v^2 / r        = %.2f N", fc);
    linea(d, "    v  = sqrt(G*MT / r)   = %.2f m/s (%.0f km/h)",
          r->velocidad_orbital, r->velocidad_orbital * 3.6);
    linea(d, "    Diferencia relativa |Fg - Fc| / Fg = %.1e", diferencia);
    if (diferencia < 1e-9) {
        linea(d, "    => Fg y Fc coinciden; la masa de la nave se cancela.");
    }
    linea(d, "");
    linea(d, "B - Periodo orbital:  T = 2*pi*r / v");
    linea(d, "    T = %.2f s = %.2f min", r->periodo_s, r->periodo_min);
    linea(d, "");
    linea(d, "C - Aceleracion centripeta:  ac = v^2 / r");
    linea(d, "    ac = %.3f m/s^2", r->aceleracion_centripeta);
    linea(d, "    (igual a la gravedad g = G*MT/r^2 a esta altura)");
}
 
static void armar_pulso(Detalle *d, const Simulacion *s)
{
    const ResultadosPropulsor *p = &s->res_propulsor;
 
    linea(d, "Pulso: a = %.3f m/s^2 durante dt = %.1f s",
          s->propulsor.aceleracion, s->propulsor.duracion);
    linea(d, "Velocidad orbital antes del pulso: v = %.2f m/s",
          s->res_orbita.velocidad_orbital);
    linea(d, "");
    linea(d, "1) Velocidad lateral:  dv = a*dt = %.3f m/s", p->velocidad_lateral);
    linea(d, "2) Desviacion lateral: d = 1/2*a*dt^2 = %.3f m", p->desviacion_lateral);
    linea(d, "3) Nueva rapidez: v' = sqrt(v^2 + dv^2) = %.4f m/s", p->velocidad_nueva);
    linea(d, "4) Giro del plano orbital: atan(dv/v) = %.5f grados", p->giro_grados);
    linea(d, "");
    linea(d, "Trayectoria: la nave sigue en orbita (la gravedad sigue");
    linea(d, "actuando como fuerza centripeta). La velocidad lateral se");
    linea(d, "suma a la orbital: el plano de la orbita se inclina %.5f", p->giro_grados);
    linea(d, "grados y deja de ser perfectamente circular");
    linea(d, "(excentricidad %.3e; el apoapsis sube %.2f m).",
          p->excentricidad, p->aumento_apoapsis);
    linea(d, "Un cuarto de vuelta despues, la nave queda a ~%.0f m de", p->separacion_maxima);
    linea(d, "su orbita original.");
    linea(d, "(Se supone un pulso lateral, perpendicular a la velocidad.)");
}
 
static void armar_reporte(Detalle *d, const Simulacion *s)
{
    if (!s->orbita_calculada && !s->pulso_calculado) {
        linea(d, "Todavia no hay resultados.");
        linea(d, "Calcule la orbita y simule el pulso lateral primero.");
        return;
    }
    if (s->orbita_calculada) {
        linea(d, "ORBITA: h = %.0f m, m = %.2f kg", s->orbita.altitud, s->orbita.masa_nave);
        linea(d, "  Fuerza gravitatoria   Fg = %.2f N", s->res_orbita.fuerza_gravitatoria);
        linea(d, "  Velocidad orbital     v  = %.2f m/s", s->res_orbita.velocidad_orbital);
        linea(d, "  Periodo               T  = %.2f s = %.2f min",
              s->res_orbita.periodo_s, s->res_orbita.periodo_min);
        linea(d, "  Aceleracion centripeta ac = %.3f m/s^2",
              s->res_orbita.aceleracion_centripeta);
    }
    if (s->pulso_calculado) {
        linea(d, "");
        linea(d, "PULSO LATERAL: a = %.3f m/s^2, dt = %.1f s",
              s->propulsor.aceleracion, s->propulsor.duracion);
        linea(d, "  Velocidad lateral     dv = %.3f m/s", s->res_propulsor.velocidad_lateral);
        linea(d, "  Desviacion lateral    d  = %.3f m", s->res_propulsor.desviacion_lateral);
        linea(d, "  Nueva rapidez         v' = %.4f m/s", s->res_propulsor.velocidad_nueva);
        linea(d, "  Giro del plano orbital    = %.5f grados", s->res_propulsor.giro_grados);
    }
}
 
static void preparar_campo(Campo *campo, const char *etiqueta, double minimo, double maximo,
                           double original, double actual)
{
    campo->etiqueta = etiqueta;
    campo->minimo = minimo;
    campo->maximo = maximo;
    campo->original = original;
    snprintf(campo->texto, LARGO_CAMPO, "%.10g", actual);
}
 
static void abrir_formulario(int opcion)
{
    formulario.activo = 0;
    formulario.reemplazar = 1;
    formulario.error[0] = '\0';
    formulario.cantidad = 2;
 
    if (opcion == OPC_PARAMETROS) {
        preparar_campo(&formulario.campos[0], "Altitud orbital h en metros",
                       ALTITUD_MIN, ALTITUD_MAX, ALTITUD_DEF, sim.orbita.altitud);
        preparar_campo(&formulario.campos[1], "Masa de la nave m en kg",
                       MASA_NAVE_MIN, MASA_NAVE_MAX, MASA_ORION_DEF, sim.orbita.masa_nave);
    } else {
        preparar_campo(&formulario.campos[0], "Aceleracion a en m/s^2",
                       ACEL_PULSO_MIN, ACEL_PULSO_MAX, ACEL_PULSO_DEF, sim.propulsor.aceleracion);
        preparar_campo(&formulario.campos[1], "Duracion dt en segundos",
                       DT_PULSO_MIN, DT_PULSO_MAX, DT_PULSO_DEF, sim.propulsor.duracion);
    }
}
 
static void seleccionar_campo(int indice)
{
    formulario.activo = indice;
    formulario.reemplazar = 1;
}
 
static void restaurar_originales(void)
{
    int i;
 
    for (i = 0; i < formulario.cantidad; i++) {
        snprintf(formulario.campos[i].texto, LARGO_CAMPO, "%.10g", formulario.campos[i].original);
    }
    formulario.error[0] = '\0';
    formulario.reemplazar = 1;
}
 
static void mostrar_resultado(Estado *estado)
{
    estado->pantalla = PANTALLA_RESULTADO;
    estado->seleccionado = 0;
}
 
static void confirmar_formulario(Estado *estado)
{
    double valores[MAX_CAMPOS];
    int i;
    int resultado;
 
    for (i = 0; i < formulario.cantidad; i++) {
        resultado = convertir_real(formulario.campos[i].texto, formulario.campos[i].minimo,
                                   formulario.campos[i].maximo, &valores[i]);
        if (resultado == VALIDACION_FORMATO) {
            snprintf(formulario.error, sizeof(formulario.error),
                     "Campo %d: ingrese un numero valido.", i + 1);
            formulario.activo = i;
            formulario.reemplazar = 1;
            return;
        }
        if (resultado == VALIDACION_RANGO) {
            snprintf(formulario.error, sizeof(formulario.error),
                     "Campo %d: debe estar entre %.10g y %.10g.", i + 1,
                     formulario.campos[i].minimo, formulario.campos[i].maximo);
            formulario.activo = i;
            formulario.reemplazar = 1;
            return;
        }
    }
 
    detalle.cantidad = 0;
    if (estado->opcion == OPC_PARAMETROS) {
        sim.orbita.altitud = valores[0];
        sim.orbita.masa_nave = valores[1];
        sim.orbita_calculada = 0;
        sim.pulso_calculado = 0;
        armar_datos(&detalle, &sim);
    } else {
        sim.propulsor.aceleracion = valores[0];
        sim.propulsor.duracion = valores[1];
        calcular_pulso_lateral(&sim.res_orbita, &sim.propulsor, &sim.res_propulsor);
        sim.pulso_calculado = 1;
        armar_pulso(&detalle, &sim);
    }
    mostrar_resultado(estado);
}
 
static void iniciar_accion(Estado *estado)
{
    detalle.cantidad = 0;
 
    switch (estado->opcion) {
        case OPC_PARAMETROS:
            abrir_formulario(OPC_PARAMETROS);
            estado->pantalla = PANTALLA_FORMULARIO;
            break;
        case OPC_ORBITA:
            calcular_orbita(&sim.orbita, &sim.res_orbita);
            sim.orbita_calculada = 1;
            sim.pulso_calculado = 0;
            armar_orbita(&detalle, &sim);
            mostrar_resultado(estado);
            break;
        case OPC_PULSO:
            if (!sim.orbita_calculada) {
                linea(&detalle, "Primero calcule la orbita con los datos actuales.");
                mostrar_resultado(estado);
            } else {
                abrir_formulario(OPC_PULSO);
                estado->pantalla = PANTALLA_FORMULARIO;
            }
            break;
        case OPC_HISTORIAL:
            linea(&detalle, "El historial todavia no esta disponible.");
            linea(&detalle, "Ningun modulo lo implementa por ahora.");
            mostrar_resultado(estado);
            break;
        case OPC_REPORTE:
            armar_reporte(&detalle, &sim);
            mostrar_resultado(estado);
            break;
        default:
            break;
    }
}

static void activar(Estado *estado, int indice)
{
    if (estado->pantalla == PANTALLA_MENU) {
        estado->opcion = indice + 1;
        if (estado->opcion == OPC_SALIR) {
            estado->ejecutando = 0;
            return;
        }
        iniciar_accion(estado);
    } else if (estado->pantalla == PANTALLA_RESULTADO) {
        if (indice == 0) {
            estado->pantalla = PANTALLA_MENU;
            estado->seleccionado = estado->opcion - 1;
        } else {
            iniciar_accion(estado);
        }
    }
}

static char caracter_tecla(SDL_Keycode tecla)
{
if (tecla >= SDLK_0 && tecla <= SDLK_9) {
        return (char)('0' + (tecla - SDLK_0));
    }
    if (tecla >= SDLK_KP_1 && tecla <= SDLK_KP_9) {
        return (char)('1' + (tecla - SDLK_KP_1));
    }
    switch (tecla) {
        case SDLK_KP_0:      return '0';
        case SDLK_PERIOD:
        case SDLK_KP_PERIOD: return '.';
        case SDLK_COMMA:     return ',';
        case SDLK_MINUS:
        case SDLK_KP_MINUS:  return '-';
        case SDLK_E:         return 'e';
        default:             return '\0';
    }
}

static void escribir_en_campo(Campo *campo, char caracter)
{
    size_t largo;
 
    if (formulario.reemplazar) {
        campo->texto[0] = '\0';
        formulario.reemplazar = 0;
    }
    largo = strlen(campo->texto);
    if (largo + 1 < LARGO_CAMPO) {
        campo->texto[largo] = caracter;
        campo->texto[largo + 1] = '\0';
    }
    formulario.error[0] = '\0';
}
 
static void borrar_en_campo(Campo *campo)
{
    size_t largo = strlen(campo->texto);
 
    if (largo > 0) {
        campo->texto[largo - 1] = '\0';
    }
    formulario.reemplazar = 0;
    formulario.error[0] = '\0';
}
 
static void manejar_tecla_formulario(Estado *estado, SDL_Keycode tecla)
{
    char caracter;
 
    switch (tecla) {
        case SDLK_ESCAPE:
            estado->pantalla = PANTALLA_MENU;
            estado->seleccionado = estado->opcion - 1;
            return;
        case SDLK_TAB:
        case SDLK_DOWN:
            seleccionar_campo((formulario.activo + 1) % formulario.cantidad);
            return;
        case SDLK_UP:
            seleccionar_campo((formulario.activo + formulario.cantidad - 1) % formulario.cantidad);
            return;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            if (formulario.activo < formulario.cantidad - 1) {
                seleccionar_campo(formulario.activo + 1);
            } else {
                confirmar_formulario(estado);
            }
            return;
        case SDLK_BACKSPACE:
            borrar_en_campo(&formulario.campos[formulario.activo]);
            return;
        default:
            break;
    }
 
    caracter = caracter_de_tecla(tecla);
    if (caracter != '\0') {
        escribir_en_campo(&formulario.campos[formulario.activo], caracter);
    }
}
 
static void manejar_tecla(Estado *estado, SDL_Keycode tecla, int repetida)
{
    int cantidad;
 
    if (estado->pantalla == PANTALLA_FORMULARIO) {
        if (!repetida || tecla == SDLK_BACKSPACE) {
            manejar_tecla_formulario(estado, tecla);
        }
        return;
    }
    if (repetida) {
        return;
    }
 
    cantidad = (estado->pantalla == PANTALLA_MENU) ? CANTIDAD_OPCIONES : 2;
 
    switch (tecla) {
        case SDLK_UP:
        case SDLK_LEFT:
            estado->seleccionado = (estado->seleccionado + cantidad - 1) % cantidad;
            return;
        case SDLK_DOWN:
        case SDLK_RIGHT:
            estado->seleccionado = (estado->seleccionado + 1) % cantidad;
            return;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            activar(estado, estado->seleccionado);
            return;
        default:
            break;
    }
 
    if (estado->pantalla == PANTALLA_MENU) {
        if (tecla >= SDLK_1 && tecla <= SDLK_6) {
            activar(estado, (int)(tecla - SDLK_1));
        } else if (tecla == SDLK_ESCAPE) {
            estado->ejecutando = 0;
        }
    } else {
        if (tecla == SDLK_S) {
            activar(estado, 0);
        } else if (tecla == SDLK_N) {
            activar(estado, 1);
        }
    }
}
 
static void manejar_clic(Estado *estado, float x, float y)
{
    int indice;

   if (estado->pantalla == PANTALLA_MENU) {
        indice = indice_boton(botones_menu, CANTIDAD_OPCIONES, x, y);
        if (indice >= 0) {
            activar(estado, indice);
        }
    } else if (estado->pantalla == PANTALLA_RESULTADO) {
        indice = indice_boton(botones_pregunta, 2, x, y);
        if (indice >= 0) {
            activar(estado, indice);
        }
    } else {
        indice = indice_boton(areas_campos, formulario.cantidad, x, y);
        if (indice >= 0) {
            seleccionar_campo(indice);
            return;
        }
        indice = indice_boton(botones_formulario, 2, x, y);
        if (indice == 0) {
            confirmar_formulario(estado);
        } else if (indice == 1) {
            restaurar_originales();
        }
    }
}
 
static void manejar_movimiento(Estado *estado, float x, float y)
{
    int indice = -1;
 
    if (estado->pantalla == PANTALLA_MENU) {
        indice = indice_boton(botones_menu, CANTIDAD_OPCIONES, x, y);
    } else if (estado->pantalla == PANTALLA_RESULTADO) {
        indice = indice_boton(botones_pregunta, 2, x, y);
    }
    if (indice >= 0) {
        estado->seleccionado = indice;
    }
}

static void dibujar_menu(SDL_Renderer *renderizador, const Estado *estado)
{
    int i;

    dibujar_franja_paleta(renderizador);

    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO, 255);
    dibujar_texto_centrado_x(renderizador, 36.0f, ESCALA_TITULO, "SIMULADOR");
SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO_CELESTE, 255);
    dibujar_texto_centrado_x(renderizador, 86.0f, ESCALA_TEXTO,
                             "ORION EN ORBITA TERRESTRE BAJA (LEO)");
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO_CLARO, 255);
    dibujar_texto_centrado_x(renderizador, 120.0f, ESCALA_TEXTO, "MENU PRINCIPAL");

    for (i = 0; i < CANTIDAD_OPCIONES; i++) {
        dibujar_boton(renderizador, &botones_menu[i], textos_menu[i], i == estado->seleccionado);
    }

    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO_SUAVE, 255);
    dibujar_texto_centrado_x(renderizador, 540.0f, ESCALA_TEXTO, "Teclas 1-6, flechas + Enter o mouse");
}

static void dibujar_formulario(SDL_Renderer *renderizador, const Estado *estado)
{
    char etiqueta[96];
    int i;

    dibujar_franja_paleta(renderizador);

    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO, 255);
    dibujar_texto_centrado_x(renderizador, 40.0f, ESCALA_TITULO, textos_menu [estado->opcion - 1]);

    for (i = 0; i < formulario.cantidad; i++) {
        snprintf(etiqueta, sizeof(etiqueta), "%s (%.10g - %.10g)", formulario.campos[i].etiqueta,
                 formulario.campos[i].minimo, formulario.campos[i].maximo);
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO_CELESTE, 255);
dibujar_texto(renderizador, areas_campos[i].x, areas_campos[i].y - 28.0f, ESCALA_TEXTO, etiqueta);
        dibujar_campo(renderizador, &areas_campos[i], formulario.campos[i].texto,
                      i == formulario.activo);
    }

if (formulario.error[0] != '\0') {
        SDL_SetRenderDrawColor(renderizador, COLOR_ERROR, 255);
        dibujar_texto(renderizador, 40.0f, 330.0f, ESCALA_TEXTO, formulario.error);
    }
 
    for (i = 0; i < 2; i++) {
        dibujar_boton(renderizador, &botones_formulario[i], textos_formulario[i], 0);
    }
 
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO_SUAVE, 255);
    dibujar_texto_centrado_x(renderizador, 470.0f, ESCALA_TEXTO, "Tab: cambiar de campo | Esc: cancelar");
}
    
    static void dibujar_resultado(SDL_Renderer *renderizador, const Estado *estado)
{
    int i;
 
    dibujar_franja_paleta(renderizador);
 
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO, 255);
    dibujar_texto_centrado_x(renderizador, 36.0f, ESCALA_TITULO, textos_menu[estado->opcion - 1]);
 
    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO_CLARO, 255);
    for (i = 0; i < detalle.cantidad; i++) {
        dibujar_texto(renderizador, 40.0f, 96.0f + (float)i * ALTO_LINEA, ESCALA_DETALLE,
                      detalle.lineas[i]);
    }

    SDL_SetRenderDrawColor(renderizador, COLOR_TEXTO, 255);
    dibujar_texto_centrado_x(renderizador, 492.0f, ESCALA_TEXTO, "Volver al menu principal?");
 
    for (i = 0; i < 2; i++) {
        dibujar_boton(renderizador, &botones_pregunta[i], textos_pregunta[i],
                      i == estado->seleccionado);
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *ventana = NULL;
    SDL_Renderer *renderizador = NULL;
    SDL_Event evento;
    Estado estado;

    (void)argc;
    (void)argv;

    cargar_valores_originales(&sim.orbita, &sim.propulsor);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "No se pudo iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (!SDL_CreateWindowAndRenderer("Simulador - Orion en LEO",
                                     ANCHO_VENTANA, ALTO_VENTANA, 0,
                                     &ventana, &renderizador)) {
        fprintf(stderr, "No se pudo crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    armar_botones();
    estado.pantalla = PANTALLA_MENU;
    estado.opcion = 0;
    estado.seleccionado = 0;
    estado.ejecutando = 1;

    while (estado.ejecutando) {
        while (SDL_PollEvent(&evento)) {
            switch (evento.type) {
                case SDL_EVENT_QUIT:
                    estado.ejecutando = 0;
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    manejar_movimiento(&estado, evento.motion.x, evento.motion.y);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (evento.button.button == SDL_BUTTON_LEFT) {
                        manejar_clic(&estado, evento.button.x, evento.button.y);
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                        manejar_tecla(&estado, evento.key.key,evento.key.repeat);
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderizador, COLOR_FONDO, 255);
        SDL_RenderClear(renderizador);
        if (estado.pantalla == PANTALLA_MENU) {
            dibujar_menu(renderizador, &estado);
        } else if (estado.pantalla == PANTALLA_FORMULARIO) {
            dibujar_formulario(renderizador, &estado);
        } else {
            dibujar_resultado(renderizador, &estado);
        }
        SDL_RenderPresent(renderizador);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
    return 0;
}
