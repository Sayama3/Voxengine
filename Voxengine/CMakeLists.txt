cmake_minimum_required(VERSION 3.20)

# set enable the debugger (when the debug mode is enable?)
if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    if(NOT (DEFINED MSVC))
        set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")
    endif()
endif()

add_executable(Voxengine
        src/Voxengine.cpp
        src/Editor/EditorLayer.cpp
        include/Voxymore/Editor/EditorLayer.hpp
        src/Editor/Panels/SceneHierarchyPanel.cpp
        include/Voxymore/Editor/Panels/SceneHierarchyPanel.hpp
        src/Editor/Panels/PropertyPanel.cpp
        include/Voxymore/Editor/Panels/PropertyPanel.hpp
        src/Editor/Panels/SystemPanel.cpp
        include/Voxymore/Editor/Panels/SystemPanel.hpp
        include/Voxymore/Editor/Panels/Panel.hpp
        src/Editor/Panels/Panel.cpp
        src/Editor/Panels/ContentBrowserPanel.cpp
        include/Voxymore/Editor/Panels/ContentBrowserPanel.hpp
        include/Voxymore/Editor/EditorHelper.hpp
        include/Voxymore/Editor/Panels/Viewport.hpp
        src/Editor/Panels/Viewport.cpp
        src/Editor/Panels/AssetManagerPanel.cpp
        include/Voxymore/Editor/Panels/AssetManagerPanel.hpp
        src/Editor/Panels/DefaultShaders.hpp
        src/Editor/Panels/LogsPanel.cpp
        include/Voxymore/Editor/Panels/LogsPanel.hpp
)

target_compile_features(Voxengine PUBLIC cxx_std_20)

include_directories(include)

target_link_libraries(Voxengine PUBLIC Voxymore::Core)
target_precompile_headers(Voxengine REUSE_FROM Core)
#set_target_properties(Voxengine PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(Voxengine PUBLIC VXM_USE_SIMPLE_FUNCTION)


configure_file(imgui.ini ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini COPYONLY)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/EditorAssets/ ${CMAKE_CURRENT_BINARY_DIR}/EditorAssets/ RESULT copy_result COPY_ON_ERROR SYMBOLIC)
    if(NOT (copy_result EQUAL 0))
        message(WARNING
                "Copy made instead of symlink.\n"
                "${copy_result}"
        )
    endif()
else()
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/EditorAssets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/EditorAssets/)
endif()