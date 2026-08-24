# Midnight Walk

A small 3D graphics application made in C++ and OpenGL for the Computer Graphics course at the Faculty of Mathematics, University of Belgrade.

The project presents a first-person nighttime scene where the player can explore the environment with a flashlight.
The scene features custom lighting and post-processing effects.

## Features
- First-person camera and movement
- Mouse-controlled camera rotation
- Flashlight with configurable lighting
- Multiple point lights / street lamps
- Bloom post-processing
- Skybox
- Several imported models
- ImGui-based GUI

## Controls
| Key	| Action |
| --- | --- |
| `W A S D` |	movement |
| `X`	| toggle flashlight |
| `ESC` |	exit |
| `F2` | toggle GUI |
| `F` | flicker animation |

## Building

Build the project and run it via:

### Linux

```bash
git clone https://github.com/filipjevtovic/Midnight-walk.git
cd Midnight-walk
./setup.sh
./build-project.sh
cd build
./APP
```
...or run it via Docker:
```bash
xhost +local:docker
docker run --rm --env DISPLAY=$DISPLAY\
      --volume /tmp/.X11-unix:/tmp/.X11-unix\
      --device /dev/dri project-image
```

## Project Structure
app/
├── include/       # Application headers
├── src/           # Application and scene controllers
├── resources/     # Models, textures and shaders
├── config.json    # Resource configuration
└── CMakeLists.txt
engine/             # Course-provided graphics engine

## Credits

Built as a project for the Computer Graphics course at the Faculty of Mathematics, University of Belgrade.
The project is based on https://github.com/matf-racunarska-grafika/matf-rg-project-2024.
