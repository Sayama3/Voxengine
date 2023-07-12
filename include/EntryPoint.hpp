//
// Created by ianpo on 12/07/2023.
//

#ifndef VOXYMORE_ENTRYPOINT_HPP
#define VOXYMORE_ENTRYPOINT_HPP

#include "Application.hpp"
namespace Voxymore::Core {
    extern Voxymore::Core::Application* CreateApplication();
}

int main(int argc, char** argv)
{
    Voxymore::Core::Application* application = Voxymore::Core::CreateApplication();
    application->Run();
    delete application;
}

#endif //VOXYMORE_ENTRYPOINT_HPP
