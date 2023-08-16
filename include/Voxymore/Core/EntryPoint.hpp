//
// Created by ianpo on 12/07/2023.
//

#ifndef VOXYMORE_ENTRYPOINT_HPP
#define VOXYMORE_ENTRYPOINT_HPP

#include "Application.hpp"
#include "Logger.hpp"
#include "Core.hpp"

namespace Voxymore::Core {
    extern Voxymore::Core::Application* CreateApplication();
}

//#ifndef VXM_Core_EXPORTS
int main(int argc, char** argv)
{
    Voxymore::Core::Log::Init();

    VXM_PROFILE_BEGIN_SESSION("startup", "Logs/VoxymoreProfile-Startup.json");
    Voxymore::Core::Application* application = Voxymore::Core::CreateApplication();
    VXM_PROFILE_END_SESSION();

    VXM_PROFILE_BEGIN_SESSION("runtime", "Logs/VoxymoreProfile-Runtime.json");
    application->Run();
    VXM_PROFILE_END_SESSION();

    VXM_PROFILE_BEGIN_SESSION("shutdown", "Logs/VoxymoreProfile-Shutdown.json");
    delete application;
    VXM_PROFILE_END_SESSION();

    return 0;
}
//#endif

#endif //VOXYMORE_ENTRYPOINT_HPP
