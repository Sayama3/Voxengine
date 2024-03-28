//
// Created by ianpo on 21/08/2023.
//

#include "Voxymore/Editor/EditorLayer.hpp"
#include "Voxymore/Editor/Panels/SystemPanel.hpp"
#include "Voxymore/Utils/Platform.hpp"
#include <ImGuizmo.h>


namespace Voxymore::Editor {
    EditorLayer::EditorLayer() : Layer("EditorLayer"), m_EditorCamera(60.0f, 1280.0f/ 720.0f, 0.1f, 1000.0f)
    {
        VXM_PROFILE_FUNCTION();
        Application::Get().GetWindow().SetCursorState(CursorState::None);
    }

    void EditorLayer::OnAttach()
    {
        VXM_PROFILE_FUNCTION();

        auto* imgui = Application::Get().FindLayer<ImGuiLayer>();
		VXM_CORE_CHECK(imgui, "ImGui must be set before adding the EditorLayer.")
		if(imgui) {
			auto &imguiLayer = *imgui;
			imguiLayer.AddFont({FileSource::EditorAsset, "fonts/OpenSans/OpenSans-Regular.ttf"}, 18.0f, FontType::Regular, true);
			imguiLayer.AddFont({FileSource::EditorAsset, "fonts/OpenSans/OpenSans-Bold.ttf"}, 18.0f, FontType::Bold);
		}

        m_OnProjectReloadId = Project::AddOnLoad(VXM_BIND_EVENT_FN(ReloadAssets));

        std::filesystem::path project;
        if(Application::Get().HasArgument(1))
        {
            project = Application::Get().GetArgument(1);
        }

        // Loading project.
        if (project.empty())
        {
            Project::New();
        }
        else
        {
            if(std::filesystem::exists(project))
            {
                Project::Load(project);
            }
            else
            {
                Project::New();
                Project::SaveActive(project);
            }
        }

        FramebufferSpecification specification(1280, 720);
        specification.Attachements = {FramebufferTextureFormat::Color, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};
        m_ViewportSize = {specification.Width, specification.Height};
        m_Framebuffer = Framebuffer::Create(specification);

        if(Project::GetConfig().startSceneId.has_value())
        {
            OpenScene(Project::GetConfig().startSceneId.value());
        }
        else
        {
            CreateNewScene();
        }
    }

    void EditorLayer::OnDetach()
    {
        VXM_PROFILE_FUNCTION();

        Project::RemoveOnLoad(m_OnProjectReloadId);
    }

    //TODO: Pause the update when m_SceneState == SceneState::Pause
    void EditorLayer::OnUpdate(TimeStep timeStep) {
        VXM_PROFILE_FUNCTION();

        // Resize
        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
                m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
                (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        }

        if(m_SceneState == SceneState::Edit || m_SceneState == SceneState::Pause)
		{
			m_EditorCamera.OnUpdate(timeStep);
		}

		{
            VXM_PROFILE_SCOPE("Rendering Preparation.");
            m_Framebuffer->Bind();
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            RenderCommand::Clear();
            m_Framebuffer->ClearColorAttachment(1, -1);
        }

        {
            VXM_PROFILE_SCOPE("Rendering Scene");

            switch (m_SceneState)
            {
				case SceneState::Pause:
                case SceneState::Edit:
                {
                    m_ActiveScene->OnUpdateEditor(timeStep, m_EditorCamera);
                    break;
                }
                case SceneState::Play:
                {
                    m_ActiveScene->OnUpdateRuntime(timeStep);
                    break;
                }
				default:
                {
                    VXM_CORE_ASSERT(false, "State {0} not handled yet.", static_cast<int>(m_SceneState));
                    break;
                }
            }

            {
                auto [fMouseX, fMouseY] = ImGui::GetMousePos();
                fMouseX -= m_ViewportBounds[0].x;
                fMouseY -= m_ViewportBounds[0].y;

                auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
                auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;

                //Reverse Y because the texture is reversed.
                fMouseY = viewportHeight - fMouseY;

                int mouseX = fMouseX;
                int mouseY = fMouseY;

                if(mouseX > 0 && mouseX < viewportWidth
                   && mouseY > 0 && mouseY < viewportHeight)
                {
                    int value = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
                    if(value < 0)
                    {
                        m_HoveredEntity = Entity();
                    }
                    else
                    {
                        auto entityId = static_cast<uint32_t>(value);
                        m_HoveredEntity = Entity(static_cast<entt::entity>(entityId), m_ActiveScene.get());
                    }
                    //TODO: pick hovered entity.
                }
            }

            m_Framebuffer->Unbind();
        }
    }

    void EditorLayer::RenderDockspace()
    {
		VXM_PROFILE_FUNCTION();
        // READ THIS !!!
        // TL;DR; this demo is more complicated than what most users you would normally use.
        // If we remove all options we are showcasing, this demo would become:
        //     void ShowExampleAppDockSpace()
        //     {
        //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //     }
        // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
        // In this specific demo, we are not using DockSpaceOverViewport() because:
        // - (1) we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
        // - (2) we allow the host window to have padding (when opt_padding == true)
        // - (3) we expose many flags and need a way to have them visible.
        // - (4) we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport()
        //      in your code, but we don't here because we allow the window to be floating)

        static bool dockspaceOpen = false;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minSizeWidthX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
            ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::SmallButton("click here"))
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }
        style.WindowMinSize.x = minSizeWidthX;

        if (ImGui::BeginMenuBar())
        {
			RenderMenuBar();

            // Disabling the following option menu but keeping it for reference.
//                if (ImGui::BeginMenu("Options"))
//                {
//                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
//                    // which we can't undo at the moment without finer window depth/z control.
//                    ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
//                    ImGui::MenuItem("Padding", NULL, &opt_padding);
//                    ImGui::Separator();
//
//                    if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
//                    if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
//                    if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
//                    if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
//                    if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
//                    ImGui::Separator();
//
//                    if (ImGui::MenuItem("Close Dockspace", NULL, false))
//                        dockspaceOpen = false;
//                    ImGui::EndMenu();
//                }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event) {
        VXM_PROFILE_FUNCTION();
        m_EditorCamera.OnEvent(event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMousePressed));
    }

    void EditorLayer::RenderMenuBar()
    {

		if (ImGui::BeginMenu("File"))
		{
			if (m_SceneState == SceneState::Edit && ImGui::BeginMenu("Project")) {
				if (ImGui::MenuItem("New")) {
					NewProject();
				}

				if (ImGui::MenuItem("Open...")) {
					OpenProject();
				}

				if (ImGui::MenuItem("Save as...")) {
					SaveProjectAs();
				}

				if (ImGui::MenuItem("Save")) {
					SaveProject();
				}
				ImGui::EndMenu();
			}

			if (m_SceneState == SceneState::Edit && ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("New", "Ctr+N")) {
					CreateNewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctr+O")) {
					OpenScene();
				}

				if (ImGui::MenuItem("Save as...", "Ctr+Shift+S")) {
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Save", "Ctr+S")) {
					SaveScene();
				}

				if(m_ActiveScene != nullptr && ImGui::MenuItem("Make Scene main scene"))
				{
					Project::SetMainScene(m_ActiveScene->GetID());
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				Application::Get().Close();
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Panels"))
		{
			if (ImGui::MenuItem("Shader Panel")) {
				m_ShaderPanel.Open();
			}

			if (ImGui::MenuItem("Systems")) {
				m_SystemPanel.Open();
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Views"))
		{
			if (ImGui::MenuItem("Reset Camera")) {
				m_EditorCamera.SetFocusTarget({0,0,0});
				m_EditorCamera.SetDistance(10);
			}

			ImGui::EndMenu();
		}
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if(e.GetRepeatCount() > 0) return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);
        bool rightClick = Input::IsMouseButtonPressed(Mouse::Right);

        switch (e.GetKeyCode()) {
            case Key::S:
            {
                if(control)
                {
                    if (shift) {
                        SaveSceneAs();
                    } else {
                        SaveScene();
                    }
                }

                break;
            }
            case Key::N:
            {
                if(control) CreateNewScene();
                break;
            }
            case Key::O:
            {
                if(control) OpenScene();
                break;
            }
            case Key::Q:
            {
                if(!rightClick && !(control || shift || alt)) m_GizmoOperation = GizmoOperation::NONE;
                break;
            }
            case Key::W:
            {
                if(!rightClick && !(control || shift || alt)) m_GizmoOperation = GizmoOperation::TRANSLATE;
                break;
            }
            case Key::E:
            {
                if(!rightClick && !(control || shift || alt)) m_GizmoOperation = GizmoOperation::ROTATE;
                break;
            }
            case Key::R:
            {
                if(!rightClick && !(control || shift || alt)) m_GizmoOperation = GizmoOperation::SCALE;
                break;
            }
            case Key::T:
            {
                if(!rightClick && !(control || shift || alt)) m_GizmoOperation = GizmoOperation::UNIVERSAL;
                break;
            }
            case Key::Z:
            {
                if(!rightClick && !(control || shift || alt)) m_GizmoMode = m_GizmoMode == GizmoMode::WORLD ? GizmoMode::LOCAL : GizmoMode::WORLD;
                break;
            }
            case Key::F:
            {
                if(!rightClick && !(control || shift || alt))
                {
                    auto selected = m_SceneHierarchyPanel.GetSelectedEntity();
                    if (selected) {
                        auto selectedTc = selected.GetComponent<TransformComponent>();
                        m_EditorCamera.SetFocusTarget(selectedTc.GetPosition());
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }
        return false;
    }

    bool EditorLayer::OnMousePressed(MouseButtonPressedEvent& e)
    {
        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);

        switch (e.GetMouseButton())
        {
            case Mouse::Left:
            {
                if (m_ViewportHovered)
                {
                    if(!m_SceneHierarchyPanel.GetSelectedEntity().IsValid() || m_GizmoOperation == GizmoOperation::NONE) m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
                    else if (m_HoveredEntity.IsValid() && !ImGuizmo::IsOver() && !ImGuizmo::IsUsingAny() && !control && !shift && !alt) m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
                }
                break;
            }
        }
        return false;
    }

    void EditorLayer::NewProject()
    {
        Project::New();
        CreateNewScene();
    }

    void EditorLayer::OpenProject()
    {
        std::filesystem::path file = FileDialogs::OpenFile({"Voxymore Project (*.vxm)", "*.vxm"});
        if(!file.empty())
        {
            if(!file.string().ends_with(".vxm")) file.append(".vxm");
            OpenProject(file);
        }
    }

    void EditorLayer::OpenProject(const std::filesystem::path& path)
    {
        if(Project::Load(path))
        {
            if(Project::GetConfig().startSceneId.has_value())
            {
                OpenScene(Project::GetConfig().startSceneId.value());
            }
            else
            {
                CreateNewScene();
            }
        }
    }

    void EditorLayer::SaveProject()
    {
        Project::SaveActive();
    }

    void EditorLayer::SaveProjectAs()
    {
        std::filesystem::path file = FileDialogs::SaveFile({"Voxymore Project (*.vxm)", "*.vxm"});
        if(!file.empty())
        {
            if(!file.string().ends_with(".vxm")) file.append(".vxm");
            SaveProjectAs(file);
        }
    }

    void EditorLayer::SaveProjectAs(const std::filesystem::path& path)
    {
        Project::SaveActive(path);
    }

    void EditorLayer::SaveSceneAs()
    {
        if(m_SceneState != SceneState::Edit)
        {
            VXM_CORE_WARNING("Cannot save the scene if we are not in edit mode.");
            return;
        }
        VXM_CORE_TRACE("Save Scene As");
        std::string file = FileDialogs::SaveFile({"Voxymore Scene (*.vxm)", "*.vxm"});
        if(!file.empty())
        {
            if(!file.ends_with(".vxm")) file.append(".vxm");
            SceneSerializer serializer(m_ActiveScene);
            if(serializer.Serialize(file))
            {
                m_FilePath = file;
                if(!SceneManager::HasScene(m_ActiveScene->GetID())) SceneManager::AddScene(m_ActiveScene);
            }
        }
    }

    void EditorLayer::CreateNewScene()
    {
        VXM_CORE_INFO("Unloading and Delete Previous Scene");
        if(m_ActiveScene != nullptr && SceneManager::HasScene(m_ActiveScene->GetID()))
        {
            SceneManager::DeleteScene(m_ActiveScene->GetID());
            m_ActiveScene = nullptr;
        }

        VXM_CORE_TRACE("Create New Scene");
        m_FilePath = std::string();
        m_ActiveScene = SceneManager::CreateScene();
        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		// Creating a default light on the empty scenes.
		auto e = m_ActiveScene->CreateEntity("Directional Light");
		auto& light = e.AddComponent<LightComponent>();
		light.m_LightType = LightType::Directional;
		e.GetComponent<TransformComponent>().SetEulerRotation({-45, 45, 0});
    }

    void EditorLayer::SaveScene()
    {
        if(m_SceneState != SceneState::Edit)
        {
            VXM_CORE_WARNING("Cannot save the scene if we are not in edit mode.");
            return;
        }
        if(!m_FilePath.empty())
        {
            VXM_CORE_TRACE("Save Scene");
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(m_FilePath);
        }
        else
        {
            SaveSceneAs();
        }
    }

    void EditorLayer::OpenScene()
    {
        VXM_CORE_TRACE("Open Scene");
        std::filesystem::path file = FileDialogs::OpenFile({"Voxymore Scene (*.vxm)", "*.vxm"});
        if(!file.empty())
        {
            OpenScene(file);
        }
    }

    void EditorLayer::OpenScene(const Path& path)
    {
        VXM_CORE_TRACE("Open Scene with path");
        if(!path.empty())
        {
            OpenScene(path.GetFullPath());
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        VXM_CORE_INFO("Unloading and Delete Previous Scene");
        if(m_ActiveScene != nullptr && SceneManager::HasScene(m_ActiveScene->GetID()))
        {
            SceneManager::DeleteScene(m_ActiveScene->GetID());
            m_ActiveScene = nullptr;
        }

        VXM_CORE_TRACE("Open Scene from path {0}", path.string());
        m_FilePath = path.string();
        m_ActiveScene = SceneManager::CreateScene(path, m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene(UUID id)
    {
        VXM_CORE_INFO("Unloading and Delete Previous Scene");
        if(m_ActiveScene != nullptr && SceneManager::HasScene(m_ActiveScene->GetID()))
        {
            SceneManager::DeleteScene(m_ActiveScene->GetID());
            m_ActiveScene = nullptr;
        }

        VXM_CORE_ASSERT(SceneManager::HasScene(id), "No scene with the ID '{0}'", id);
        m_ActiveScene = SceneManager::GetScene(id);
        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        //TODO: Get the associated file path.
        m_FilePath = std::string();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnImGuiRender() {
        VXM_PROFILE_FUNCTION();
        RenderDockspace();

        m_SceneHierarchyPanel.OnImGuiRender();
        m_SystemPanel.OnImGuiRender();
		m_ShaderPanel.OnImGuiRender();

        DrawImGuiViewport();

        DrawImGuiToolbar();
    }

    void EditorLayer::DrawImGuiViewport()
    {
        VXM_PROFILE_FUNCTION();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
		auto* imgui = Application::Get().FindLayer<ImGuiLayer>();
		if(imgui) {
			imgui->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		}
        m_EditorCamera.SetViewportFocused(m_ViewportFocused);
        m_EditorCamera.SetViewportHovered(m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = glm::uvec2(static_cast<uint32_t>(viewportPanelSize.x), static_cast<uint32_t>(viewportPanelSize.y));

        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if(m_SceneState == SceneState::Edit || m_SceneState == SceneState::Pause) DrawGizmos();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::DrawImGuiToolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 2));

        auto& colors = ImGui::GetStyle().Colors;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(colors[ImGuiCol_ButtonHovered].x, colors[ImGuiCol_ButtonHovered].y, colors[ImGuiCol_ButtonHovered].z, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(colors[ImGuiCol_ButtonActive].x, colors[ImGuiCol_ButtonActive].y, colors[ImGuiCol_ButtonActive].z, 0.5f));

        ImGui::Begin("toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        Ref<Texture2D> imageTex =  m_SceneState == SceneState::Edit ? m_PlayTexture : m_StopTexture;
		Ref<Texture2D> pauseTex = m_SceneState != SceneState::Pause ? m_PauseTexture : m_PlayTexture;

        auto size = ImGui::GetWindowSize();
        float buttonSize = glm::max(glm::min(size.x, size.y) - 4.0f, 20.0f);
		auto offset = buttonSize + 4.f;
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x * 0.5f - offset);
		// Play/stop button
        if(ImGui::ImageButton((ImTextureID)(uint64_t)imageTex->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1,1), 0))
        {
			switch (m_SceneState) {
				case SceneState::Edit:
				{
                	OnScenePlay();
					break;
				}
				case SceneState::Pause:
				case SceneState::Play:
				{
					OnSceneStop();
					break;
				}
			}
        }

		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x * 0.5f + 4.f);

		// pause resume button
		ImGui::BeginDisabled(m_SceneState != SceneState::Play && m_SceneState != SceneState::Pause);
        if(ImGui::ImageButton((ImTextureID)(uint64_t)pauseTex->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1,1), 0))
        {
			switch (m_SceneState) {
				case SceneState::Play:
				{
					OnScenePause();
					break;
				}
				case SceneState::Pause:
				{
					OnSceneResume();
					break;
				}
        	}
        }
		ImGui::EndDisabled();

        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
    }

    void EditorLayer::DrawGizmos()
    {
        DrawGizmosWindow();
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if(selectedEntity)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            // Runtime Camera
//            Entity cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
//            const auto& cameraComponent = cameraEntity.GetComponent<CameraComponent>();
//            const auto& transformComponent = cameraEntity.GetComponent<TransformComponent>();
//
//            const glm::mat4 projectionMatrix = cameraComponent.Camera.GetProjectionMatrix();
//            const glm::mat4 viewMatrix = glm::inverse(transformComponent.GetTransform());

            // Editor Camera
            const glm::mat4 projectionMatrix = m_EditorCamera.GetProjectionMatrix();
            const glm::mat4& viewMatrix = m_EditorCamera.GetViewMatrix();

            // Selected Entity
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 modelMatrix = tc.GetTransform();

            bool isSnapping = (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl)) && m_GizmoOperation != GizmoOperation::UNIVERSAL;

            float snapValue= m_GizmoOperation == GizmoOperation::TRANSLATE
                             ? m_GizmoTranslationSnapValue
                             : m_GizmoOperation == GizmoOperation::ROTATE
                               ? m_GizmoRotationSnapValue
                               : m_GizmoScaleSnapValue;

            float snapValues[3] = {snapValue, snapValue, snapValue};
            bool manipulate = ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), static_cast<ImGuizmo::OPERATION>(m_GizmoOperation), static_cast<ImGuizmo::MODE>(m_GizmoMode), glm::value_ptr(modelMatrix), nullptr, isSnapping ? snapValues : nullptr);
            m_EditorCamera.EnableMovement(!manipulate);
            if(ImGuizmo::IsUsing())
            {
                glm::vec3 scale;
                glm::vec3 position;
                glm::quat rotation;

                Math::DecomposeTransform(modelMatrix, position, rotation, scale);

                tc.SetPosition(position);
                tc.SetRotation(rotation);
                tc.SetScale(scale);
            }

			//TODO: Panel ImGuizmo drawing
			m_SceneHierarchyPanel.OnImGuizmo(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
        }
    }

    void EditorLayer::DrawGizmosWindow()
    {
        ImGui::Begin("Gizmo Params");

        if (ImGui::RadioButton("Select", m_GizmoOperation == GizmoOperation::NONE))
            m_GizmoOperation = GizmoOperation::NONE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Translate", m_GizmoOperation == GizmoOperation::TRANSLATE))
            m_GizmoOperation = GizmoOperation::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", m_GizmoOperation == GizmoOperation::ROTATE))
            m_GizmoOperation = GizmoOperation::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", m_GizmoOperation == GizmoOperation::SCALE))
            m_GizmoOperation = GizmoOperation::SCALE;
        ImGui::SameLine();
        if (ImGui::RadioButton("All", m_GizmoOperation == GizmoOperation::UNIVERSAL))
            m_GizmoOperation = GizmoOperation::UNIVERSAL;

        if (m_GizmoOperation != GizmoOperation::SCALE)
        {
            if (ImGui::RadioButton("Local", m_GizmoMode == GizmoMode::LOCAL))
                m_GizmoMode = GizmoMode::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", m_GizmoMode == GizmoMode::WORLD))
                m_GizmoMode = GizmoMode::WORLD;
        }
        ImGui::End();
    }

    void EditorLayer::OnScenePlay()
    {
        if(m_SceneState == SceneState::Play || m_SceneState == SceneState::Pause) return;
        m_SceneState = SceneState::Play;
        m_CacheScene = m_ActiveScene;
        m_ActiveScene = CreateRef<Scene>(m_CacheScene);

        m_CacheFilePath = m_FilePath;
        m_FilePath = "";

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->StartScene();
		ParticlePhysicsLayer* ppl;
		if(Application::Get().TryGetLayer<ParticlePhysicsLayer>(ppl))
		{
			ppl->SetScene(m_ActiveScene);
		}

		RigidbodyPhysicsLayer* rpl;
		if(Application::Get().TryGetLayer<RigidbodyPhysicsLayer>(rpl))
		{
			rpl->SetScene(m_ActiveScene);
		}
    }

    void EditorLayer::OnSceneStop()
    {
        if(m_SceneState == SceneState::Edit) return;
        m_SceneState = SceneState::Edit;
        m_CacheScene->StopScene();

		ParticlePhysicsLayer* ppl;
		if(Application::Get().TryGetLayer<ParticlePhysicsLayer>(ppl))
		{
			ppl->ResetScene();
		}

		RigidbodyPhysicsLayer* rpl;
		if(Application::Get().TryGetLayer<RigidbodyPhysicsLayer>(rpl))
		{
			rpl->ResetScene();
		}

        m_ActiveScene = m_CacheScene;
        m_CacheScene = nullptr;

        m_FilePath = m_CacheFilePath;
        m_CacheFilePath = "";

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    }

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState != SceneState::Play) return;
		m_SceneState = SceneState::Pause;

		ParticlePhysicsLayer* ppl;
		if(Application::Get().TryGetLayer<ParticlePhysicsLayer>(ppl))
		{
			ppl->ResetScene();
		}

		RigidbodyPhysicsLayer* rpl;
		if(Application::Get().TryGetLayer<RigidbodyPhysicsLayer>(rpl))
		{
			rpl->ResetScene();
		}
	}

	void EditorLayer::OnSceneResume()
	{
		if (m_SceneState != SceneState::Pause) return;
		m_SceneState = SceneState::Play;

		ParticlePhysicsLayer* ppl;
		if(Application::Get().TryGetLayer<ParticlePhysicsLayer>(ppl))
		{
			ppl->SetScene(m_ActiveScene);
		}

		RigidbodyPhysicsLayer* rpl;
		if(Application::Get().TryGetLayer<RigidbodyPhysicsLayer>(rpl))
		{
			rpl->SetScene(m_ActiveScene);
		}
	}

    void EditorLayer::ReloadAssets()
    {
        ShaderLibrary::GetInstance().Load("FlatColor", {FileSource::EditorAsset, "Shaders/FlatColor.vert"}, {FileSource::EditorAsset, "Shaders/FlatColor.frag"});
        ShaderLibrary::GetInstance().Load("Texture", {FileSource::EditorAsset, "Shaders/TextureShader.vert"}, {FileSource::EditorAsset, "Shaders/TextureShader.frag"});
        ShaderLibrary::GetInstance().Load("Default", {FileSource::EditorAsset, "Shaders/DefaultShader.vert"}, {FileSource::EditorAsset, "Shaders/DefaultShader.frag"});
        ShaderLibrary::GetInstance().Load("Bezier", std::unordered_map<ShaderType, Path>{
															{ShaderType::VERTEX_SHADER,{FileSource::EditorAsset, "Shaders/Bezier.vert"}},
															{ShaderType::FRAGMENT_SHADER,{FileSource::EditorAsset, "Shaders/Bezier.frag"}},
															{ShaderType::TESS_CONTROL_SHADER,{FileSource::EditorAsset, "Shaders/Bezier.tessco"}},
															{ShaderType::TESS_EVALUATION_SHADER,{FileSource::EditorAsset, "Shaders/Bezier.tessev"}},
//															{ShaderType::GEOMETRY_SHADER,{FileSource::EditorAsset, "Shaders/Bezier.geom"}},
													});

        Assets::ReloadAll();

        m_PlayTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/Play.png"});
        m_StopTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/Stop.png"});
        m_PauseTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/Pause.png"});
    }
} // Voxymore
// Editor