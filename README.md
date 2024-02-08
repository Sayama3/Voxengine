[![build](https://github.com/Sayama3/VoxymoreCore/actions/workflows/cmake.yml/badge.svg)](https://github.com/Sayama3/VoxymoreCore/actions/workflows/cmake.yml)

# Voxengine
 
The Voxengine repository. This repository contains the Voxengine solution (*made with CMake*) and two project. Respectively "*Core*" and "*Voxymore*".

## Core

The core libray (or more like framework at this point) is where all the backend is located. The rendering logic, code, etc is done there.

## Voxymore

It's the editor for the Voxengine project. Later on, as the project advanced and scripting is implemented, this is what will be made as a installer for the project to be created.
(Probably throught another "*Launcher*" project. Which won't be in this repository.)

# Previews

Here's a bit of preview of the Engine capability as of September 2024.

![Buoyancy_Test_Scene.gif](ReadMe%2FBuoyancy_Test_Scene.gif)

![Spring_Test_Scene.gif](ReadMe%2FSpring_Test_Scene.gif)

# Requirement

For the project to work, you **need** to have the [Vulkan SDK]() installed.

On windows, do not forget to add the installation path to the environement variable (if it's not done by default).
ie. `VULKAN_SDK=C:\VulkanSDK\1.3.250.1`

# Credits

a little credit list to all the library I use. I should update this list pretty often but don't hesitate to make an issue if I forgot one.

- [imgui](https://github.com/ocornut/imgui): Bloat-free Graphical User interface for C++ with minimal dependencies
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h): public domain image loader
- [glfw](https://www.glfw.org): an Open source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop
- [glm](https://github.com/g-truc/glm): a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
- [glad](https://glad.dav1d.de): Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs
- [spdlog](https://github.com/gabime/spdlog): Very fast, header-only/compiled, C++ logging library.
- [EnTT](https://github.com/skypjack/entt): Tiny and easy to use header-only Entity Component System library, for game programming and much more. Written in modern C++.
- [yaml-cpp](https://github.com/jbeder/yaml-cpp): YAML parser and emitter in C++ matching the YAML 1.2 spec.
- [spriv-cross](https://github.com/KhronosGroup/SPIRV-Cross): practical tool and library for performing reflection on SPIR-V and disassembling SPIR-V back to high level languages.
- [shaderc](https://github.com/google/shaderc): A collection of tools, libraries, and tests for Vulkan shader compilation.
- [tinygltf](https://github.com/syoyo/tinygltf) (_2.8.17_) : Header only C++11 tiny glTF 2.0 library.
- [tracy](https://github.com/wolfpld/tracy) (_0.10.0_): A Real time, nanosecond resolution, remote telemetry, hybrid frame and sampling profiler for games and other applications.

# License

Well, for now I don't know what I want to do with this project, so there is no defined License. If I abandon the project I will add it a MIT License,
but for now I wait for the project to grow to decide on the commercialisation model (Full Open Source, Closed Source, Proprietary, anything else...)
