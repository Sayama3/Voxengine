[![build](https://github.com/Sayama3/VoxymoreCore/actions/workflows/cmake.yml/badge.svg)](https://github.com/Sayama3/VoxymoreCore/actions/workflows/cmake.yml)

# Voxengine

The Voxengine is currently a general purpose Game Engine with it's own tech. Inside there is :
- a Renderer made with OpenGL.
- a physic engine
- an editor (made with [ImGui](https://github.com/ocornut/imgui))
- scripting in C++

And will have later on :

- [ ] Audio Engine
- [ ] Lua Scritping
- [ ] Nodal Scripting
- [ ] Nodal Shader Creation

---

The renderer support the GLTF 2.0 format for the 3D model and will support import of other formats later on.

## Core

The core library (or more like framework at this point) is where all the backend is located. The rendering logic, code, etc is done there.

## Voxymore

It's the editor for the Voxengine project. Later on, as the project advanced and scripting is implemented, this is what will be made as an installer for the project to be created.
(Probably through another "*Launcher*" project. Which won't be in this repository.)

# Previews

Here's a bit of a preview of the Engine capability as of March 2024.

![Rigidbody_Basic_Buoyancy.gif](ReadMe%2FRigidbody_Basic_Buoyancy.gif)
![Rigidbody_Spring.gif](ReadMe%2FRigidbody_Spring.gif)

# Requirement

For the project to work, you **need** to have the [Vulkan SDK]() installed.

On windows, do not forget to add the installation path to the environement variable (if it's not done by default).
ie. `VULKAN_SDK=C:\VulkanSDK\1.3.250.1`

# Credits

a little credit list to all the library I use. I should update this list pretty often but don't hesitate to make an issue if I forgot one.

- [imgui](https://github.com/ocornut/imgui): Bloat-free Graphical User interface for C++ with minimal dependencies
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)Immediate mode 3D gizmo for scene editing and other controls based on Dear Imgui
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h): public domain image loader
- [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h): public domain image loader
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
- [hash-library](https://create.stephan-brumme.com/hash-library/) (version 8): Portable C++ Hashing Library (*a little edition has been made to add `<machine/endian.h>` or other include to make it work on MacOS.*)
- [json](https://github.com/nlohmann/json) JSON for Modern C++
- [static_block](Core/vendor/static_block.hpp) An implementation of a Java-style static block, in C++ (and potentially a GCC/clang extension to avoid warnings).

# License

Well, for now I don't know what I want to do with this project, so there is no defined License. If I abandon the project I will add it a MIT License,
but for now I wait for the project to grow to decide on the commercialization model (Full Open Source, Closed Source, Proprietary, anything else...)
