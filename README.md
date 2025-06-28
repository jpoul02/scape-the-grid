

# ———————————————————————————
# 1. Configuración de compilador
# ———————————————————————————
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -I$(SFML_INCLUDE) -I./src

# ———————————————————————————
# 2. Rutas de SFML (modifica si lo tienes en otro sitio)
# ———————————————————————————
SFML_HOME    ?= /usr
SFML_INCLUDE := $(SFML_HOME)/include
SFML_LIB     := $(SFML_HOME)/lib

# ———————————————————————————
# 3. Librerías a enlazar
# ———————————————————————————
LIBS := -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system

# ———————————————————————————
# 4. Fuentes y objetos
# ———————————————————————————
SRC := main.cpp \
       src/game/Game.cpp \
       src/game/Player.cpp \
       src/grid/Grid.cpp

OBJ := $(SRC:.cpp=.o)

# ———————————————————————————
# 5. Ejecutable final
# ———————————————————————————
TARGET := escape-the-grid

# ———————————————————————————
# 6. Targets “virtuales”
# ———————————————————————————
.PHONY: all install-deps clean run help describe

# Target por defecto
all: $(TARGET)

# Enlaza objetos y genera el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Regla genérica para compilar cada .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ———————————————————————————
# Instalación automática de dependencias
# (Linux / macOS / Windows instrucciones)
# ———————————————————————————
install-deps:
	@echo "Instalando SFML y dependencias..."
	@UNAME_S=$$(uname -s); \
	if [ "$$UNAME_S" = "Linux" ]; then \
	  sudo apt-get update && sudo apt-get install -y libsfml-dev; \
	elif [ "$$UNAME_S" = "Darwin" ]; then \
	  brew update && brew install sfml; \
	else \
	  echo "Windows: clona vcpkg y ejecuta:"; \
	  echo "  git clone https://github.com/microsoft/vcpkg.git"; \
	  echo "  ./vcpkg/bootstrap-vcpkg.sh"; \
	  echo "  ./vcpkg/vcpkg install sfml:x64-windows"; \
	fi

# ———————————————————————————
# Limpieza de binarios y objetos
# ———————————————————————————
clean:
	rm -f $(OBJ) $(TARGET)

# ———————————————————————————
# Ejecutar el juego
# ———————————————————————————
run: $(TARGET)
	@echo "Ejecutando $(TARGET)..."
	@./$(TARGET)

# ———————————————————————————
# Ayuda rápida
# ———————————————————————————
help:
	@echo "Uso:"; \
	echo "  make           — Compila el proyecto"; \
	echo "  make install-deps  — Instala SFML (Linux/macOS) o muestra instrucciones para Windows"; \
	echo "  make run       — Ejecuta el juego"; \
	echo "  make clean     — Elimina ejecutable y .o"; \
	echo "  make describe  — Resumen de mecánicas y controles"

# ———————————————————————————
# Descripción de jugabilidad y mecánicas
# ———————————————————————————
describe:
	@printf "\nEscape the Grid – Puzzle game\n\n"; \
	printf "Controles: clic izquierdo en celda adyacente para mover o interactuar.\n"; \
	printf "Mecánicas:\n"; \
	printf "  • Caja (marrón): recógela para ‘tener la llave’ (avatar VERDE).\n"; \
	printf "  • Puertas cerradas (rojo oscuro) → al avanzar con caja, se abren.\n"; \
	printf "  • Trampas (rojo claro): si pisas, vuelves al inicio.\n"; \
	printf "  • Switch walls (amarillo): alternan cada 5 turnos (consulta consola).\n"; \
	printf "  • Pressure plates (teal → cian): al dejar caja, abren puertas ligadas.\n"; \
	printf "  • Meta (verde): se mueve cada 10 turnos. ¡Llega antes para ganar!\n\n"; \
	printf "Cada movimiento o interacción suma 1 turno e imprime el contador en consola.\n\n"
