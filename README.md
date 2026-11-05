# PlatformGame — v1.0

**Group 13** — Jonathan Bermello, Sandra Salvador

Assignment 1 · Platformer  
Video Game Development (804237 DESVJ) · CITM UPC

---

## The game

SandKnight is a 2D platformer developed in C++ using SDL3 and Box2D. The player controls a knight who must explore an arena, avoid spikes, stay away from deadly pits, collect coins and survive using three lives.

**Features not yet implemented:** Enemies and the main menu. The game currently includes a damage sound effect.

## How to play

Download the game archive, extract it and run `Game.exe`. Make sure that `config.xml` and the `Assets` folder are located in the same directory as the executable.

| Key        | Action                     |
|------------|----------------------------|
| A / D      | Move left / right          |
| Space      | Jump                       |
| Left Shift | Roll                       |
| Enter      | Restart after Game Over    |
| ESC        | Quit the game              |

The objective is to survive the level. Spikes remove one life, while falling into a pit causes instant death. When all three lives are lost, the Game Over screen appears, allowing the player to restart the level by pressing Enter.

## Debug keys

| Key        | Action                          |
|------------|---------------------------------|
| H          | Show / hide the help menu       |
| F9         | Show / hide Box2D colliders     |
| F10        | Enable / disable God Mode       |
| F11        | Enable / disable the 30 FPS cap |
| F8         | Enable / disable VSync          |
| Arrow keys | Move the camera manually        |

## Features in this release

### Player

The player controls a knight with animated sprites and physics implemented using Box2D. The available animations include idle, running, jumping, rolling, taking damage and dying.

The character can move horizontally, jump and perform a roll to move faster. The system includes ground detection, three lives, temporary invulnerability (0.5 seconds) and blinking after taking damage. When the player dies, they respawn at the spawn point defined in the map.

### Map

The level was created using Tiled and contains different visual and collision layers.

- **Map** and **delante**: Visual layers of the environment.
- **Collisions**: Defines platforms, spikes and pits.
- **Spawn**: Object layer that defines the player's starting position.

The map uses different collision types to distinguish solid platforms from dangerous areas.

### Data from files

The player's configuration is loaded from `config.xml`. It includes texture paths, animation definitions, movement speed, jump strength, frame dimensions and sound effects.

Animations are defined through Tiled tileset and animation files (`.tsx`). Textures, sounds and other resources are loaded from the `Assets` folder.

### Engine

The game is developed in C++20 using SDL3 and Box2D 3.1.

The engine includes a main game loop with delta time, frame rate control and VSync support. The window title displays engine information, including performance and FPS.

## Extra features

- **Rolling mechanic:** Allows the player to perform a roll to move faster and cover distances more quickly.

## Credits

- **Sprites:** analogStudios_ (knight, slime, platforms and coins) and RottingPixels (world tileset and fruits).
- **Sound effects and music:** Roblox.
- **Physics engine:** Box2D.

## Team

**Jonathan Bermello:** Implementation of the engine and its main systems, player mechanics, physics, animations and the overall game logic.

**Sandra Salvador:** Collaborated in different areas of development, supporting the implementation of several classes and features, such as the rolling mechanic, sound effects integration and map development.
