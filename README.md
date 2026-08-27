# NEngine

3D Engine **NEngine**, written in **2002-2003** by **Alexey Egorov**.

This was a **school project** made when the author was **15-16 years old**.
The engine is written in C++ for Windows using **OpenGL 1.1** and the classic
Win32 / Microsoft Developer Studio (Visual C++) build system.

## Functionality

- **OpenGL renderer** — double-buffered windowed and fullscreen rendering with
  perspective projection, depth buffering, alpha blending and face culling.
- **Display setup dialog** — resolution selection (320x200 .. 1600x1200) and
  color depth (8/16/32 bpp), plus toggles for fullscreen, mipmapping,
  trilinear and bilinear filtering.
- **Custom model formats**:
  - `.mda` — animated models with keyframe-based vertex animation and linear
    interpolation between frames (multi-animation support).
  - `.mds` — static models with per-face material groups.
- **Materials** — diffuse color, transparency/opacity, two-sided and wireframe
  flags, plus reflection maps.
- **Textures** — Targa (`.tga`) loader with an on-the-fly texture cache and
  deduplication, mipmap generation, combined diffuse+opacity textures and
  procedurally generated Phong (specular) maps.
- **Multitexture** — runtime detection of `GL_ARB_multitexture` and
  `GL_EXT_texture_env_combine` with manual extension loading.
- **Text rendering** — bitmap font textures and 2D text output (including a
  word-wrapping variant for the console).
- **In-game console** — toggleable with `~`, scrollable history, and commands
  such as `fov`, `consize`, `fps`, `halt`.
- **Input** — keyboard and mouse (mouse-look style relative movement).
- **Scene / world**:
  - Static level rendering and skybox.
  - Animated characters with walk/idle animation.
  - First-person view-model weapon rendering and camera-aligned billboards.
  - Weapon system (`.wpn` definitions referencing models; pistol/rifle/sniper/RPG
    types).
- **Demo scene** — loads a level, two animated characters, a skybox, four
  weapons and an FPS counter; movement with WASD/arrows, FOV control and
  windowed/fullscreen switching (F2) via keyboard.
- **Logging** — console/engine messages written to `NEngine.log`; a particle
  system module stub is also included.

## Controls

- **Mouse** — look around (mouse-look).
- **Esc** — exit the program.
- **F2** — toggle fullscreen / windowed (restarts the engine).
- **- / =** — decrease / increase field of view.
- **1 / 2 / 3 / 4** — select weapon (AK, Pushka, RPG-7, PPSha).
- **W / A / S / D** — move the camera.
- **Ctrl** — move camera up.
- **Space** — move camera down.
- **Arrow Up / Down** — move the animated character forward / backward
  (plays the walk animation).
- **Arrow Left / Right** — rotate the character.
- **~** — toggle the in-game console.

Console commands: `fov <n>`, `consize <n>`, `fps`, `halt`.

## Credits

- **Dogi model** — Dmitriy Kokoteev.

## Building

The project targets Microsoft Visual C++ (`.dsp`/`.dsw` project files, Developer
Studio 5/6 era) on Windows. Requires OpenGL, GLU and the Windows SDK.
Run `clean.bat` to remove build artifacts.
