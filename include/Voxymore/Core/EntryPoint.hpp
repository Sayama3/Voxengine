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

    Voxymore::Core::Application* application = Voxymore::Core::CreateApplication();
    application->Run();
    delete application;

    return 0;
}
//#endif

#endif //VOXYMORE_ENTRYPOINT_HPP
