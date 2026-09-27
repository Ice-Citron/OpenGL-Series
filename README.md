# OpenGL Series

This repo contains the C++ code and study notes I written whilst following 
[The Cherno's OpenGL series][series].

I started this project when my work on Hazel exposed gaps in my OpenGL
knowledge. I wanted to understand the graphics API before I did more game engine
development.

## The example

The application creates a 640 × 480 window with GLFW. It requests an
OpenGL 3.3 core context and loads OpenGL functions through GLEW.

Four vertices and six indices define a rectangle as two triangles.
A GLSL uniform controls its colour. The red channel changes over time.

Start with [Application.cpp][application] to follow the setup and main loop.
The [shader file][shader] contains the vertex and fragment shader source.

## What the code covers

- **Vertex and index buffers.** C++ classes create GPU buffers and
  upload data. Their destructors release the buffer objects.
- **Vertex arrays.** A class connects vertex buffers to attribute layouts.
  It calculates attribute offsets from the layout.
- **Buffer layouts.** Template specialisations define supported attribute
  types. The layout records each element and calculates the vertex stride.
- **Shaders.** The shader class reads both shader stages from one file.
  It compiles the source and links the program.
- **Uniforms.** Setter methods update scalar and colour values.
  A cache stores uniform locations for later use.
- **Renderer.** A separate class provides clear and draw methods.
  The draw method binds the required objects before the indexed draw call.
- **Error checks.** The code reports OpenGL errors with the function name
  and source location. Shader compilation failures include the error log.

## Study notes

The notes record the concepts and API details I studied.
They include code extracts and explanations alongside the implementation.

- [Episodes 1–6][notes-1] — Vertex buffers, attributes, and shader basics.
- [Episodes 7–9][notes-2] — Shader source and the early examples.
- [Episodes 10–12][notes-3] — Error checks, uniforms, and vertex arrays.
- [Episodes 13–15][notes-4] — C++ classes for buffers, layouts, and shaders.
- [Episodes 16–18][notes-5] — Renderer structure and OpenGL object state.

The file names retain the original episode groups.
The [Episode 13 snapshot][snapshot] preserves an earlier implementation.

## Repository structure

```text
OpenGL-Series/
├── src/                 # C++ application and OpenGL classes
├── res/shaders/         # GLSL shader source
└── OpenGL-Series.vcxproj
Dependencies/
├── GLFW/
└── GLEW/
docs/notes/
├── Notes [EP...].md     # Five groups of study notes
└── Archive Code/        # Earlier code snapshot
OpenGL-Series.sln        # Visual Studio solution
LICENSE                 # Repository licence
```

## Build and run

The saved project targets Visual Studio 2022 with the v143 C++ toolset.
It uses the Windows 10 SDK. The repository includes GLFW and GLEW files.

1. Install Visual Studio 2022 with the C++ desktop tools and Windows SDK.
2. Open `OpenGL-Series.sln`.
3. Select the `Debug` configuration and the `x86` solution platform.
4. Set `OpenGL-Series` as the startup project.
5. Set the debugger's working directory to `$(ProjectDir)`.
6. Build and run the project.

The solution maps `x86` to the project's `Win32` configuration.
The saved x64 configurations do not contain the dependency paths.

## Credits and licence

The code follows [The Cherno's OpenGL series][series].
The notes preserve my study of the tutorial and the graphics API.

The root [LICENSE](LICENSE) contains the Apache License 2.0.
GLFW and GLEW retain their own licences.

[series]:
  https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
[application]: OpenGL-Series/src/Application.cpp
[shader]: OpenGL-Series/res/shaders/Basic.shader
[notes-1]: <docs/notes/Notes [EP1-EP6].md>
[notes-2]: <docs/notes/Notes [EP7-EP9].md>
[notes-3]: <docs/notes/Notes [EP10-EP12].md>
[notes-4]: <docs/notes/Notes [EP13-EP15].md>
[notes-5]: <docs/notes/Notes [EP16-EP18].md>
[snapshot]: <docs/notes/Archive Code/EP13 - 11-2-2024/>
