#define NEWLINE "\n"

#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Core/EntryPoint.hpp"
#include "Voxymore/Editor/EditorLayer.hpp"

namespace Voxymore::Editor {

class Voxengine : public Voxymore::Core::Application {
    public:
        Voxengine(const ApplicationParameters& parameters) : Voxymore::Core::Application(parameters) {
            PushLayer(new EditorLayer());
        }

        ~Voxengine() {}
    };
}

Voxymore::Core::Application* Voxymore::Core::CreateApplication(const std::vector<std::string>& arguments) {
    VXM_PROFILE_FUNCTION();

    std::filesystem::path exePath = arguments[0];

    ApplicationParameters parameters;
    parameters.name = "Voxengine";
    parameters.arguments = arguments;

    return new Voxymore::Editor::Voxengine(parameters);
}