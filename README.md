# Escape the Grid - Ruinas Ancestrales (Versión SFML 3.0.0)

## 📥 Clonar el Repositorio

```bash
git clone https://github.com/tu-usuario/scape-the-grid.git
cd scape-the-grid
```

## 🛠️ Requisitos
- vcpkg integrado con Visual Studio: `.cpkg integrate install`
- vcpkg integrado con Visual Studio: `.cpkg integrate install`
- vcpkg integrado con Visual Studio: `.cpkg integrate install`

- **C++20 o superior**
- **[SFML 3.0.0](https://github.com/SFML/SFML)** (instalación vía vcpkg recomendada)
- Un compilador moderno (Visual Studio 2022, GCC 11+, Clang 12+)

## 📦 Instalación (Windows)

Requiere [vcpkg](https://github.com/microsoft/vcpkg):

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install sfml3:x64-windows
```

Asegúrate de que vcpkg esté en el ```CMAKE_TOOLCHAIN_FILE``` de tu proyecto.

# 🧱 Estructura del Proyecto

```plaintext
scape-the-grid/
├── assets/                    # Recursos (fuentes, etc.)
│   └── OpenSans-Regular.ttf
├── src/
│   ├── game/                  # Lógica principal del juego (Game.hpp/cpp, Player.hpp/cpp)
│   └── grid/                  # Manejo del grid y celdas (Grid.hpp/cpp)
├── main.cpp                   # Punto de entrada del juego
├── CMakeLists.txt             # Configuración de compilación
├── README.md
└── game.mk                    # Este archivo (documentación y setup)
```

# 🧩 Compilación y Ejecución

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/ruta/a/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Debug
./Debug/escape-the-grid.exe
```

Ajusta vcpkg.cmake según tu sistema. En Linux o MacOS, el ejecutable estará en ./escape-the-grid.

# 🕹️ Controles y Jugabilidad
-El juego se controla con el mouse: haz clic en una celda adyacente para moverte o interactuar.
-Cada clic (mover, recoger, soltar) cuenta como 1 turno.
-El objetivo es llegar a la meta verde, superando trampas, puertas y muros temporales.

# 🎨 Leyenda de Colores

| Color           | Tipo de Celda                        |
| --------------- | ------------------------------------ |
| Gris claro      | Celda vacía (transitable)            |
| Gris oscuro     | Muro (intransitable)                 |
| Azul            | Tu avatar al inicio                  |
| Verde           | Meta (puedes ganar si llegás)        |
| Rojo claro      | Trampa (te devuelve al inicio)       |
| Café claro      | Caja (objeto pesado)                 |
| Teal oscuro     | Placa de presión (apagada)           |
| Cian            | Placa activada                       |
| Rojo oscuro     | Puerta cerrada (requiere caja)       |
| Marrón claro    | Puerta abierta (puedes pasar)        |
| Amarillo vivo   | Muro temporal activo                 |
| Amarillo oscuro | Muro temporal desactivado            |

# ✅ ¿Cómo se gana?

Explora el laberinto:
    -Mueve tu avatar (azul) por las celdas grises claras evitando trampas.

Busca la caja (café claro):
    -Haz clic en ella (desde una celda adyacente) para recogerla.
    -El avatar se vuelve verde: significa que llevás la “llave”.

Dirígete a la puerta roja:
    -Con la caja en mano, al hacer clic sobre la puerta cerrada, se abrirá.

Evita trampas (rojo claro):
    -Si las pisás, regresarás al punto de partida.

Gestioná los muros temporales (amarillos):
    -Cambian cada 5 turnos entre bloqueado y transitable.
    -Usá el contador en consola para planear cuándo cruzar.

Llega a la meta (verde):
    -La meta se mueve cada 10 turnos.
    -Ganás si la pisás justo antes de que desaparezca (turno 10, 20, 30, etc.).
