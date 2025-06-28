# Escape the Grid

**Escape the Grid** es un juego desarrollado en C++ usando SFML 3.0.0, orientado a la simulación y exploración de conceptos de lógica de juego sobre una cuadrícula.

## Requisitos

- CMake 3.20 o superior
- SFML 3.0.0 (instalado vía vcpkg)
- Visual Studio 2022 (o cualquier compilador C++ compatible con C++17)
- vcpkg integrado con Visual Studio: `.cpkg integrate install`

## Instalación

```bash
git clone https://github.com/jpoul02/escape-the-grid.git
cd escape-the-grid
```

## Controles

- `W`: Mover arriba
- `A`: Mover izquierda
- `S`: Mover abajo
- `D`: Mover derecha

Cada movimiento se da en bloques de `60.f` en una cuadrícula de 10x10.

## Estructura del proyecto

```
escape-the-grid/
├── src/
│   ├── main.cpp
│   ├── game/
│   │   ├── Game.hpp / Game.cpp
│   │   ├── Grid.hpp / Grid.cpp
│   │   └── Player.hpp / Player.cpp
├── CMakeLists.txt
├── README.md
```

## Características actuales

- Cuadrícula de 10x10 bloques.
- Movimiento controlado con teclado en pasos exactos de bloque.
- Renderizado de jugador como rectángulo rojo.
- Uso correcto de eventos modernos en SFML 3.0 (`std::optional<sf::Event>` y `getIf<T>()`).

## Por hacer

- Implementar colisiones.
- Generar laberinto o mapa con obstáculos.
- Meta o zona de escape.
- Cronómetro o scoring.
- Animaciones opcionales.

## Licencia

Distribuido bajo los términos de la licencia MIT.
