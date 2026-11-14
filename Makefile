# Compatible con MSYS2 (terminal UCRT64) y Linux.
CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -O2
LDLIBS  = -lm

# En Windows gcc genera archivos .exe
ifeq ($(OS),Windows_NT)
EXE = .exe
endif

# Modulos de logica y validacion (sin interfaz grafica)
LOGICA = artemis.c propulsor.c historial.c reportes.c validaciones.c
HDRS   = artemis.h propulsor.h historial.h reportes.h validaciones.h interfaz.h

# Banderas de SDL3 (MSYS2: paquete mingw-w64-ucrt-x86_64-sdl3)
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl3)

# Programa completo: main.c + interfaz.c + logica (requiere SDL 3.2.0 o mas nuevo).
# Los headers de SDL3 usan extensiones, por eso este objetivo no usa -std=c99 ni -pedantic.
all: artemis$(EXE)

artemis$(EXE): main.c interfaz.c $(LOGICA) $(HDRS)
	@pkg-config --exists sdl3 || { echo "ERROR: no se encontro SDL3. En MSYS2 UCRT64: pacman -S mingw-w64-ucrt-x86_64-sdl3"; exit 1; }
	$(CC) -Wall -Wextra -O2 -o $@ main.c interfaz.c $(LOGICA) $(SDL_FLAGS) $(LDLIBS)

# Demo por consola del propulsor (no necesita SDL)
demo_propulsor$(EXE): demo_propulsor.c $(LOGICA) $(HDRS)
	$(CC) $(CFLAGS) -o $@ demo_propulsor.c $(LOGICA) $(LDLIBS)

# Pruebas automaticas de la logica (no necesitan SDL)
test_propulsor$(EXE): test_propulsor.c $(LOGICA) $(HDRS)
	$(CC) $(CFLAGS) -o $@ test_propulsor.c $(LOGICA) $(LDLIBS)

test: test_propulsor$(EXE)
	./test_propulsor$(EXE)

demo: demo_propulsor$(EXE)
	./demo_propulsor$(EXE)

clean:
	rm -f artemis artemis.exe demo_propulsor demo_propulsor.exe \
	      test_propulsor test_propulsor.exe *.o

.PHONY: all test demo clean
