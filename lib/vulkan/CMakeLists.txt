cmake_minimum_required(VERSION 3.25)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(Vulkan REQUIRED
        COMPONENTS glslc glslang SPIRV-Tools spirv-cross-cpp shaderc_combined glslang
)

