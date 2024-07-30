//
// Created by ianpo on 12/07/2023.
//

#pragma once

#include "Voxymore/Voxymore.hpp"
#include "Jolt/Jolt.h"

#ifdef VXM_IMPLEMENT
#include VXM_IMPLEMENT
#endif

namespace Voxymore::Core {
	extern Voxymore::Core::Application* CreateApplication(const std::vector<std::string>& arguments);
}

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char *inFMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	// Print to the TTY
	VXM_CORE_INFO("[Jolt Logs] :", buffer);
}

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, JPH::uint inLine)
{
	// Print to the TTY
	::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{inFile, int(inLine), inExpression}, spdlog::level::err, (inMessage != nullptr? inMessage : ""));

	// Breakpoint
	VXM_BREAK();
	return true;
};
//#ifndef VXM_Core_EXPORTS
int main(int argc, char** argv)
{
	std::vector<std::string> arguments(argc);
	for (int i = 0; i < argc; ++i) {
		arguments[i] = argv[i];
	}

	Voxymore::Core::Log::Init();

	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	JPH::RegisterDefaultAllocator();

	// Install trace and assert callbacks
	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)


	VXM_PROFILE_BEGIN_SESSION("startup", "VoxymoreProfile-Startup.json");
	VXM_FRAME_START();
	Voxymore::Core::Application* application = Voxymore::Core::CreateApplication(arguments);
	VXM_FRAME_END();
	VXM_PROFILE_END_SESSION();

	VXM_PROFILE_BEGIN_SESSION("runtime", "VoxymoreProfile-Runtime.json");
	application->Run();
	VXM_PROFILE_END_SESSION();

	VXM_PROFILE_BEGIN_SESSION("shutdown", "VoxymoreProfile-Shutdown.json");

	VXM_FRAME_START();
	delete application;
	VXM_FRAME_END();
	VXM_PROFILE_END_SESSION();

	return 0;
}