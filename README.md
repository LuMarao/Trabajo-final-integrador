# Simulador de Misiones Artemis – Escenario 2: Orion en órbita baja terrestre (LEO)

## Descripción del proyecto

Este proyecto consiste en desarrollar un programa en lenguaje C que permita simular algunos aspectos físicos de la nave Orion mientras se encuentra en una órbita baja alrededor de la Tierra (LEO).

El programa permite realizar cálculos relacionados con la órbita de la nave y simular el efecto de un pulso de propulsión lateral. Los datos iniciales pueden ser modificados por el usuario para realizar diferentes pruebas.

El trabajo corresponde a la materia Algoritmos y Estructuras de Datos de la carrera Ingeniería en Sistemas de Información de la UTN Facultad Regional Resistencia.

## Integrantes

- Luciana Noeli Marao Duarte – Menú e integración.
- Uriel Vallejos – Cálculos de física orbital.
- Luciano Medina Saavedra – Propulsor lateral.
- Alegre Sanchez Ruben Bautista – Validaciones.
- Aranda Silvero Maximiliano Uriel – Estructuras, documentación y pruebas.

**Comisión:** K 1.2

## Escenario de trabajo

La simulación se basa en una nave Orion que, luego de apagar sus motores de ascenso, permanece en una órbita circular alrededor de la Tierra.

Para realizar los cálculos se utilizan los siguientes datos iniciales:

- Masa de la Tierra: 5,972 × 10²⁴ kg.
- Radio de la Tierra: 6.371.000 m.
- Altitud de la órbita: 192.000 m.
- Radio orbital: 6.563.000 m.
- Constante gravitatoria: 6,674 × 10⁻¹¹ N·m²/kg².
- Masa de Orion: 33.000 kg.
- Aceleración del pulso lateral: 0,50 m/s².
- Duración del pulso: 10 segundos.

## Cálculos que realiza el programa

El programa contempla los siguientes cálculos:

- Fuerza gravitatoria y velocidad orbital de la nave.
- Período orbital, expresado en segundos y minutos.
- Aceleración centrípeta.
- Velocidad lateral y desviación producida por el pulso del propulsor.

Para el pulso lateral se considera una aceleración constante durante el tiempo indicado.

Como referencia, utilizando los datos iniciales se obtienen aproximadamente los siguientes resultados:

- Fuerza gravitatoria: 305.362 N.
- Velocidad orbital: 7.792,95 m/s.
- Período orbital: 5.291,52 segundos (88,19 minutos).
- Aceleración centrípeta: 9,253 m/s².
- Velocidad lateral: 5 m/s.
- Desviación lateral: 25 m.

## Rangos de los datos del pulso lateral

| Dato            | Mínimo   | Máximo  | Valor de la consigna |
| Aceleración `a` | 0,01 m/s²| 10 m/s² | 0,50 m/s² |
| Duración `dt`   | 0,1 s    | 60 s    | 10 s |

- Los mínimos son mayores que 0, porque con `a = 0` o `dt = 0` no hay pulso que simular.
- El máximo de `dt` (60 s) es cerca del 1 % del período orbital (88 min). Así el pulso es corto y se lo puede tratar como instantáneo. Con los máximos (`a = 10 m/s²`, `dt = 60 s`) la velocidad lateral es de 600 m/s, menos del 8 % de la velocidad orbital, y la nave sigue en una órbita baja casi circular.
- Los rangos están definidos en un solo lugar (`artemis.h`: `ACEL_PULSO_MIN/MAX` y `DT_PULSO_MIN/MAX`). La ventana, la consola y las pruebas los toman de ahí.

## Funcionamiento del programa

Al iniciar, el programa carga los valores iniciales del escenario y muestra un menú con las operaciones disponibles.

Desde el menú, el usuario podrá consultar o modificar los datos, realizar los cálculos de la órbita, simular el pulso lateral o salir del programa.

Los datos ingresados serán validados para evitar valores incorrectos. Luego de cada operación se mostrarán los resultados correspondientes y se podrá volver al menú principal.

## Organización de los archivos

El proyecto se encuentra organizado en módulos para separar la interfaz, los cálculos físicos, las validaciones y el almacenamiento de resultados.

- `main.c`: punto de entrada del programa. Inicializa la simulación y ejecuta la interfaz.
- `artemis.c` y `artemis.h`: contienen los cálculos de mecánica orbital, constantes físicas, estructuras y declaraciones compartidas.
- `interfaz.c` y `interfaz.h`: implementan la interfaz gráfica mediante SDL3, incluyendo el menú, formularios y navegación.
- `propulsor.c` y `propulsor.h`: contienen los cálculos y operaciones del propulsor lateral.
- `validaciones.c` y `validaciones.h`: se encargan de validar los datos ingresados por el usuario.
- `historial.c` y `historial.h`: administran el historial de cálculos realizados durante la simulación.
- `reportes.c` y `reportes.h`: generan los reportes de parámetros, resultados orbitales, pulsos y resúmenes.
- `demo_propulsor.c`: permite ejecutar una demostración independiente del propulsor lateral desde la consola.
- `test_propulsor.c`: contiene pruebas automáticas para verificar los cálculos, validaciones, historial y reportes.
- `Makefile`: automatiza la compilación, ejecución de pruebas y demostraciones.
- `.gitignore`: especifica los archivos generados que Git debe ignorar.

## Compilación y ejecución

Una vez que estén integrados los archivos del proyecto, se podrá compilar desde una terminal utilizando GCC o desde el propio archivo creado por el MAKEFILE llamado "artemis.exe".

El comando previsto es:

```bash
gcc -Wall -Wextra -std=c99 main.c artemis.c propulsor.c validaciones.c -o artemis -lm
```

Para ejecutar el programa:

```bash
./artemis
```

La opción `-lm` permite utilizar las funciones matemáticas de la biblioteca correspondiente.

## Uso de inteligencia artificial

Durante el desarrollo del proyecto utilizamos ChatGPT y Claude como herramientas de apoyo.

En primer lugar, utilizamos ChatGPT para elaborar el prompt que luego se le proporcionó a Claude para trabajar en la parte de programación del proyecto.

También recurrimos a ambas herramientas para ayudarnos a comprender el código, obtener explicaciones sobre su funcionamiento, entender la estructura de cada archivo y conocer cómo se relacionan los distintos módulos entre sí.

Por último, utilizamos la IA como apoyo para analizar y solucionar distintos errores que fueron surgiendo durante la integración y conexión de los archivos del programa.

## Estos fueron algunos de los prompts más relevantes que se utilizó con la IA para la realización y apoyo del trabajo:

-Me podes dar valores reales para probar en el programa? y dame casos de prueba para probar que ande todo bien y que casos de prueba que tengan error y que errores se esperan.

-En la parte de pulso general, la aceleracion si puede ser 0? porque me deja ingresar con valor 0, y el tiempo lo mismo, tambien puede ser 0 no? porque tambien me deja.

-Necesito que me ayudes con esta parte que son las estructuras, documentación y pruebas Coordinar structs/prototipos con el equipo, README, casos de prueba y apoyo a integración. artemis.h y plan de pruebas necesitaria que me hagas el prompt para el claude.

-Actuá como asistente especializado en programación en C. Estoy desarrollando un Trabajo Práctico Integrador de Algoritmos y Estructuras de Datos de la UTN: un simulador por consola de la misión Artemis, escenario 2: Orion en órbita baja terrestre (LEO).

Te adjunto la consigna oficial y el documento de organización del equipo. Soy el integrante 5 y tengo a cargo las estructuras, la documentación y las pruebas.

Necesito que revises los documentos y los archivos disponibles del proyecto y prepares lo siguiente:

### 1. artemis.h
- Definir las estructuras `struct` necesarias.
- Declarar los prototipos de las funciones compartidas.
- Incluir las protecciones necesarias (`#ifndef`, `#define`, `#endif`).
- Coordinar los nombres y parámetros con los archivos existentes.
- Evitar duplicaciones o conflictos con el código de mis compañeros.

### 2. README.md
Crear la documentación del proyecto incluyendo:
- Descripción del simulador y del escenario.
- Integrantes y comisión (dejar espacios si faltan datos).
- Reglas y funcionamiento.
- Descripción de los módulos.
- Instrucciones de compilación y ejecución.
- Registro del uso de IA, herramientas, consultas y validación del código.

### 3. Diagrama
Crear un diagrama Mermaid que represente el flujo del programa: inicio, menú, validaciones, cálculos orbitales, propulsión lateral, resultados y salida.

### 4. Plan de pruebas
Crear `PLAN_DE_PRUEBAS.md` con casos de prueba, entradas, resultados esperados y estados pendientes de comprobar. Incluir cálculos orbitales, propulsión lateral, entradas correctas e incorrectas, validaciones y navegación del menú.

### 5. Conceptos de C
Revisá que el proyecto utilice y aplique correctamente los siguientes conceptos de la materia:
- Arreglos.
- Listas.
- Recursividad.
- Punteros.
- Estructuras (`struct`).
- Funciones y modularización.

Explicá dónde aparecen estos conceptos en el proyecto. Si alguno todavía no está implementado, indicá cómo podría incorporarse de manera coherente, sin agregar código innecesario ni inventar funcionalidades.

### 6. Integración y Git
Indicá cómo se relacionan los módulos, qué archivos comparten estructuras y funciones, y cómo compilar el proyecto completo. Revisá también qué cambios corresponden a mi parte y proponé commits descriptivos. No modifiques archivos de mis compañeros ni realices commits sin mi autorización.

### IMPORTANTE
- No inventes funciones, integrantes, resultados ni funcionalidades.
- Utilizá los valores y requisitos de los documentos adjuntos.
- Si faltan archivos o información, indicá qué necesitás.
- No des por definitivas las estructuras y los prototipos sin coordinarlos con el equipo.
- Priorizá código sencillo, ordenado y adecuado a primer año de Ingeniería.
- Explicame brevemente cada archivo generado, cómo funciona y qué debería saber defender en el coloquio.

Primero analizá los documentos y el código existente; después prepará los entregables y verificá que sean coherentes con el proyecto real.
