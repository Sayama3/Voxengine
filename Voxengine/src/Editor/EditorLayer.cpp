//
// Created by ianpo on 21/08/2023.
//

#include "Voxymore/Editor/EditorLayer.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Editor/Panels/SystemPanel.hpp"
#include "Voxymore/Editor/Panels/AssetManagerPanel.hpp"
#include "Voxymore/Utils/Platform.hpp"
#include <ImGuizmo.h>


namespace Voxymore::Editor {
    EditorLayer::EditorLayer() : Layer("EditorLayer")
    {
        VXM_PROFILE_FUNCTION();
        Application::Get().GetWindow().SetCursorState(CursorState::None);
    }

    void EditorLayer::OnAttach()
    {
        VXM_PROFILE_FUNCTION();

        auto* imgui = Application::Get().FindLayer<ImGuiLayer>();
		VXM_CHECK(imgui, "ImGui must be set before adding the EditorLayer.")
		if(imgui) {
			auto &imguiLayer = *imgui;
			imguiLayer.AddFont({FileSource::EditorAsset, "Fonts/OpenSans/OpenSans-Regular.ttf"}, 18.0f, FontType::Regular, true);
			imguiLayer.AddFont({FileSource::EditorAsset, "Fonts/OpenSans/OpenSans-Bold.ttf"}, 18.0f, FontType::Bold);
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
		m_Viewports.push_back(CreateRef<Viewport>(specification));

		auto handle = Project::GetConfig().startSceneId;
		if(handle.has_value() && AssetManager::IsAssetHandleValid(handle.value()))
        {
            OpenScene(Project::GetConfig().startSceneId.value());
        }
        else
        {
            CreateNewScene();
        }

		m_PanelCreator.insert({PropertyPanel::StaticGetTypeID(), PanelMetadata(PropertyPanel::StaticGetName(), PropertyPanel::StaticGetTypeID(), &PropertyPanel::CreatePanel)});
//		m_PanelCreator.insert({ShaderPanel::StaticGetTypeID(), PanelMetadata(ShaderPanel::StaticGetName(), ShaderPanel::StaticGetTypeID(), &ShaderPanel::CreatePanel)});
		m_PanelCreator.insert({SceneHierarchyPanel::StaticGetTypeID(), PanelMetadata(SceneHierarchyPanel::StaticGetName(), SceneHierarchyPanel::StaticGetTypeID(), &SceneHierarchyPanel::CreatePanel)});
		m_PanelCreator.insert({SystemPanel::StaticGetTypeID(), PanelMetadata(SystemPanel::StaticGetName(), SystemPanel::StaticGetTypeID(), &SystemPanel::CreatePanel)});
		m_PanelCreator.insert({ContentBrowserPanel::StaticGetTypeID(), PanelMetadata(ContentBrowserPanel::StaticGetName(), ContentBrowserPanel::StaticGetTypeID(), &ContentBrowserPanel::CreatePanel)});
//		m_PanelCreator.insert({AssetManagerPanel::StaticGetTypeID(), PanelMetadata(AssetManagerPanel::StaticGetName(), AssetManagerPanel::StaticGetTypeID(), &AssetManagerPanel::CreatePanel)});

		// Create the default panels'
		// TODO: Make a system to be able to save and load the last Panel Configuration.
		for (auto&& [name, metadata] : m_PanelCreator) {
			Ref<BasePanel> panel = metadata.createPanelFunc();
			m_Panels.push_back(panel);
		}
    }

    void EditorLayer::OnDetach()
    {
        VXM_PROFILE_FUNCTION();

        Project::RemoveOnLoad(m_OnProjectReloadId);
		m_PanelCreator.clear();
		m_Panels.clear();
    }

    //TODO: Pause the update when m_SceneState == SceneState::Pause
    void EditorLayer::OnUpdate(TimeStep timeStep) {
        VXM_PROFILE_FUNCTION();


		std::for_each(m_Viewports.begin(), m_Viewports.end(), [timeStep](Ref<Viewport> viewportPtr) {viewportPtr->PrepareFramebuffer();});


        if(m_SceneState == SceneState::Edit || m_SceneState == SceneState::Pause)
		{
			std::for_each(m_Viewports.begin(), m_Viewports.end(), [timeStep](Ref<Viewport> viewportPtr) {viewportPtr->UpdateCamera(timeStep);});
		}

		{
			switch (m_SceneState)
			{
				case SceneState::Pause:
				case SceneState::Edit:
				{
					m_ActiveScene->OnUpdateEditor(timeStep, nullptr, false);
					break;
				}
				case SceneState::Play:
				{
					m_ActiveScene->OnUpdateRuntime(timeStep, false);
					break;
				}
				default:
				{
					VXM_ASSERT(false, "State {0} not handled yet.", static_cast<int>(m_SceneState));
					break;
				}
			}
		}

		for(Ref<Viewport> viewportPtr : m_Viewports)
		{
			if(!viewportPtr->IsVisible()) continue;

			if(viewportPtr->GetSize().x == 0 || viewportPtr->GetSize().y == 0) continue;

			// Resize
			m_ActiveScene->SetViewportSize(viewportPtr->GetSize().x, viewportPtr->GetSize().y);

			viewportPtr->BindFramebuffer();

			{
				VXM_PROFILE_SCOPE("Rendering Scene");

				switch (m_SceneState)
				{
					case SceneState::Pause:
					case SceneState::Edit:
					{
						m_ActiveScene->RenderEditor(timeStep, viewportPtr->GetCamera());
						break;
					}
					case SceneState::Play:
					{
						m_ActiveScene->RenderRuntime(timeStep);
						break;
					}
					default:
					{
						VXM_ASSERT(false, "State {0} not handled yet.", static_cast<int>(m_SceneState));
						break;
					}
				}
				viewportPtr->PostRender(m_ActiveScene.get());
				viewportPtr->UnbindFramebuffer();
			}
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
        EventDispatcher dispatcher(event);
		for(auto viewPtr : m_Viewports){
			if(viewPtr->IsHovered()) {
				viewPtr->OnEvent(event);
				break;
			}
		}

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
			if(ImGui::MenuItem("Viewport")) {
				if(!m_UnloadedViewport.empty()) {
					auto viewport = m_UnloadedViewport.back();
					viewport->Open();
					m_Viewports.push_back(viewport);
					m_UnloadedViewport.pop_back();
				} else {
					FramebufferSpecification specification(1280, 720);
					specification.Attachements = {FramebufferTextureFormat::Color, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};
					m_Viewports.push_back(CreateRef<Viewport>(specification));
				}
			}
			for (auto&& [typeId, panelMetadata] : m_PanelCreator) {
				if (ImGui::MenuItem(panelMetadata.name.c_str())) {
					auto it = m_UnloadedPanels.find(typeId);
					if(it != m_UnloadedPanels.end()) {
						Ref<BasePanel> panel = it->second;
						panel->Open();
						m_Panels.push_back(panel);
						m_UnloadedPanels.erase(it);
					} else {
						m_Panels.push_back(panelMetadata.createPanelFunc());
					}
				}
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Views"))
		{
			if (ImGui::MenuItem("Reset Camera")) {
				for (auto viewportPtr : m_Viewports)
				{
					if(viewportPtr->IsFocused())
					{
						viewportPtr->SetFocusTarget({0,0,0});
						viewportPtr->SetDistance(10);
					}
				}
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
                    auto selected = PropertyPanel::GetSelectedEntity();
                    if (selected) {
                        auto selectedTc = selected.GetComponent<TransformComponent>();
						std::for_each(m_Viewports.begin(), m_Viewports.end(), [&selectedTc](Ref<Viewport> viewportPtr){if(viewportPtr->IsFocused()) viewportPtr->SetFocusTarget(selectedTc.GetPosition());});
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
				bool anyHovered = false;
				for(auto ptr : m_Viewports) {
					if (ptr->IsHovered()) {
						anyHovered = true;
						if (!ptr->HasHoveredEntity() && m_GizmoOperation == GizmoOperation::NONE) PropertyPanel::Reset();
						else if (ptr->HasHoveredEntity() && !ImGuizmo::IsOver() && !ImGuizmo::IsUsingAny() && !control && !shift && !alt) {
							PropertyPanel::SetSelectedEntity(ptr->GetHoveredEntity());
						}
						break;
					}
				}
				if(!anyHovered) {
					PropertyPanel::Reset();
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
			auto handle = Project::GetConfig().startSceneId;
            if(handle.has_value() && AssetManager::IsAssetHandleValid(handle.value()))
            {
                OpenScene(handle.value());
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
            VXM_WARNING("Cannot save the scene if we are not in edit mode.");
            return;
        }
        VXM_TRACE("Save Scene As");
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
        VXM_INFO("Unloading and Delete Previous Scene");
        if(m_ActiveScene != nullptr && SceneManager::HasScene(m_ActiveScene->id()))
        {
            SceneManager::DeleteScene(m_ActiveScene->id());
            m_ActiveScene = nullptr;
        }

        VXM_TRACE("Create New Scene");
        m_FilePath = std::string();
        m_ActiveScene = SceneManager::CreateScene();
        m_ActiveScene->SetViewportSize(1280, 720);
        SceneHierarchyPanel::SetContext(m_ActiveScene);

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
            VXM_WARNING("Cannot save the scene if we are not in edit mode.");
            return;
        }
        if(!m_FilePath.empty())
        {
            VXM_TRACE("Save Scene");
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
        VXM_TRACE("Open Scene");
        std::filesystem::path file = FileDialogs::OpenFile({"Voxymore Scene (*.vxm_scn)", "*.vxm_scn"});
        if(!file.empty())
        {
            OpenScene(file);
        }
    }

    void EditorLayer::OpenScene(const Path& path)
    {
        VXM_TRACE("Open Scene with path");
        if(!path.empty())
        {
            OpenScene(path.GetFullPath());
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        VXM_INFO("Unloading and Delete Previous Scene");
        if(m_ActiveScene != nullptr && SceneManager::HasScene(m_ActiveScene->id()))
        {
            SceneManager::DeleteScene(m_ActiveScene->id());
            m_ActiveScene = nullptr;
        }

        VXM_TRACE("Open Scene from path {0}", path.string());
        m_FilePath = path.string();
		Ref<Asset> asset = Project::GetActive()->GetEditorAssetManager()->GetOrCreateAsset(Path::GetPath(path));
		if(asset && asset->GetType() == AssetType::Scene) {
			m_ActiveScene = CastPtr<Scene>(asset);
			SceneHierarchyPanel::SetContext(m_ActiveScene);
		} else {
			m_ActiveScene = CreateRef<Scene>();
			SceneHierarchyPanel::SetContext(m_ActiveScene);
		}
    }

    void EditorLayer::OpenScene(AssetHandle id)
    {
        VXM_INFO("Unloading and Delete Previous Scene");

        VXM_ASSERT(SceneManager::HasScene(id), "No scene with the ID '{0}'", id);
        m_ActiveScene = AssetManager::GetAssetAs<Scene>(id);
		if(m_ActiveScene)
		{
			auto metadata = Project::GetActive()->GetEditorAssetManager()->GetMetadata(id);
			m_FilePath = metadata.FilePath;
		}
		SceneHierarchyPanel::SetContext(m_ActiveScene);
    }

    void EditorLayer::OnImGuiRender() {
        VXM_PROFILE_FUNCTION();
        RenderDockspace();

		for(uint32_t i = 0; i < m_Panels.size(); ++i)
		{
			Ref<BasePanel> panel = m_Panels[i];
			{
				panel->BeginPanel();
				panel->OnImGuiRender();
				panel->EndPanel();
				if(!panel->IsOpen()) {
					m_UnloadedPanels.insert({panel->GetTypeID(), panel});
					m_Panels.erase(m_Panels.begin() + i);
					--i;
				}
			}
		}

		DrawGizmosWindow();

		for(uint32_t i = 0; i < m_Viewports.size(); ++i)
		{
			Ref<Viewport> viewport = m_Viewports[i];
			viewport->BeginPanel();
			viewport->OnImGuiRender();

			if(m_SceneState == SceneState::Edit || m_SceneState == SceneState::Pause)  {
				ImGuizmo::PushID(i);
				viewport->DrawGizmos(m_Panels, m_GizmoOperation, m_GizmoMode, m_GizmoTranslationSnapValue, m_GizmoRotationSnapValue, m_GizmoScaleSnapValue);
				ImGuizmo::PopID();
			}

			viewport->EndPanel();

			if(!viewport->IsOpen()) {
				m_UnloadedViewport.push_back(viewport);
				m_Viewports.erase(m_Viewports.begin() + i);
				--i;
			}
		}

        DrawImGuiToolbar();
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

        SceneHierarchyPanel::SetContext(m_ActiveScene);
//        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
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

        SceneHierarchyPanel::SetContext(m_ActiveScene);
//        m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
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
		auto assetManager = Project::GetActive()->GetEditorAssetManager();
        // ShaderLibrary::GetInstance().Load("FlatColor", {FileSource::EditorAsset, "Shaders/FlatColor.vert"}, {FileSource::EditorAsset, "Shaders/FlatColor.frag"});
        // ShaderLibrary::GetInstance().Load("Texture", {FileSource::EditorAsset, "Shaders/TextureShader.vert"}, {FileSource::EditorAsset, "Shaders/TextureShader.frag"});
        // ShaderLibrary::GetInstance().Load("Default", {FileSource::EditorAsset, "Shaders/DefaultShader.vert"}, {FileSource::EditorAsset, "Shaders/DefaultShader.frag"});
        // ShaderLibrary::GetInstance().Load("Bezier4", std::unordered_map<ShaderType, Path>{
		// 													{ShaderType::VERTEX_SHADER,{FileSource::EditorAsset, "Shaders/Bezier4/Bezier4.vert"}},
		// 													{ShaderType::FRAGMENT_SHADER,{FileSource::EditorAsset, "Shaders/Bezier4/Bezier4.frag"}},
		// 													{ShaderType::TESS_CONTROL_SHADER,{FileSource::EditorAsset, "Shaders/Bezier4/Bezier4.tessco"}},
		// 													{ShaderType::TESS_EVALUATION_SHADER,{FileSource::EditorAsset, "Shaders/Bezier4/Bezier4.tessev"}},
		// 													{ShaderType::GEOMETRY_SHADER,{FileSource::EditorAsset, "Shaders/Bezier4/Bezier4.geom"}},
		// 											});
        // ShaderLibrary::GetInstance().Load("Bezier", std::unordered_map<ShaderType, Path>{
		// 													{ShaderType::VERTEX_SHADER,{FileSource::EditorAsset, "Shaders/Bezier/Bezier.vert"}},
		// 													{ShaderType::FRAGMENT_SHADER,{FileSource::EditorAsset, "Shaders/Bezier/Bezier.frag"}},
		// 													{ShaderType::TESS_CONTROL_SHADER,{FileSource::EditorAsset, "Shaders/Bezier/Bezier.tessco"}},
		// 													{ShaderType::TESS_EVALUATION_SHADER,{FileSource::EditorAsset, "Shaders/Bezier/Bezier.tessev"}},
		// 													{ShaderType::GEOMETRY_SHADER,{FileSource::EditorAsset, "Shaders/Bezier/Bezier.geom"}},
		// 											});

        m_PlayTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/Play.png"});
        m_StopTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/Stop.png"});
        m_PauseTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/Pause.png"});
    }
} // Voxymore
// Editor