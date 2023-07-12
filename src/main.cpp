// Keep for later

//#define STB_IMAGE_IMPLEMENTATION
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <stb_image.h>
//#include <iostream>
//#include <string>
//#include "SystemHelper.hpp"
//#include "Renderer/ShaderProgram.hpp"
//#include "Renderer/Texture2D.hpp"
//#include "MathHelper.hpp"
//#include "Renderer/Camera.hpp"
//#include "Renderer/GLFWType.hpp"
//#include "Renderer/LightSource.hpp"
//#include "Renderer/Model.hpp"
//#include "ImguiContainer.hpp"
//
//// For visual studio, we need this to access the usual math macro.
//// Using it here as I cannot add it on the cmakelist.
//#ifndef _USE_MATH_DEFINES
//#define _USE_MATH_DEFINES
//#endif
//
//float _width = 800;
//float _height = 600;
//
//bool cursorDisable = true;
//
//Camera camera({_width, _height}, {0,0,3});
//
//// Set this in the renderer.
//double currentFrame = 0.0;
//double deltaTime = 0.001667; // Arbitrary 60fps
//double lastFrame = 0.0;
//bool spotLightEnable = false;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//
//int main() {
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//#if defined(IMGUI_IMPL_OPENGL_ES2)
//    // GL ES 2.0 + GLSL 100
//    const auto glsl_version = "#version 100";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
//    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
//#elif defined(__APPLE__)
//    // GL 3.2 + GLSL 150
//    const auto glsl_version = "#version 150";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
//    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
//#else
//    // GL 4.6 + GLSL 330
//    const auto glsl_version = "#version 330";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
//    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
//#endif
//
//    GLFWwindow* window = glfwCreateWindow(_width, _height, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    cursorDisable = false;
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSwapInterval(1);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//    glEnable(GL_DEPTH_TEST);
//
//    // === Light ===
//    LightSource lightSource;
//    ShaderProgram lightSourceShader(ShaderConstructor("resources/shaders/lightsource.vert", ShaderType::VERTEX_SHADER),
//                                    ShaderConstructor("resources/shaders/lightsource.frag", ShaderType::FRAGMENT_SHADER));
//    lightSourceShader.Bind();
//    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//
//    // === Shaders ===
//    ShaderProgram cubeShader(ShaderConstructor("resources/shaders/container.vert", ShaderType::VERTEX_SHADER),
//                             ShaderConstructor("resources/shaders/container.frag", ShaderType::FRAGMENT_SHADER));
//    cubeShader.Bind();
//    cubeShader.SetUniform<glm::vec3>("objectColor", {1.0f, 0.5f, 0.31f});
//    cubeShader.SetUniform<glm::vec3>("lightColor", {1.0f, 1.0f, 1.0f});
//
//    // === Models ===
//
//    glm::mat4 modelMatrix = glm::identity<glm::mat4>();
//    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
//    Model model1("resources/models/SurvivalGuitarBackpack/scene.gltf", modelMatrix);
//
//    cubeShader.Unbind();
//
//    glm::vec3 pointLightPositions[] = {
//            glm::vec3( 0.7f, 0.2f, 2.0f),
//            glm::vec3( 2.3f, -3.3f, -4.0f),
//            glm::vec3(-4.0f, 2.0f, -12.0f),
//            glm::vec3( 0.0f, 0.0f, -3.0f)
//    };
//
//
//    ImguiContainer imgui(window, glsl_version);
//    // Our IMGUI state
//    bool show_demo_window = true;
//    bool show_another_window = false;
//    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//    // === Rendering ===
//    while(!glfwWindowShouldClose(window))
//    {
//        // Frame duration processing.
//        currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // Input Processing
//        processInput(window);
//
//        // Clearing old image.
//        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        imgui.BeginFrame();
//
//        // Rendering LightSource
//        lightSourceShader.Bind();
//        for (auto pointLightPosition : pointLightPositions)
//        {
//            auto model = glm::mat4(1.0f);
//            model = glm::translate(model, pointLightPosition);
//            model = glm::scale(model, glm::vec3(0.2f));
//            lightSourceShader.SetUniform<glm::mat4>("model", model);
//            lightSourceShader.SetUniform<glm::mat4>("view", camera.GetViewMatrix());
//            lightSourceShader.SetUniform<glm::mat4>("projection", camera.GetProjectionMatrix());
//            lightSource.Draw();
//        }
//        lightSourceShader.Unbind();
//        lightSource.Unbind();
//
//        // Rendering meshes.
//        {
//            // Texture Update
//            cubeShader.Bind();
//
//            // Updating projection & view matrix.
//            cubeShader.SetUniform<glm::mat4>(ViewMatrixName, camera.GetViewMatrix());
//            cubeShader.SetUniform<glm::mat4>(ProjectionMatrixName, camera.GetProjectionMatrix());
//            cubeShader.SetUniform<glm::vec3>("viewPos", camera.GetPosition());
//
//            // Directional Light
//            {
//                cubeShader.SetUniform<glm::vec3>("directionalLight.direction", {-0.2f, -1.0f, -0.3f});
//
//                cubeShader.SetUniform<glm::vec3>("directionalLight.ambient", {0.2f, 0.2f, 0.2f});
//                cubeShader.SetUniform<glm::vec3>("directionalLight.diffuse", {0.5f, 0.5f, 0.5f});
//                cubeShader.SetUniform<glm::vec3>("directionalLight.specular", {1.0f, 1.0f, 1.0f});
//            }
//
//           // Point Lights
//           const int pointsLightsCount = sizeof(pointLightPositions)/ sizeof(pointLightPositions[0]);
//            cubeShader.SetUniform("pointsLightsCount", pointsLightsCount);
//            for (int i = 0; i < pointsLightsCount; ++i) {
//                cubeShader.SetUniform<glm::vec3>("pointLights["+std::to_string(i)+"].position", pointLightPositions[i]);
//
//                cubeShader.SetUniform<glm::vec3>("pointLights["+std::to_string(i)+"].ambient", {0.2f, 0.2f, 0.2f});
//                cubeShader.SetUniform<glm::vec3>("pointLights["+std::to_string(i)+"].diffuse", {0.5f, 0.5f, 0.5f});
//                cubeShader.SetUniform<glm::vec3>("pointLights["+std::to_string(i)+"].specular", {1.0f, 1.0f, 1.0f});
//
//                cubeShader.SetUniform<float>("pointLights["+std::to_string(i)+"].constant", 1.0f);
//                cubeShader.SetUniform<float>("pointLights["+std::to_string(i)+"].linear", 0.09f);
//                cubeShader.SetUniform<float>("pointLights["+std::to_string(i)+"].quadratic", 0.032f);
//            }
//
//            // SpotLight
//            cubeShader.SetUniform<bool>("spotLightEnable", spotLightEnable);
//            if(spotLightEnable)
//            {
//                cubeShader.SetUniform<glm::vec3>("spotLight.position", camera.GetPosition());
//                cubeShader.SetUniform<glm::vec3>("spotLight.direction", camera.GetForward());
//
//                cubeShader.SetUniform<glm::vec3>("spotLight.ambient", {0.2f, 0.2f, 0.2f});
//                cubeShader.SetUniform<glm::vec3>("spotLight.diffuse", {0.5f, 0.5f, 0.5f});
//                cubeShader.SetUniform<glm::vec3>("spotLight.specular", {1.0f, 1.0f, 1.0f});
//
//                cubeShader.SetUniform<float>("spotLight.constant", 1.0f);
//                cubeShader.SetUniform<float>("spotLight.linear", 0.09f);
//                cubeShader.SetUniform<float>("spotLight.quadratic", 0.032f);
//
//                cubeShader.SetUniform<float>("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//                cubeShader.SetUniform<float>("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
//            }
//
//            // Meshes
////            for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
////            {
////                auto &pos = cubePositions[i];
////                auto model = glm::identity<glm::mat4>();
////                model = glm::translate(model, pos);
////                float angle = 20.0f * static_cast<float>(i);
////                if (i % 3 == 0) {
////                    angle += static_cast<float>(glfwGetTime()) * rotationSpeed * (i % 6 ? -1.0f : 1.0f);
////                }
////                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
////
////                cubeShader.SetUniform<glm::vec3>("material.ambient", {0.8f, 0.8f, 0.8f});
//////                cubeShader.SetUniform<int>("material.diffuseMat", 0);
//////                cubeShader.SetUniform<int>("material.specularMat", 1);
////                cubeShader.SetUniform<float>("material.shininess", 32.0f);
////
////                cubeShader.SetUniform<glm::mat4>("model", model);
////                model1.Draw(cubeShader);
////            }
//
//            {
//                model1.Draw(cubeShader);
//            }
//            cubeShader.Unbind();
//        }
//
//        // Our ImGUI Window
//        {
//            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//            if (show_demo_window)
//                ImGui::ShowDemoWindow(&show_demo_window);
//
//            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//            {
//                static float f = 0.0f;
//                static int counter = 0;
//
//                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//                ImGui::Checkbox("Another Window", &show_another_window);
//
//                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//                    counter++;
//                ImGui::SameLine();
//                ImGui::Text("counter = %d", counter);
//
//                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imgui.GetImGui()->Framerate, imgui.GetImGui()->Framerate);
//                ImGui::End();
//            }
//
//            // 3. Show another simple window.
//            if (show_another_window)
//            {
//                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//                ImGui::Text("Hello from another window!");
//                if (ImGui::Button("Close Me"))
//                    show_another_window = false;
//                ImGui::End();
//            }
//        }
//
//        imgui.EndFrame();
//
//        // check and call events and swap the buffers
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    _width = width;
//    _height = height;
//    camera.OnScreenSizeChange({width, height});
//}
//bool hasPressCtrl = false;
//bool hasPressF = false;
//void processInput(GLFWwindow *window)
//{
//    // Simple exit with the "escape" button for now.
//    if(glfwGetKey(window, GLFWKey::KEY_ESCAPE) == GLFWKeyState::PRESS)
//    {
//        glfwSetWindowShouldClose(window, true);
//    }
//    if(glfwGetKey(window, GLFWKey::KEY_LEFT_CONTROL) == GLFWKeyState::PRESS && !hasPressCtrl)
//    {
//        hasPressCtrl = !hasPressCtrl;
//        if(cursorDisable) {
//            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//            camera.ResetCursor();
//        } else {
//            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//        }
//        cursorDisable = !cursorDisable;
//    } else if(glfwGetKey(window, GLFWKey::KEY_LEFT_CONTROL) == GLFWKeyState::RELEASE && hasPressCtrl) {
//        hasPressCtrl = !hasPressCtrl;
//    }
//
//    if(!cursorDisable) {
//        camera.ProcessKeyboard(window, deltaTime);
//        if (glfwGetKey(window, GLFWKey::KEY_F) == GLFWKeyState::PRESS && !hasPressF) {
//            spotLightEnable = !spotLightEnable;
//            hasPressF = true;
//        } else if(glfwGetKey(window, GLFWKey::KEY_F) == GLFWKeyState::RELEASE) {
//            hasPressF = false;
//        }
//    } else {
//        hasPressF = false;
//    }
//}
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//    if(!cursorDisable) {
//        camera.ProcessMouseCursor({xpos, ypos});
//    }
//}
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    if(!cursorDisable) {
//        camera.ProcessMouseScroll({xoffset, yoffset});
//    }
//}