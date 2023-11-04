//
// Created by ianpo on 12/07/2023.
//

#pragma once

#include "Application.hpp"
#include "Logger.hpp"
#include "Core.hpp"

namespace Voxymore::Core {
    extern Voxymore::Core::Application* CreateApplication(const std::vector<std::string>& arguments);
}

//#ifndef VXM_Core_EXPORTS
int main(int argc, char** argv)
{
	std::vector<std::string> arguments(argc);
	for (int i = 0; i < argc; ++i) {
		arguments[i] = argv[i];
	}

	Voxymore::Core::Log::Init();

    VXM_PROFILE_BEGIN_SESSION("startup", "VoxymoreProfile-Startup.json");
    Voxymore::Core::Application* application = Voxymore::Core::CreateApplication(arguments);
    VXM_PROFILE_END_SESSION();

    VXM_PROFILE_BEGIN_SESSION("runtime", "VoxymoreProfile-Runtime.json");
    application->Run();
    VXM_PROFILE_END_SESSION();

    VXM_PROFILE_BEGIN_SESSION("shutdown", "VoxymoreProfile-Shutdown.json");
    delete application;
    VXM_PROFILE_END_SESSION();

    return 0;
}